#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "Player.h"

ThirdPersonCamera::ThirdPersonCamera()
{
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

BOOL ThirdPersonCamera::Rotate(const XMFLOAT3& xmf3Rotation)
{
	return Rotate(xmf3Rotation.x, xmf3Rotation.y, xmf3Rotation.z);
}

BOOL ThirdPersonCamera::Rotate(const XMVECTOR& xmvRotation)
{
	XMFLOAT3 xmf3Rotation;
	XMStoreFloat3(&xmf3Rotation, xmvRotation);
	return Rotate(xmf3Rotation.x, xmf3Rotation.y, xmf3Rotation.z);
}

BOOL ThirdPersonCamera::Rotate(float fPitch, float fYaw, float fRoll)
{
	BOOL bResult = FALSE;

	if (fPitch != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(fPitch));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
		bResult = TRUE;
	}
	if (fYaw != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(fYaw));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
		bResult = TRUE;
	}
	if (fRoll != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(fRoll));
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
		bResult = TRUE;
	}

	return bResult;
}

void ThirdPersonCamera::GenerateViewMatrix()
{
	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Up, m_xmf3Look));
	m_xmf3Up = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Look, m_xmf3Right));
	m_xmf4x4View._11 = m_xmf3Right.x; m_xmf4x4View._12 = m_xmf3Up.x; m_xmf4x4View._13 = m_xmf3Look.x;
	m_xmf4x4View._21 = m_xmf3Right.y; m_xmf4x4View._22 = m_xmf3Up.y; m_xmf4x4View._23 = m_xmf3Look.y;
	m_xmf4x4View._31 = m_xmf3Right.z; m_xmf4x4View._32 = m_xmf3Up.z; m_xmf4x4View._33 = m_xmf3Look.z;
	m_xmf4x4View._41 = -Vector3::DotProduct(m_xmf3Position, m_xmf3Right);
	m_xmf4x4View._42 = -Vector3::DotProduct(m_xmf3Position, m_xmf3Up);
	m_xmf4x4View._43 = -Vector3::DotProduct(m_xmf3Position, m_xmf3Look);

	m_xmf4x4ViewPerspectiveProject = Matrix4x4::Multiply(m_xmf4x4View, m_xmf4x4PerspectiveProject);
	m_xmf4x4OrthographicProject = Matrix4x4::Multiply(m_xmf4x4View, m_xmf4x4OrthographicProject);

	m_xmf4x4InverseView._11 = m_xmf3Right.x;		m_xmf4x4InverseView._12 = m_xmf3Right.y;		m_xmf4x4InverseView._13 = m_xmf3Right.z;
	m_xmf4x4InverseView._21 = m_xmf3Up.x;			m_xmf4x4InverseView._22 = m_xmf3Up.y;			m_xmf4x4InverseView._23 = m_xmf3Up.z;
	m_xmf4x4InverseView._31 = m_xmf3Look.x;			m_xmf4x4InverseView._32 = m_xmf3Look.y;			m_xmf4x4InverseView._33 = m_xmf3Look.z;
	m_xmf4x4InverseView._41 = m_xmf3Position.x;		m_xmf4x4InverseView._42 = m_xmf3Position.y;		m_xmf4x4InverseView._43 = m_xmf3Position.z;

	m_xmFrustumView.Transform(m_xmFrustumWorld, XMLoadFloat4x4(&m_xmf4x4InverseView));
}

void ThirdPersonCamera::Initialize(std::shared_ptr<Player> pOwnerPlayer)
{
	Camera::Initialize(pOwnerPlayer);
}

void ThirdPersonCamera::Update(float fElapsedTime)
{
	if (m_wpOwner.expired()) __debugbreak();

	if (auto p = dynamic_pointer_cast<Player>(m_wpOwner.lock())) {
		XMFLOAT3 xmf3OwnerRight = p->GetTransform()->GetRight();
		XMFLOAT3 xmf3OwnerUp = p->GetTransform()->GetUp();
		XMFLOAT3 xmf3OwnerLook = p->GetTransform()->GetLook();
		
		XMFLOAT4X4 xmf4x4Rotate = Matrix4x4::Identity();
		xmf4x4Rotate._11 = xmf3OwnerRight.x; xmf4x4Rotate._21 = xmf3OwnerUp.x; xmf4x4Rotate._31 = xmf3OwnerLook.x;
		xmf4x4Rotate._12 = xmf3OwnerRight.y; xmf4x4Rotate._22 = xmf3OwnerUp.y; xmf4x4Rotate._32 = xmf3OwnerLook.y;
		xmf4x4Rotate._13 = xmf3OwnerRight.z; xmf4x4Rotate._23 = xmf3OwnerUp.z; xmf4x4Rotate._33 = xmf3OwnerLook.z;

		XMFLOAT3 xmf3Offset = Vector3::TransformCoord(p->GetCameraOffset(), xmf4x4Rotate);
		XMFLOAT3 xmf3Position = Vector3::Add(p->GetTransform()->GetPosition(), xmf3Offset);
		XMFLOAT3 xmf3Direction = Vector3::Subtract(xmf3Position, m_xmf3Position);
		float fLength = Vector3::Length(xmf3Direction);
		xmf3Direction = Vector3::Normalize(xmf3Direction);

		float fTimeLagScale = fElapsedTime * (1.0f / 0.25f);
		float fDistance = fLength * fTimeLagScale;
		if (fDistance > fLength) fDistance = fLength;
		if (fLength < 0.01f) fDistance = fLength;
		if (fDistance > 0) {
			SetPosition(Vector3::Add(m_xmf3Position, xmf3Direction, fDistance));
			SetLookAt(m_xmf3Position, p->GetTransform()->GetPosition(), p->GetTransform()->GetUp());
		}
	}

	Camera::Update(fElapsedTime);
}