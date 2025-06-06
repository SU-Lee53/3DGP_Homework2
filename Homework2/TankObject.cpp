#include "stdafx.h"
#include "TankObject.h"
#include "TankPlayer.h"
#include "BulletObject.h"
#include "WallsObject.h"
#include "ObstacleObject.h"

using namespace std;

TankObject::TankObject()
{
}

TankObject::~TankObject()
{
}

void TankObject::SetMovingDirection(const XMFLOAT3& xmf3MovingDirection)
{
	m_xmf3MovingDirection = Vector3::Normalize(xmf3MovingDirection);

	// 이동방향과 바라보는 방향을 일치 -> LookAt 이용
	XMVECTOR xmvLook = XMVector3Normalize(XMLoadFloat3(&m_xmf3MovingDirection));
	XMVECTOR xmvUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMVECTOR xmvRight = XMVector3Normalize(XMVector3Cross(xmvUp, xmvLook));

	XMFLOAT3 xmf3Up;
	XMStoreFloat3(&xmf3Up, xmvUp);
	
	XMFLOAT3 xmf3Right;
	XMStoreFloat3(&xmf3Right, xmvRight);

	m_xmf4x4World._11 = xmf3Right.x; m_xmf4x4World._21 = xmf3Up.x; m_xmf4x4World._31 = m_xmf3MovingDirection.x;
	m_xmf4x4World._12 = xmf3Right.y; m_xmf4x4World._22 = xmf3Up.y; m_xmf4x4World._32 = m_xmf3MovingDirection.y;
	m_xmf4x4World._13 = xmf3Right.z; m_xmf4x4World._23 = xmf3Up.z; m_xmf4x4World._33 = m_xmf3MovingDirection.z;
}

void TankObject::InitializeMovingDirection()
{
	XMFLOAT3 xmf3InitialRotation{};
	xmf3InitialRotation.y = RandomGenerator::GenerateRandomFloatInRange(0.f, 360.f);

	float fPitch = XMConvertToRadians(xmf3InitialRotation.x);
	float fYaw = XMConvertToRadians(xmf3InitialRotation.y);
	float fRoll = XMConvertToRadians(xmf3InitialRotation.z);

	XMFLOAT4X4 xmf4x4Rotate;
	XMStoreFloat4x4(&xmf4x4Rotate, XMMatrixRotationRollPitchYaw(fPitch, fYaw, fRoll));
	SetMovingDirection(Vector3::Normalize(XMFLOAT3{ xmf4x4Rotate._31, xmf4x4Rotate._32, xmf4x4Rotate._33 }));
}

void TankObject::Move(const XMFLOAT3& xmf3Shift)
{
	// 위치는 Transform 에 등록해야함
	// 총알이 Transform 에서 위치를 찾아오기 때문
	m_pTransform->AddPosition(xmf3Shift.x, xmf3Shift.y, xmf3Shift.z);

	XMFLOAT3 xmf3CurPosition = m_pTransform->GetPosition();

	m_xmf4x4World._41 = xmf3CurPosition.x;
	m_xmf4x4World._42 = xmf3CurPosition.y;
	m_xmf4x4World._43 = xmf3CurPosition.z;

	if (GameFramework::m_pCurrentScene->HasFloor()) {
		AdjustHeightToFloor(GameFramework::m_pCurrentScene->GetFloorHeight());
	}

}

void TankObject::Initialize(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	ExplosiveObject::Initialize(pd3dDevice, pd3dCommandList);

	shared_ptr<Mesh<DiffusedVertex>> pTankMesh = make_shared<Mesh<DiffusedVertex>>();
	MeshHelper::CreateMeshFromOBJFiles(pd3dDevice, pd3dCommandList, pTankMesh, L"../Resources/Tank.obj", XMFLOAT4{1.f, 0.f, 0.f, 1.f});
	SetMeshDefaultOrientation(XMFLOAT3{ -90.f, 180.f, 0.f });
	SetMesh(pTankMesh);
	SetColor(RGB(255, 0, 0));

	XMFLOAT3 xmf3InitialPosition{};
	xmf3InitialPosition.x = RandomGenerator::GenerateRandomFloatInRange(-30.f, 30.f);
	xmf3InitialPosition.y = 0;
	xmf3InitialPosition.z = RandomGenerator::GenerateRandomFloatInRange(-30.f, 30.f);

	m_pTransform->SetPosition(xmf3InitialPosition);

	m_fMovingSpeed = RandomGenerator::GenerateRandomFloatInRange(10.f, 20.f);

	SetAutoReset(FALSE);

	InitializeMovingDirection();

	UpdateBoundingBox();

	if (GameFramework::m_pCurrentScene->HasFloor()) {
		AdjustHeightToFloor(GameFramework::m_pCurrentScene->GetFloorHeight());
	}

}

void TankObject::Update(float fElapsedTime)
{
	// 이동은 Transform 에 갱신중이므로 이전 프레임의 위치를 저장
	m_pTransform->CacheLastFrameMovement();

	if (m_fMovingSpeed != 0.0f && !m_bBlowingUp) {
		XMFLOAT3 xmf3Shift = Vector3::Add(XMFLOAT3{ 0,0,0 }, m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);
		Move(xmf3Shift);
	}

	m_pTransform->SetWorldMatrix(m_xmf4x4World);

	UpdateBoundingBox();

	ExplosiveObject::Update(fElapsedTime);
}

void TankObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<class Camera> pCamera)
{
	ExplosiveObject::Render(pd3dCommandList, pCamera);
}

XMFLOAT3 TankObject::GetReflectedMovingDirection(const XMFLOAT3& otherPosition)
{
	// 일단 반사벡터
	XMVECTOR xmvNormal = XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&m_pTransform->GetPosition()), XMLoadFloat3(&otherPosition)));
	XMVECTOR xmvReflected = XMVector3Reflect(XMLoadFloat3(&m_xmf3MovingDirection), xmvNormal);
	
	// 탱크가 날아다니지 않도록 이동 방향 벡터의 Y 성분을 0으로 맞춤
	xmvReflected = XMVector3Normalize(XMVectorSetY(xmvReflected, 0.f));
	
	XMFLOAT3 xmf3Ret;
	XMStoreFloat3(&xmf3Ret, xmvReflected);
	return xmf3Ret;
}

void TankObject::BeginCollision(std::shared_ptr<GameObject> pOther)
{
	if (auto p = dynamic_pointer_cast<TankPlayer>(pOther)) {
		if (p->IsShieldOn()) {
			m_bBlowingUp = TRUE;
		}
		else {
			SetMovingDirection(GetReflectedMovingDirection(p->GetTransform()->GetPosition()));
		}
	}
	else if (auto p = dynamic_pointer_cast<TankObject>(pOther)) {
		if (!m_bCollisionUpdated) {
			XMFLOAT3 xmf3MovingDirection = p->GetMovingDirection();
			float fMovingSpeed = p->GetMovingSpeed();

			m_pTransform->InvalidateMovement();

			p->SetMovingDirection(m_xmf3MovingDirection);
			p->SetMovingSpeed(m_fMovingSpeed);
			p->SetCollisionUpdated(TRUE);

			SetMovingDirection(xmf3MovingDirection);
			SetMovingSpeed(fMovingSpeed);
		}
		else {
			m_bCollisionUpdated = FALSE;
		}
	}
	else if (auto p = dynamic_pointer_cast<BulletObject>(pOther)) {
		m_bBlowingUp = TRUE;
	}
	else if (auto p = dynamic_pointer_cast<WallsObject>(pOther)) {
	}
	else if (auto p = dynamic_pointer_cast<ObstacleObject>(pOther)) {
		SetMovingDirection(GetReflectedMovingDirection(pOther->GetTransform()->GetPosition()));
	}
}

void TankObject::InCollision(std::shared_ptr<GameObject> pOther)
{
	if (auto p = dynamic_pointer_cast<TankPlayer>(pOther)) {
		//m_pTransform->InvalidateMovement();
	}
}

