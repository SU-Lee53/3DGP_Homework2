#pragma once
#include "ExplosiveObject.h"


class TankObject : public ExplosiveObject {
public:
	TankObject();
	~TankObject();

public:
	void SetMovingDirection(const XMFLOAT3& xmf3MovingDirection);
	XMFLOAT3 GetMovingDirection() { return m_xmf3MovingDirection; }
	
	void SetMovingSpeed(float xmf3MovingDirection) { m_fMovingSpeed = m_fMovingSpeed; }
	float GetMovingSpeed() { return m_fMovingSpeed; }

	void SetCollisionUpdated(BOOL bUpdated) { m_bCollisionUpdated = bUpdated; }

public:
	virtual void Initialize(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;
	virtual void Update(float fElapsedTime) override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<class Camera> pCamera) override;

public:
	virtual void BeginCollision(std::shared_ptr<GameObject> pOther) override;
	virtual void InCollision(std::shared_ptr<GameObject> pOther) override;

private:
	XMFLOAT3 GetReflectedMovingDirection(const XMFLOAT3& otherPosition);

private:
	void InitializeMovingDirection();

	void Move(const XMFLOAT3& xmf3Shift);

private:
	XMFLOAT4X4	m_xmf4x4World = Matrix4x4::Identity();

	XMFLOAT3	m_xmf3MovingDirection = {};
	float		m_fMovingSpeed = 0.f;


	BOOL m_bCollisionUpdated = FALSE;

};

