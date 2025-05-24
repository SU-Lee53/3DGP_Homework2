#include "stdafx.h"
#include "FirstPersonPlayer.h"

using namespace std;

FirstPersonPlayer::FirstPersonPlayer()
{
}

FirstPersonPlayer::~FirstPersonPlayer()
{
}

void FirstPersonPlayer::Initialize()
{
	m_pCamera = make_shared<FirstPersonCamera>();
	m_pCamera->Initialize(static_pointer_cast<Player>(shared_from_this()));
	m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	m_pCamera->SetFOVAngle(60.0f);
	m_pCamera->SetNearZ(1.01f);
	m_pCamera->SetFarZ(500.0f);
}

void FirstPersonPlayer::Update(float fTimeElapsed)
{
	Player::Update(fTimeElapsed);
}

void FirstPersonPlayer::ProcessKeyboardInput(float fTimeElapsed)
{
	if (INPUT.GetButtonPressed('W')) {
		Player::Move(m_pTransform->GetLook(), 13.5f * fTimeElapsed);
	}

	if (INPUT.GetButtonPressed('S')) {
		Player::Move(m_pTransform->GetLook(), -13.5f * fTimeElapsed);
	}

	if (INPUT.GetButtonPressed('D')) {
		Player::Move(m_pTransform->GetRight(), 13.5f * fTimeElapsed);
	}

	if (INPUT.GetButtonPressed('A')) {
		Player::Move(m_pTransform->GetRight(), -13.5f * fTimeElapsed);
	}
	
	if (INPUT.GetButtonPressed('E')) {
		Player::Move(m_pTransform->GetUp(), 13.5f * fTimeElapsed);
	}
	
	if (INPUT.GetButtonPressed('Q')) {
		Player::Move(m_pTransform->GetUp(), -13.5f * fTimeElapsed);
	}

}

void FirstPersonPlayer::ProcessMouseInput(float fTimeElapsed)
{
	if (INPUT.GetButtonPressed(VK_LBUTTON)) {
		HWND hWnd = ::GetActiveWindow();

		RECT rtClientRect;
		::GetClientRect(hWnd, &rtClientRect);
		::ClientToScreen(hWnd, (LPPOINT)&rtClientRect.left);
		::ClientToScreen(hWnd, (LPPOINT)&rtClientRect.right);

		int nScreenCenterX = 0, nScreenCenterY = 0;
		nScreenCenterX = rtClientRect.left + FRAME_BUFFER_WIDTH / 2;
		nScreenCenterY = rtClientRect.top + FRAME_BUFFER_HEIGHT / 2;

		POINT ptCursorPos;
		::GetCursorPos(&ptCursorPos);

		POINT ptDelta{ (ptCursorPos.x - nScreenCenterX), (ptCursorPos.y - nScreenCenterY) };

		Rotate(ptDelta.y * 0.10f, ptDelta.x * 0.10f, 0.f);

		::SetCursorPos(nScreenCenterX, nScreenCenterY);

		INPUT.HideCursor();
	}

	if (INPUT.GetButtonUp(VK_LBUTTON)) {
		INPUT.ShowCursor();
	}

}

void FirstPersonPlayer::Rotate(float fPitch, float fYaw, float fRoll)
{
	m_pTransform->AddRotationEuler(fPitch, fYaw, fRoll);
	m_pCamera->Rotate(fPitch, fYaw, fRoll);
}

void FirstPersonPlayer::UpdatePlayerCamera(float fTimeElapsed)
{
	m_pCamera->SetPosition(m_pTransform->GetPosition());
	m_pCamera->Update(fTimeElapsed);
}
