#pragma once
#include "Scene.h"

class MenuScene : public Scene {
public:
	MenuScene() {
		m_eSceneTag = TAG_SCENE_TITLE;
	}

	virtual ~MenuScene() {}

public:
	virtual void BuildObjects(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;
	virtual void ReleaseObjects() override;

	virtual void Update(float fTimeElapsed) override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;

public:
	virtual void ProcessMouseInput(float fTimeElapsed) override;
	virtual void ProcessKeyboardInput(float fTimeElapsed) override;


};

