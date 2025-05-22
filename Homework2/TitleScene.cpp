#include "stdafx.h"
#include "TitleScene.h"
#include "FirstPersonPlayer.h"
#include "ExplosiveObject.h"

using namespace std;

void TitleScene::BuildObjects()
{
	m_bSceneChanged = FALSE;

	shared_ptr<Mesh> p3DGPMesh = make_shared<Mesh>();
	MeshHelper::CreateMeshFromOBJFiles(p3DGPMesh, L"../Resources/3D_Game_Programming.obj");

	shared_ptr<Mesh> pNameMesh = make_shared<Mesh>();
	MeshHelper::CreateMeshFromOBJFiles(pNameMesh, L"../Resources/name.obj");

	m_pObjects.resize(2);
	m_pObjects[0] = make_shared<GameObject>();
	m_pObjects[0]->SetColor(RGB(0, 255, 0));
	m_pObjects[0]->SetMesh(p3DGPMesh);
	m_pObjects[0]->GetTransform()->SetPosition(0.f, 10.f, 70.f);
	m_pObjects[0]->SetMeshDefaultOrientation(XMFLOAT3{ 90.f, 0.f, 0.f });

	m_pObjects[1] = make_shared<ExplosiveObject>();
	m_pObjects[1]->SetColor(RGB(255, 0, 0));
	m_pObjects[1]->SetMesh(pNameMesh);
	m_pObjects[1]->GetTransform()->SetPosition(25.f, -10.f, 70.f);
	m_pObjects[1]->SetMeshDefaultOrientation(XMFLOAT3{ 90.f, 0.f, 0.f });
	static_pointer_cast<ExplosiveObject>(m_pObjects[1])->SetAutoReset(FALSE);
	
	m_pPlayer = make_shared<FirstPersonPlayer>();
	m_pPlayer->Initialize();
	m_pPlayer->GetTransform()->SetPosition(0.f, 0.f, 0.f);

	ExplosiveObject::PrepareExplosion();
}

void TitleScene::ReleaseObjects()
{
	Scene::ReleaseObjects();
}

void TitleScene::Update(float fTimeElapsed)
{
	if (static_pointer_cast<ExplosiveObject>(m_pObjects[1])->IsExploded()) {
		GameFramework::ChangeScene(TAG_SCENE_MENU);
		return;
	}

	m_fRunningTime += fTimeElapsed;
	m_pObjects[1]->GetTransform()->AddRotationEuler(0.f, XMConvertToRadians(m_fRotationSpeed * fTimeElapsed), 0.0f);

	ProcessMouseInput(fTimeElapsed);
	ProcessKeyboardInput(fTimeElapsed);

	if (m_bSceneChanged) {
		return;
	}

	if (m_pPlayer)
		m_pPlayer->Update(fTimeElapsed);

	if (!m_pObjects.empty()) {
		std::for_each(m_pObjects.begin(), m_pObjects.end(), [fTimeElapsed](std::shared_ptr<GameObject>& p) { p->Update(fTimeElapsed); });
	}
}

void TitleScene::Render(HDC hDCFrameBuffer)
{
}

void TitleScene::ProcessMouseInput(float fTimeElapsed)
{
	if (INPUT.GetButtonDown(VK_LBUTTON)) {
		POINT ptCursorPos = INPUT.GetCurrentCursorPos();
		std::shared_ptr<GameObject> pPickedObj = PickObjectPointedByCursor(ptCursorPos.x, ptCursorPos.y, m_pPlayer->GetCamera());
		if (pPickedObj) {
			pPickedObj->OnPicked();
		}
	}
}

void TitleScene::ProcessKeyboardInput(float fTimeElapsed)
{
}
