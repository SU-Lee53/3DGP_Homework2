#include "stdafx.h"
#include "Player.h"
#include "Camera.h"

Player::Player()
{
	m_eObjectType = TAG_GAMEOBJECT_TYPE_PLAYER;
}

Player::~Player()
{
}

void Player::Move(const XMFLOAT3& xmf3Direction, float fDistance)
{
	XMFLOAT3 xmf3Shift = XMFLOAT3{ 0.f, 0.f, 0.f };
	xmf3Shift = Vector3::Add(xmf3Shift, xmf3Direction, fDistance);
	Move(xmf3Shift, TRUE);
}

void Player::Move(const XMFLOAT3& xmf3Shift, BOOL bUpdateVelocity)
{
	if (bUpdateVelocity) {
		m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
	}
	else {
		m_pTransform->SetPosition(Vector3::Add(xmf3Shift, m_pTransform->GetPosition()));
	}
}

void Player::Move(float x, float y, float z)
{
	Move(XMFLOAT3(x, y, z), FALSE);
}

void Player::Rotate(float fPitch, float fYaw, float fRoll)
{
}

void Player::SetCameraOffset(const XMFLOAT3& xmf3CameraOffset)
{
	m_xmf3CameraOffset = xmf3CameraOffset;
	m_pCamera->SetLookAt(Vector3::Add(m_pTransform->GetPosition(), m_xmf3CameraOffset), m_pTransform->GetPosition(), m_pTransform->GetUp());
}

void Player::Initialize()
{
}

void Player::Update(float fElapsedTime)
{
	Move(m_xmf3Velocity, FALSE);
	UpdatePlayerCamera(fElapsedTime);

	XMFLOAT3 xmf3Deceleration = Vector3::Normalize(Vector3::ScalarProduct(m_xmf3Velocity, -1.0f));
	float fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = m_fFriction * fElapsedTime;
	if (fDeceleration > fLength) fDeceleration = fLength;
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Deceleration, fDeceleration);

	GameObject::Update(fElapsedTime);
}

void Player::Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera)
{
	GameObject::Render(hDCFrameBuffer, pCamera);
}

void Player::UpdatePlayerCamera(float fTimeElapsed)
{
	m_pCamera->Update(fTimeElapsed);
}
