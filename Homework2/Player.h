#pragma once
#include "GameObject.h"

class Camera;

class Player : public GameObject {
public:
	Player();
	virtual ~Player();

public:
	void SetCameraOffset(const XMFLOAT3& xmf3CameraOffset);
	XMFLOAT3& GetCameraOffset() { return m_xmf3CameraOffset; }

	void Move(const XMFLOAT3& xmf3Direction, float fDistance);
	void Move(const XMFLOAT3& xmf3Shift, BOOL bUpdateVelocity);
	void Move(float x, float y, float z);

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);

	virtual void Initialize() override;
	virtual void Update(float fTimeElapsed) override;

	virtual void ProcessMouseInput(float fTimeElapsed) {};
	virtual void ProcessKeyboardInput(float fTimeElapsed) {};
	virtual void Render(std::shared_ptr<Camera> pCamera);

	void SetCamera(std::shared_ptr<Camera> pCamera) { m_pCamera = pCamera; }
	std::shared_ptr<Camera>& GetCamera() { return m_pCamera; }

protected:
	virtual void UpdatePlayerCamera(float fTimeElapsed);

protected:
	XMFLOAT3 m_xmf3CameraOffset = XMFLOAT3{ 0.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Velocity = XMFLOAT3{ 0.f ,0.f, 0.f };

	float m_fFriction = 125.0f;

	std::shared_ptr<Camera> m_pCamera = nullptr;

};

