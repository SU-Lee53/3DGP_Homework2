#include "stdafx.h"
#include "ObstacleObject.h"
#include "TankPlayer.h"
#include "BulletObject.h"
#include "TankObject.h"
#include "WallsObject.h"

using namespace std;

void ObstacleObject::Initialize(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	shared_ptr<Mesh<DiffusedVertex>> pCubeMesh = make_shared<Mesh<DiffusedVertex>>();
	float fExtents = 2.f;
	MeshHelper::CreateCubeMesh(pd3dDevice, pd3dCommandList, pCubeMesh, fExtents, fExtents, fExtents);

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

void ObstacleObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<class Camera> pCamera)
{
	GameObject::Render(pd3dCommandList, pCamera);
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
