#include "stdafx.h"
#include "TankPlayer.h"
#include "ThirdPersonCamera.h"
#include "BulletObject.h"
#include "TankObject.h"
#include "WallsObject.h"
#include "ObstacleObject.h"

using namespace std;

TankPlayer::TankPlayer()
{
}

TankPlayer::~TankPlayer()
{
}

void TankPlayer::Initialize(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	m_pCamera = make_shared<ThirdPersonCamera>();
	m_pCamera->Initialize(static_pointer_cast<Player>(shared_from_this()));
	m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	m_pCamera->SetFOVAngle(60.0f);
	m_pCamera->SetNearZ(1.01f);
	m_pCamera->SetFarZ(500.0f);
	SetCameraOffset(XMFLOAT3{ 0.f, 3.0f, -10.0f });
	m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);


	shared_ptr<Mesh<DiffusedVertex>> pTankMesh = make_shared<Mesh<DiffusedVertex>>();
	MeshHelper::CreateMeshFromOBJFiles(pd3dDevice, pd3dCommandList, pTankMesh, L"../Resources/Tank.obj", XMFLOAT4{0.f, 1.f, 0.f, 1.f});
	SetMeshDefaultOrientation(XMFLOAT3{ -90.f, 180.f, 0.f });
	SetMesh(pTankMesh);
	SetColor(RGB(255, 0, 0));
	SetShader(SHADER.GetShader(TAG_SHADER_DIFFUSED));
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	shared_ptr<Mesh<DiffusedVertex>> pBulletMesh = make_shared<Mesh<DiffusedVertex>>();
	XMFLOAT4 xmf4BulletColor;
	XMStoreFloat4(&xmf4BulletColor, DirectX::Colors::Orange);
	MeshHelper::CreateCubeMesh(pd3dDevice, pd3dCommandList, pBulletMesh, 0.5f, 0.5f, 1.f, xmf4BulletColor);
	std::generate_n(m_pBullets.begin(), BULLET_COUNT, [this, &pBulletMesh, &pd3dDevice, &pd3dCommandList]()->std::shared_ptr<BulletObject> {
		shared_ptr<BulletObject> pBullet = make_shared<BulletObject>(m_fBulletEffectiveRange);
		pBullet->SetMesh(pBulletMesh);
		pBullet->SetRotationAxis(XMFLOAT3{ 0.f, 1.f, 0.f });
		pBullet->SetRotationSpeed(360.f);
		pBullet->SetMovingSpeed(120.0f);
		pBullet->SetActive(FALSE);
		pBullet->SetShader(SHADER.GetShader(TAG_SHADER_DIFFUSED));
		pBullet->CreateShaderVariables(pd3dDevice, pd3dCommandList);
		return pBullet;
	});

	XMFLOAT3 xmf3ShieldSize;
	XMStoreFloat3(&xmf3ShieldSize, XMVectorScale(XMLoadFloat3(&m_xmOBB.Extents), 2.0));
	float fShieldSize = std::max(std::max(xmf3ShieldSize.x, xmf3ShieldSize.y), xmf3ShieldSize.z);
	m_pShieldMesh = make_shared<Mesh<DiffusedVertex>>();
	MeshHelper::CreateCubeMesh(pd3dDevice, pd3dCommandList, m_pShieldMesh, fShieldSize, fShieldSize, fShieldSize);

	m_pShieldObject = make_shared<ShieldObject>();
	m_pShieldObject->SetOwner(static_pointer_cast<Player>(shared_from_this()));
	m_pShieldObject->Initialize(pd3dDevice, pd3dCommandList);
	m_pShieldObject->SetShader(SHADER.GetShader(TAG_SHADER_WIREFRAME));
	m_pShieldObject->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	UpdateBoundingBox();
}

void TankPlayer::Update(float fTimeElapsed)
{
	if (GameFramework::m_pCurrentScene->HasFloor()) {
		AdjustHeightToFloor(GameFramework::m_pCurrentScene->GetFloorHeight());
	}

	Player::Update(fTimeElapsed);
	std::for_each(m_pBullets.begin(), m_pBullets.end(), [&fTimeElapsed](std::shared_ptr<BulletObject>& p) {
		if (p->IsActive()) p->Update(fTimeElapsed);
	});

	m_pShieldObject->Update(fTimeElapsed);

}

void TankPlayer::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Camera> pCamera)
{
	if (pCamera->IsInFrustum(m_xmOBB)) {
		GameObject::Render(pd3dCommandList, pCamera);

		if (m_bShieldOn) {
			m_pShieldObject->Render(pd3dCommandList, pCamera);
		}
	}

	for (const auto& pBullet : m_pBullets) {
		if (pBullet->IsActive())
			pBullet->Render(pd3dCommandList, pCamera);
	}

}

void TankPlayer::ProcessKeyboardInput(float fTimeElapsed)
{
	if (INPUT.GetButtonPressed(VK_UP)) {
		Player::Move(m_pTransform->GetLook(), m_fSpeed * fTimeElapsed);
	}

	if (INPUT.GetButtonPressed(VK_DOWN)) {
		Player::Move(m_pTransform->GetLook(), -m_fSpeed * fTimeElapsed);
	}

	if (INPUT.GetButtonPressed(VK_RIGHT)) {
		Player::Move(m_pTransform->GetRight(), m_fSpeed * fTimeElapsed);
	}

	if (INPUT.GetButtonPressed(VK_LEFT)) {
		Player::Move(m_pTransform->GetRight(), -m_fSpeed * fTimeElapsed);
	}

	if (INPUT.GetButtonDown('S')) {
		m_bShieldOn = !m_bShieldOn;
	}

	if (INPUT.GetButtonDown('A')) {
		m_bAutoFire = !m_bAutoFire;
	}
}

void TankPlayer::ProcessMouseInput(float fTimeElapsed)
{
	if (INPUT.GetButtonPressed(VK_LBUTTON)) {
		HWND hWnd = ::GetActiveWindow();

		::SetCursor(NULL);

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
	}
}

void TankPlayer::Rotate(float fPitch, float fYaw, float fRoll)
{
	m_pTransform->AddRotationEuler(0.f, fYaw, 0.f);
}

void TankPlayer::BeginCollision(std::shared_ptr<GameObject> pOther)
{
	if (auto p = dynamic_pointer_cast<TankObject>(pOther)) {
		if (!p->IsBlowingUp()) {
			m_pTransform->InvalidateMovement();
		}
	}
	else if (auto p = dynamic_pointer_cast<BulletObject>(pOther)) {

	}
	else if (auto p = dynamic_pointer_cast<WallsObject>(pOther)) {
		m_pTransform->SetPosition(0.f, 0.f, 0.f);
	}
	else if (auto p = dynamic_pointer_cast<ObstacleObject>(pOther)) {
		m_pTransform->InvalidateMovement();
	}
}

void TankPlayer::InCollision(std::shared_ptr<GameObject> pOther)
{
	// 탱크 vs 플레이어 끼리 충돌간에 끼이는거만 좀 수정하자
	if (auto p = dynamic_pointer_cast<TankObject>(pOther)) {
		if (!p->IsBlowingUp()) {
			m_pTransform->InvalidateMovement();
		}
	}
	else if (auto p = dynamic_pointer_cast<ObstacleObject>(pOther)) {
		m_pTransform->InvalidateMovement();
	}
}

void TankPlayer::FireBullet(std::shared_ptr<GameObject> pLockedObject)
{
	std::shared_ptr<BulletObject> pBulletObject = nullptr;

	auto it = std::find_if(m_pBullets.begin(), m_pBullets.end(), [](std::shared_ptr<BulletObject> p) { return !(p->IsActive()); });
	if (it != m_pBullets.end()) {
		pBulletObject = *it;
	}

	if (pBulletObject) {
		XMFLOAT3 xmf3Position = m_pTransform->GetPosition();
		XMFLOAT3 xmf3Direction = m_pTransform->GetLook();
		XMFLOAT3 xmf3FirePosition = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Direction, 6.0f, FALSE));

		pBulletObject->GetTransform()->SetPosition(m_pTransform->GetPosition());
		pBulletObject->GetTransform()->SetRotationEuler(m_pTransform->GetRotationEuler());
		pBulletObject->GetTransform()->SetRotationAxisAngle(m_pTransform->GetRotationAxisAngle());

		pBulletObject->SetFirePosition(xmf3FirePosition);
		pBulletObject->SetMovingDirection(xmf3Direction);
		pBulletObject->SetColor(RGB(255, 0, 0));
		pBulletObject->SetActive(TRUE);

		if (pLockedObject && m_bAutoFire) {
			pBulletObject->SetLockedObject(pLockedObject);
			pBulletObject->SetColor(RGB(0, 0, 255));
		}
	}
}

void TankPlayer::UpdatePlayerCamera(float fTimeElapsed)
{
	m_pCamera->Update(fTimeElapsed);
}
