#pragma once
#include "Scene.h"

class MenuScene : public Scene {
public:
	MenuScene() {
		m_eSceneTag = TAG_SCENE_TITLE;
	}

	virtual ~MenuScene() {}

public:
	virtual void BuildObjects() override;
	virtual void ReleaseObjects() override;

	virtual void Update(float fTimeElapsed) override;
	virtual void Render(HDC hDCFrameBuffer) override;

public:
	virtual void ProcessMouseInput(float fTimeElapsed) override;
	virtual void ProcessKeyboardInput(float fTimeElapsed) override;


};

