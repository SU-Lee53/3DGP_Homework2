#include "stdafx.h"
#include "ShieldObject.h"
#include "Player.h"

ShieldObject::ShieldObject()
{
}

ShieldObject::~ShieldObject()
{
}

void ShieldObject::Initialize(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	XMFLOAT3 xmf3ShieldSize;
	XMStoreFloat3(&xmf3ShieldSize, XMVectorScale(XMLoadFloat3(&m_wpOwner.lock()->GetOBB().Extents), 2.0));
	float fShieldSize = std::max(std::max(xmf3ShieldSize.x, xmf3ShieldSize.y), xmf3ShieldSize.z);

	std::shared_ptr<Mesh<DiffusedVertex>> pShieldMesh = std::make_shared<Mesh<DiffusedVertex>>();
	MeshHelper::CreateCubeMesh(pd3dDevice, pd3dCommandList, pShieldMesh, fShieldSize, fShieldSize, fShieldSize, XMFLOAT4{ 0.f, 0.f, 1.f, 1.f });
	SetMesh(pShieldMesh);

	SetColor(XMFLOAT4(0.f ,0.f, 1.f, 1.f));
}

void ShieldObject::Update(float fTimeElapsed)
{
	m_pTransform->SetPosition(m_wpOwner.lock()->GetTransform()->GetPosition());
	m_pTransform->SetRotationEuler(m_wpOwner.lock()->GetTransform()->GetRotationEuler());

	GameObject::Update(fTimeElapsed);
}

void ShieldObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Camera> pCamera)
{
	GameObject::Render(pd3dCommandList, pCamera);
}
