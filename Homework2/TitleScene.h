#pragma once
#include "Scene.h"

class TitleScene : public Scene {
public:
	TitleScene() {
		m_eSceneTag = TAG_SCENE_TITLE;
	}

	virtual ~TitleScene() {}

public:
	virtual void BuildObjects() override;
	virtual void ReleaseObjects() override;

	virtual void Update(float fTimeElapsed) override;
	virtual void Render(HDC hDCFrameBuffer) override;

public:
	virtual void ProcessMouseInput(float fTimeElapsed) override;
	virtual void ProcessKeyboardInput(float fTimeElapsed) override;


public:
	float m_fRunningTime = 0.f;
	float m_fRotationSpeed = 1000.f;

};

