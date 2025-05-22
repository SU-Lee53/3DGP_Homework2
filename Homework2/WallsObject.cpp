#include "stdafx.h"
#include "WallsObject.h"

WallsObject::WallsObject()
{
}

WallsObject::~WallsObject()
{
}

void WallsObject::Update(float fTimeElapsed)
{
	m_pTransform->Update();
}

void WallsObject::Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera)
{
	GameObject::Render(hDCFrameBuffer, m_pTransform->GetWorldMatrix(), m_pMesh);
}
