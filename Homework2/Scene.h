#pragma once

#include "GameObject.h"
#include "Camera.h"

enum TAG_SCENE_NAME : UINT8 {
	TAG_SCENE_TITLE = 0,
	TAG_SCENE_MENU,
	TAG_SCENE_LEVEL1,
	TAG_SCENE_LEVEL2,

	TAG_SCENE_COUNT,

	TAG_SCENE_UNDEFINED = 99
};

class Scene {
public:
	Scene() { };
	virtual ~Scene() { }

public:
	TAG_SCENE_NAME GetSceneTag() { return m_eSceneTag; }

public:
	virtual void BuildObjects(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	virtual void ReleaseObjects();

	virtual void Update(float fElapsedTime);
	virtual void Render();
	
	virtual void ProcessMouseInput(float fTimeElapsed) { }
	virtual void ProcessKeyboardInput(float fTimeElapsed) { }

	virtual void CheckObjectByObjectCollisions();
	std::shared_ptr<GameObject> PickObjectPointedByCursor(int xClient, int yClient, std::shared_ptr<Camera> pCamera);

	std::shared_ptr<GameObject> FindObjectInScene(std::string_view svName);
	void UpdatePipelineVaribles(std::shared_ptr<class Camera> pCamera);


protected:
	std::vector<std::shared_ptr<GameObject>>	m_pObjects = {};
	std::shared_ptr<Player>						m_pPlayer;

	std::shared_ptr<GameObject>					m_pPickedObject = nullptr;

	TAG_SCENE_NAME								m_eSceneTag = TAG_SCENE_UNDEFINED;

	BOOL m_bSceneChanged = FALSE;

};
