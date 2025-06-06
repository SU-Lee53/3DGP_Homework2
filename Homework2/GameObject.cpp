#include "stdafx.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"

GameObject::GameObject()
{
	m_pTransform = std::make_shared<Transform>();
}

void GameObject::SetMesh(const std::shared_ptr<Mesh_Base>& pMesh)
{
	m_pMesh = pMesh;
	m_xmOBB = m_pMesh->GetOBB();

}

XMFLOAT4X4 GameObject::GetModelTransform()
{
	XMFLOAT4X4 xmf4x4Model;
	
	float fPitch = XMConvertToRadians(m_xmf3DefaultOrientation.x);
	float fYaw = XMConvertToRadians(m_xmf3DefaultOrientation.y);
	float fRoll = XMConvertToRadians(m_xmf3DefaultOrientation.z);
	
	XMStoreFloat4x4(&xmf4x4Model, XMMatrixRotationRollPitchYaw(fPitch, fYaw, fRoll));
	return xmf4x4Model; 
}

void GameObject::LookTo(const XMFLOAT3& xmf3LookTo, const XMFLOAT3& xmf3Up)
{
	/*XMFLOAT4X4 xmf4x4View = Matrix4x4::LookToLH(GetPosition(), xmf3LookTo, xmf3Up);
	m_xmf4x4World = Matrix4x4::Transpose(xmf4x4View);*/
}

void GameObject::LookAt(const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up)
{
	/*XMFLOAT4X4 xmf4x4View = Matrix4x4::LookAtLH(GetPosition(), xmf3LookAt, xmf3Up);
	m_xmf4x4World = Matrix4x4::Transpose(xmf4x4View);*/
}

void GameObject::UpdateBoundingBox()
{
	if (m_pMesh) {
		XMFLOAT4X4 xmf4x4FinalWorld = m_pTransform->GetWorldMatrix();
		if (!XMVector3Equal(XMLoadFloat3(&m_xmf3DefaultOrientation), XMVectorZero())) {
			XMMATRIX xmmtxCurrentWorld = XMLoadFloat4x4(&m_pTransform->GetWorldMatrix());
			XMMATRIX xmmtxAdditionalRotation = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(m_xmf3DefaultOrientation.x),
				XMConvertToRadians(m_xmf3DefaultOrientation.y),
				XMConvertToRadians(m_xmf3DefaultOrientation.z)
			);
			XMStoreFloat4x4(&xmf4x4FinalWorld, XMMatrixMultiply(xmmtxAdditionalRotation, xmmtxCurrentWorld));
		}

		m_pMesh->GetOBB().Transform(m_xmOBB, XMLoadFloat4x4(&xmf4x4FinalWorld));
		XMStoreFloat4(&m_xmOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOBB.Orientation)));
	}
}

void GameObject::Initialize(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

void GameObject::Update(float fElapsedTime)
{
	if (m_bActive) {
		m_pTransform->Update();
		UpdateBoundingBox();
	}

}

void GameObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<class Camera> pCamera)
{
	if (pCamera->IsInFrustum(m_xmOBB)) {
		RenderObject(pd3dCommandList, pCamera);
	}
}

void GameObject::RenderObject(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<class Camera> pCamera)
{
	if (!m_pMesh) {
		return;
	}
	
	if (!m_pShader) {
		return;
	}

	m_pShader->SetRootSignature(pd3dCommandList);
	m_pShader->SetPipelineState(pd3dCommandList);
	pCamera->SetViewportAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	m_pMesh->Render(pd3dCommandList, 1);
}

void GameObject::GenerateRayForPicking(XMVECTOR& xmvPickPosition, const XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection) const
{
	XMMATRIX xmmtxAdditionalRotation = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(m_xmf3DefaultOrientation.x),
		XMConvertToRadians(m_xmf3DefaultOrientation.y),
		XMConvertToRadians(m_xmf3DefaultOrientation.z)
	);

	XMMATRIX xmmtxToModel = XMMatrixInverse(NULL, XMMatrixMultiply(XMMatrixMultiply(xmmtxAdditionalRotation, XMLoadFloat4x4(&m_pTransform->GetWorldMatrix())), xmmtxView));

	XMFLOAT3 xmf3CameraOrigin{ 0.f, 0.f,0.f };
	xmvPickRayOrigin = XMVector3TransformCoord(XMLoadFloat3(&xmf3CameraOrigin), xmmtxToModel);
	xmvPickRayDirection = XMVector3TransformCoord(xmvPickPosition, xmmtxToModel);
	xmvPickRayDirection = XMVector3Normalize(xmvPickRayDirection - xmvPickRayOrigin);
}

BOOL GameObject::PickObjectByRayIntersection(XMVECTOR& xmvPickPosition, const XMMATRIX& xmmtxView, float& fHitDistance) const
{
	int nIntersected = 0;
	if (m_pMesh) {
		XMVECTOR xmvPickRayOrigin, xmvPickRayDirection;
		GenerateRayForPicking(xmvPickPosition, xmmtxView, xmvPickRayOrigin, xmvPickRayDirection);
		//nIntersected = m_pMesh->CheckRayIntersection(xmvPickRayOrigin, xmvPickRayDirection, fHitDistance);

		return m_pMesh->GetOBB().Intersects(xmvPickRayOrigin, xmvPickRayDirection, fHitDistance);

	}

	return FALSE;
}

void GameObject::AdjustHeightToFloor(float fFloorHeight)
{
	/*
	*	- OBB 를 이용하여 바닥에 붙임
	*/

	XMFLOAT3 xmf3CurrentPos = m_pTransform->GetPosition();

	XMVECTOR xmvOBBExtent = XMLoadFloat3(&m_xmOBB.Extents);
	XMVECTOR xmvOBBOrientationQuat = XMLoadFloat4(&m_xmOBB.Orientation);

	// 일단 회전만 시켜서 높이 구함
	XMVECTOR xmvRotatedExtent = XMVector3Rotate(xmvOBBExtent, xmvOBBOrientationQuat);
	float fHeight = fFloorHeight + XMVectorGetY(xmvRotatedExtent);

	// 현위치에서 바닥
	if (xmf3CurrentPos.y < fHeight) {
		xmf3CurrentPos.y = fHeight;
		m_pTransform->SetPosition(xmf3CurrentPos);
	}
}

void GameObject::CreateShaderVariables(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	m_upcbTransfromData = std::make_unique<ConstantBuffer<CB_TRANSFORM_DATA>>(pd3dDevice, pd3dCommandList);
}

void GameObject::UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	XMMATRIX xmmtxTransform = XMLoadFloat4x4(&m_pTransform->GetWorldMatrix());
	XMFLOAT4X4 xmf4x4Transform;
	XMStoreFloat4x4(&xmf4x4Transform, XMMatrixTranspose(xmmtxTransform));

	XMFLOAT4X4 xmf4x4Model = GetModelTransform();
	XMStoreFloat4x4(&xmf4x4Model, XMLoadFloat4x4(&xmf4x4Model));
	xmf4x4Model = Matrix4x4::Transpose(xmf4x4Model);

	CB_TRANSFORM_DATA data{ xmf4x4Model, xmf4x4Transform };

	m_upcbTransfromData->UpdateData(data);

	m_upcbTransfromData->SetBufferToPipeline(pd3dCommandList, 1);
}
