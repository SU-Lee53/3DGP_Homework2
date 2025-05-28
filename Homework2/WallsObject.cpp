#include "stdafx.h"
#include "WallsObject.h"

using namespace std;

WallsObject::WallsObject()
{
}

WallsObject::~WallsObject()
{
}

void WallsObject::Initialize(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 45.0f;
	shared_ptr<Mesh<DiffusedVertex>> pWallMesh = make_shared<Mesh<DiffusedVertex>>();
	MeshHelper::CreateWallMesh(pd3dDevice, pd3dCommandList, pWallMesh, fHalfWidth * 2.0f, fHalfHeight * 2.0f, fHalfDepth * 2.0f, 30);

	m_pTransform->SetPosition(0.0f, fHalfHeight, 0.0f);
	SetMesh(std::move(pWallMesh));
	SetColor(RGB(0, 0, 0));
	SetWallPlane(0, XMFLOAT4{ +1.0f, 0.0f, 0.0f, fHalfWidth });
	SetWallPlane(1, XMFLOAT4{ -1.0f, 0.0f, 0.0f, fHalfWidth });
	SetWallPlane(2, XMFLOAT4{ 0.0f, +1.0f, 0.0f, fHalfHeight });
	SetWallPlane(3, XMFLOAT4{ 0.0f, -1.0f, 0.0f, fHalfHeight });
	SetWallPlane(4, XMFLOAT4{ 0.0f, 0.0f, +1.0f, fHalfDepth });
	SetWallPlane(5, XMFLOAT4{ 0.0f, 0.0f, -1.0f, fHalfDepth });

	SetOBB(BoundingOrientedBox{ XMFLOAT3{0.f, 0.f, 0.f}, XMFLOAT3{fHalfWidth, fHalfHeight, fHalfDepth * 0.05f}, XMFLOAT4{0.f, 0.f, 0.f, 1.f} });
}

void WallsObject::Update(float fTimeElapsed)
{
	m_pTransform->Update();
}

void WallsObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Camera> pCamera)
{
	GameObject::Render(pd3dCommandList, pCamera);
}
