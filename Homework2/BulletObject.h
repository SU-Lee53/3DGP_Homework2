#pragma once
#include "GameObject.h"

class BulletObject : public GameObject {
public:
	BulletObject(float fEffectiveRange) : m_fBulletEffectiveRange{ fEffectiveRange } {}
	virtual ~BulletObject() {}

public:
	void SetRotationAxis(const XMFLOAT3& xmf3RotationAxis) { m_xmf3RotationAxis = xmf3RotationAxis; }
	void SetMovingDirection(const XMFLOAT3& xmf3MovingDirection) { m_xmf3MovingDirection = xmf3MovingDirection; }
	void SetRotationSpeed(float fSpeed) { m_fRotationSpeed = fSpeed; }
	void SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }

	void SetFirePosition(XMFLOAT3 xmf3FirePosition) {
		m_xmf3FirePosition = xmf3FirePosition;
		m_pTransform->SetPosition(xmf3FirePosition);
	}

	void SetLockedObject(std::shared_ptr<GameObject> pLockedObject) { m_pLockedObject = pLockedObject; }
	BOOL IsActive() const { return m_bActive; }

	void Reset() {
		m_pLockedObject.reset();
		m_pLockedObject = nullptr;
		m_fElapsedTimeAfterFire = 0;
		m_fMovingDistance = 0;
		m_fRotationAngle = 0.0f;

		m_bActive = FALSE;
	}

	virtual void Update(float fElapsedTime) {

		m_fElapsedTimeAfterFire += fElapsedTime;

		float fDistance = m_fMovingSpeed * fElapsedTime;


		if ((m_fElapsedTimeAfterFire > m_fLockingDelayTime) && m_pLockedObject) {
			XMFLOAT3 xmf3Position = m_pTransform->GetPosition();
			XMVECTOR xmvPosition = XMLoadFloat3(&xmf3Position);

			XMFLOAT3 xmf3LockedObjectPosition = m_pLockedObject->GetTransform()->GetPosition();
			XMVECTOR xmvLockedObjectPosition = XMLoadFloat3(&xmf3LockedObjectPosition);
			XMVECTOR xmvToLockedObject = XMVectorSubtract(xmvLockedObjectPosition, xmvPosition);
			xmvToLockedObject = XMVector3Normalize(xmvToLockedObject);

			XMVECTOR xmvMovingDirection = XMLoadFloat3(&m_xmf3MovingDirection);
			xmvMovingDirection = XMVector3Normalize(XMVectorLerp(xmvMovingDirection, xmvToLockedObject, 0.5f));
			XMStoreFloat3(&m_xmf3MovingDirection, xmvMovingDirection);
		}

		XMFLOAT3 xmf3Position = m_pTransform->GetPosition();

		m_fRotationAngle += m_fRotationSpeed * fElapsedTime;
		if (m_fRotationAngle > 360.0f) m_fRotationAngle = m_fRotationAngle - 360.0f;

		m_pTransform->SetRotationEuler(0.0f, XMConvertToDegrees(m_fRotationAngle), 0.0f);

		XMFLOAT3 xmf3RotationAxis = Vector3::CrossProduct(m_xmf3RotationAxis, m_xmf3MovingDirection, TRUE);
		float fDotProduct = Vector3::DotProduct(m_xmf3RotationAxis, m_xmf3MovingDirection);
		float fRotationAngle = ::IsEqual(fDotProduct, 1.0f) ? 0.0f : (float)XMConvertToDegrees(acos(fDotProduct));
		m_pTransform->SetRotationAxisAngle(xmf3RotationAxis, XMConvertToDegrees(fRotationAngle));

		XMFLOAT3 xmf3Movement = Vector3::ScalarProduct(m_xmf3MovingDirection, fDistance, FALSE);
		xmf3Position = Vector3::Add(xmf3Position, xmf3Movement);
		m_pTransform->SetPosition(xmf3Position);

		GameObject::Update(fElapsedTime);

		if ((m_fMovingDistance > m_fBulletEffectiveRange) || (m_fElapsedTimeAfterFire > m_fLockingTime)) Reset();

	}

public:
	void BeginCollision(std::shared_ptr<GameObject> pOther);

private:
	float		m_fBulletEffectiveRange = 50.0f;
	float		m_fMovingDistance = 0.0f;
	float		m_fRotationAngle = 0.0f;
	XMFLOAT3	m_xmf3FirePosition = XMFLOAT3{ 0.f, 0.f, 1.f };

	float						m_fElapsedTimeAfterFire = 0.0f;
	float						m_fLockingDelayTime = 0.3f;
	float						m_fLockingTime = 4.0f;
	std::shared_ptr<GameObject> m_pLockedObject = nullptr;

	float m_fMovingSpeed = 0.f;
	float m_fRotationSpeed = 0.f;
	XMFLOAT3 m_xmf3RotationAxis = {};
	XMFLOAT3 m_xmf3MovingDirection = {};

};

