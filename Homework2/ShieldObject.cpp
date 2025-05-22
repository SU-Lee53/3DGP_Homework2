#include "stdafx.h"
#include "ShieldObject.h"
#include "Player.h"

ShieldObject::ShieldObject()
{
}

ShieldObject::~ShieldObject()
{
}

void ShieldObject::Initialize()
{
	XMFLOAT3 xmf3ShieldSize;
	XMStoreFloat3(&xmf3ShieldSize, XMVectorScale(XMLoadFloat3(&m_wpOwner.lock()->GetOBB().Extents), 2.0));
	float fShieldSize = std::max(std::max(xmf3ShieldSize.x, xmf3ShieldSize.y), xmf3ShieldSize.z);

	std::shared_ptr<Mesh> pShieldMesh = std::make_shared<Mesh>();
	MeshHelper::CreateCubeMesh(pShieldMesh, fShieldSize, fShieldSize, fShieldSize);
	SetMesh(pShieldMesh);

	SetColor(RGB(0, 0, 255));
}

void ShieldObject::Update(float fTimeElapsed)
{
	m_pTransform->SetPosition(m_wpOwner.lock()->GetTransform()->GetPosition());
	m_pTransform->SetRotationEuler(m_wpOwner.lock()->GetTransform()->GetRotationEuler());

	GameObject::Update(fTimeElapsed);
}

void ShieldObject::Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera)
{
	GameObject::Render(hDCFrameBuffer, pCamera);
}
