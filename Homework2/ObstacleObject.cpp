#include "stdafx.h"
#include "ObstacleObject.h"
#include "TankPlayer.h"
#include "BulletObject.h"
#include "TankObject.h"
#include "WallsObject.h"

using namespace std;

void ObstacleObject::Initialize()
{
	shared_ptr<Mesh> pCubeMesh = make_shared<Mesh>();
	float fExtents = 2.f;
	MeshHelper::CreateCubeMesh(pCubeMesh, fExtents, fExtents, fExtents);

	SetMesh(pCubeMesh);

	XMFLOAT3 xmf3InitialPosition{};
	xmf3InitialPosition.x = RandomGenerator::GenerateRandomFloatInRange(-45.f, 45.f);
	xmf3InitialPosition.y = 0;
	xmf3InitialPosition.z = RandomGenerator::GenerateRandomFloatInRange(-45.f, 45.f);
	m_pTransform->SetPosition(xmf3InitialPosition);
}

void ObstacleObject::Update(float fElapsedTime)
{
	GameObject::Update(fElapsedTime);
}

void ObstacleObject::Render(HDC hDCFrameBuffer, std::shared_ptr<class Camera> pCamera)
{
	GameObject::Render(hDCFrameBuffer, pCamera);
}

void ObstacleObject::BeginCollision(std::shared_ptr<GameObject> pOther)
{
	if (auto p = dynamic_pointer_cast<TankPlayer>(pOther)) {

	}
	else if (auto p = dynamic_pointer_cast<TankObject>(pOther)) {

	}
	else if (auto p = dynamic_pointer_cast<BulletObject>(pOther)) {

	}
	else if (auto p = dynamic_pointer_cast<WallsObject>(pOther)) {

	}
}
