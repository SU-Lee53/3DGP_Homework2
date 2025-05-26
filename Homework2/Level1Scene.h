#pragma once
#include "Scene.h"

class Level1Scene : public Scene {
public:
	Level1Scene() {
		m_eSceneTag = TAG_SCENE_LEVEL1;
	}

	virtual ~Level1Scene() {}

public:
	virtual void BuildObjects(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;
	virtual void ReleaseObjects() override;

	virtual void Update(float fTimeElapsed) override;
	virtual void Render() override;

public:
	virtual void ProcessMouseInput(float fTimeElapsed) override;
	virtual void ProcessKeyboardInput(float fTimeElapsed) override;

public:
	void UpdatePlayerRide(float fTimeElapsed);
	void ResetPlayerRide();

private:
	BOOL m_bPlayerRide = FALSE;
	BOOL m_bRollercoasterEnd = FALSE;

	std::vector<XMFLOAT3>	m_xmf3MoveRoutes = {};
	int						m_nCurrentRoute = 1;
	float					m_fInterpolationFactor = 0.f;
};

