#include "stdafx.h"
#include "ExplosiveObject.h"

using namespace std;

std::shared_ptr<Mesh<DiffusedVertex>> ExplosiveObject::m_pExplosionMesh = nullptr;
std::array<XMFLOAT3, EXPLOSION_DEBRISES> ExplosiveObject::m_xmf3SphereVectors = {};

ExplosiveObject::ExplosiveObject()
{
	m_eObjectType = TAG_GAMEOBJECT_TYPE_EXPLOSIVE;
}

ExplosiveObject::~ExplosiveObject()
{
}

void ExplosiveObject::Initialize(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

void ExplosiveObject::Update(float fTimeElapsed)
{
	if (m_bExplosionCompleted) return;

	if (m_bBlowingUp) {
		m_fElapsedTimes += fTimeElapsed;
		if (m_fElapsedTimes <= m_fDuration) {
			XMFLOAT3 xmf3Position = m_pTransform->GetPosition();
			for (int i = 0; i < EXPLOSION_DEBRISES; i++)
			{
				m_xmf4x4Transforms[i] = Matrix4x4::Identity();
				m_xmf4x4Transforms[i]._41 = xmf3Position.x + m_xmf3SphereVectors[i].x * m_fExplosionSpeed * m_fElapsedTimes;
				m_xmf4x4Transforms[i]._42 = xmf3Position.y + m_xmf3SphereVectors[i].y * m_fExplosionSpeed * m_fElapsedTimes;
				m_xmf4x4Transforms[i]._43 = xmf3Position.z + m_xmf3SphereVectors[i].z * m_fExplosionSpeed * m_fElapsedTimes;
				m_xmf4x4Transforms[i] = Matrix4x4::Multiply(Matrix4x4::RotationAxis(m_xmf3SphereVectors[i], m_fExplosionRotation * m_fElapsedTimes), m_xmf4x4Transforms[i]);
			}
		}
		else {
			if (m_bAutoReset) {
				Reset();
			}
			else {
				m_bBlowingUp = FALSE;
				m_bExplosionCompleted = TRUE;
				m_fElapsedTimes = 0.0f;
				m_bActive = FALSE;
			}
		}
	}
	else {
		GameObject::Update(fTimeElapsed);
	}
}

void ExplosiveObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Camera> pCamera)
{
	if (m_bExplosionCompleted) return;

	if (m_bBlowingUp) {
		shared_ptr<Shader> pInstancingShader = SHADER.GetShader(TAG_SHADER_INSTANCED);

		pInstancingShader->SetRootSignature(pd3dCommandList);
		pInstancingShader->SetPipelineState(pd3dCommandList);

		pCamera->SetViewportAndScissorRects(pd3dCommandList);
		pCamera->UpdateShaderVariables(pd3dCommandList);

		std::vector<VS_INSTANCING_DATA> transformDatas;
		transformDatas.resize(EXPLOSION_DEBRISES);

		XMFLOAT4 xmf4Color = m_xmf4Color;

		std::transform(m_xmf4x4Transforms.begin(), m_xmf4x4Transforms.end(), transformDatas.begin(), [xmf4Color](const XMFLOAT4X4& xmf4x4Transform) {

			XMMATRIX xmmtxTransform = XMLoadFloat4x4(&xmf4x4Transform);
			XMFLOAT4X4 xmf4x4Transformdata;
			XMStoreFloat4x4(&xmf4x4Transformdata, XMMatrixTranspose(xmmtxTransform));

			return VS_INSTANCING_DATA{ Matrix4x4::Identity(), xmf4x4Transformdata, xmf4Color };
		});

		pInstancingShader->UpdateShaderVariables(pd3dCommandList, transformDatas.data(), transformDatas.size());

		m_pExplosionMesh->Render(pd3dCommandList, EXPLOSION_DEBRISES);
	}
	else {
		GameObject::Render(pd3dCommandList, pCamera);
	}
}

void ExplosiveObject::OnPicked()
{
	m_bBlowingUp = TRUE;
}

void ExplosiveObject::BeginCollision(std::shared_ptr<GameObject> pOther)
{
}

void ExplosiveObject::PrepareExplosion(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	for (XMFLOAT3& vec : m_xmf3SphereVectors) {
		XMStoreFloat3(&vec, RandomGenerator::GenerateRandomUnitVectorOnSphere());
	}

	m_pExplosionMesh = std::make_shared<Mesh<DiffusedVertex>>();
	MeshHelper::CreateCubeMesh(pd3dDevice, pd3dCommandList, m_pExplosionMesh, 0.5f, 0.5f, 0.5f);
}
