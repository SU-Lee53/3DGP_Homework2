#pragma once
#include "Player.h"
#include "FirstPersonCamera.h"

class FirstPersonPlayer : public Player {
public:
	FirstPersonPlayer();
	virtual ~FirstPersonPlayer();

public:
	virtual void Initialize() override;
	virtual void Update(float fTimeElapsed) override;

public:
	virtual void ProcessKeyboardInput(float fTimeElapsed) override;
	virtual void ProcessMouseInput(float fTimeElapsed) override;

public:
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) override;

private:
	virtual void UpdatePlayerCamera(float fTimeElapsed) override;


};

