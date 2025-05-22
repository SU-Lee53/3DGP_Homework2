#include "stdafx.h"
#include "ExplosiveObject.h"

using namespace std;

std::shared_ptr<Mesh> ExplosiveObject::m_pExplosionMesh = nullptr;
std::array<XMFLOAT3, EXPLOSION_DEBRISES> ExplosiveObject::m_xmf3SphereVectors = {};

ExplosiveObject::ExplosiveObject()
{
	m_eObjectType = TAG_GAMEOBJECT_TYPE_EXPLOSIVE;
}

ExplosiveObject::~ExplosiveObject()
{
}

void ExplosiveObject::Initialize()
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

void ExplosiveObject::Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera)
{
	if (m_bExplosionCompleted) return;

	if (m_bBlowingUp) {
		for (XMFLOAT4X4& xmf4x4Transform : m_xmf4x4Transforms) {
			GameObject::Render(hDCFrameBuffer, xmf4x4Transform, m_pExplosionMesh);
		}
	}
	else {
		GameObject::Render(hDCFrameBuffer, pCamera);
	}
}

void ExplosiveObject::OnPicked()
{
	m_bBlowingUp = TRUE;
}

void ExplosiveObject::BeginCollision(std::shared_ptr<GameObject> pOther)
{
}

void ExplosiveObject::PrepareExplosion()
{
	for (XMFLOAT3& vec : m_xmf3SphereVectors) {
		XMStoreFloat3(&vec, RandomGenerator::GenerateRandomUnitVectorOnSphere());
	}

	m_pExplosionMesh = std::make_shared<Mesh>();
	MeshHelper::CreateCubeMesh(m_pExplosionMesh, 0.5f, 0.5f, 0.5f);
}
