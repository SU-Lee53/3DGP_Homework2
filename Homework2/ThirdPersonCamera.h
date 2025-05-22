#pragma once
#include "Camera.h"


class ThirdPersonCamera : public Camera {
public:
	ThirdPersonCamera();
	~ThirdPersonCamera();

public:
	virtual BOOL Rotate(const XMFLOAT3& xmf3Rotation) override;
	virtual BOOL Rotate(const XMVECTOR& xmvRotation) override;
	virtual BOOL Rotate(float fPitch, float fYaw, float fRoll) override;

public:
	virtual void GenerateViewMatrix() override;

public:
	virtual void Initialize(std::shared_ptr<Player> pOwnerPlayer) override;
	virtual void Update(float fElapsedTime) override;


};

