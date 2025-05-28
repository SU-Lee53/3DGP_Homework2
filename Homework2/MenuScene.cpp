#include "stdafx.h"
#include "MenuScene.h"
#include "FirstPersonPlayer.h"
#include "ExplosiveObject.h"

using namespace std;

void MenuScene::BuildObjects(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	m_bSceneChanged = FALSE;

	shared_ptr<Mesh<DiffusedVertex>> pTutorialMesh = make_shared<Mesh<DiffusedVertex>>();
	MeshHelper::CreateMeshFromOBJFiles(pd3dDevice, pd3dCommandList, pTutorialMesh, L"../Resources/tutorial.obj", RandomGenerator::GenerateRandomColor());

	shared_ptr<Mesh<DiffusedVertex>> pLevel1Mesh = make_shared<Mesh<DiffusedVertex>>();
	MeshHelper::CreateMeshFromOBJFiles(pd3dDevice, pd3dCommandList, pLevel1Mesh, L"../Resources/Level-1.obj", RandomGenerator::GenerateRandomColor());
	
	shared_ptr<Mesh<DiffusedVertex>> pLevel2Mesh = make_shared<Mesh<DiffusedVertex>>();
	MeshHelper::CreateMeshFromOBJFiles(pd3dDevice, pd3dCommandList, pLevel2Mesh, L"../Resources/Level-2.obj", RandomGenerator::GenerateRandomColor());

	shared_ptr<Mesh<DiffusedVertex>> pStartMesh = make_shared<Mesh<DiffusedVertex>>();
	MeshHelper::CreateMeshFromOBJFiles(pd3dDevice, pd3dCommandList, pStartMesh, L"../Resources/Start.obj", RandomGenerator::GenerateRandomColor());
	
	shared_ptr<Mesh<DiffusedVertex>> pEndMesh = make_shared<Mesh<DiffusedVertex>>();
	MeshHelper::CreateMeshFromOBJFiles(pd3dDevice, pd3dCommandList, pEndMesh, L"../Resources/End.obj", RandomGenerator::GenerateRandomColor());

	m_pObjects.resize(5);
	m_pObjects[0] = make_shared<GameObject>();
	m_pObjects[0]->SetColor(RandomGenerator::GenerateRandomColor());
	m_pObjects[0]->SetMesh(pTutorialMesh);
	m_pObjects[0]->GetTransform()->SetPosition(0.f, 20.f, 70.f);
	m_pObjects[0]->SetMeshDefaultOrientation(XMFLOAT3{ 90.f, 0.f, 0.f });
	m_pObjects[0]->SetShader(SHADER.GetShader(TAG_SHADER_DIFFUSED));
	m_pObjects[0]->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pObjects[1] = make_shared<ExplosiveObject>();
	m_pObjects[1]->SetColor(RandomGenerator::GenerateRandomColor());
	m_pObjects[1]->SetMesh(pLevel1Mesh);
	m_pObjects[1]->GetTransform()->SetPosition(0.f, 10.f, 70.f);
	m_pObjects[1]->SetMeshDefaultOrientation(XMFLOAT3{ 90.f, 0.f, 0.f });
	static_pointer_cast<ExplosiveObject>(m_pObjects[1])->SetAutoReset(FALSE);
	m_pObjects[1]->SetName("Level1_Text");
	m_pObjects[1]->SetShader(SHADER.GetShader(TAG_SHADER_DIFFUSED));
	m_pObjects[1]->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	
	m_pObjects[2] = make_shared<ExplosiveObject>();
	m_pObjects[2]->SetColor(RandomGenerator::GenerateRandomColor());
	m_pObjects[2]->SetMesh(pLevel2Mesh);
	m_pObjects[2]->GetTransform()->SetPosition(0.f, 0.f, 70.f);
	m_pObjects[2]->SetMeshDefaultOrientation(XMFLOAT3{ 90.f, 0.f, 0.f });
	static_pointer_cast<ExplosiveObject>(m_pObjects[2])->SetAutoReset(FALSE);
	m_pObjects[2]->SetName("Level2_Text");
	m_pObjects[2]->SetShader(SHADER.GetShader(TAG_SHADER_DIFFUSED));
	m_pObjects[2]->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	
	m_pObjects[3] = make_shared<ExplosiveObject>();
	m_pObjects[3]->SetColor(RandomGenerator::GenerateRandomColor());
	m_pObjects[3]->SetMesh(pStartMesh);
	m_pObjects[3]->GetTransform()->SetPosition(-20.f, -15.f, 70.f);
	m_pObjects[3]->SetMeshDefaultOrientation(XMFLOAT3{ 90.f, 0.f, 0.f });
	m_pObjects[3]->SetName("Start_Text");
	static_pointer_cast<ExplosiveObject>(m_pObjects[3])->SetAutoReset(FALSE);
	m_pObjects[3]->SetShader(SHADER.GetShader(TAG_SHADER_DIFFUSED));
	m_pObjects[3]->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	
	m_pObjects[4] = make_shared<ExplosiveObject>();
	m_pObjects[4]->SetColor(RandomGenerator::GenerateRandomColor());
	m_pObjects[4]->SetMesh(pEndMesh);
	m_pObjects[4]->GetTransform()->SetPosition(20.f, -15.f, 70.f);
	m_pObjects[4]->SetMeshDefaultOrientation(XMFLOAT3{ 90.f, 0.f, 0.f });
	static_pointer_cast<ExplosiveObject>(m_pObjects[4])->SetAutoReset(FALSE);
	m_pObjects[4]->SetName("End_Text");
	m_pObjects[4]->SetShader(SHADER.GetShader(TAG_SHADER_DIFFUSED));
	m_pObjects[4]->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pPlayer = make_shared<FirstPersonPlayer>();
	m_pPlayer->Initialize(pd3dDevice, pd3dCommandList);
	m_pPlayer->GetTransform()->SetPosition(0.f, 0.f, 0.f);
	//m_pPlayer->SetShader(SHADER.GetShader(TAG_SHADER_DIFFUSED));
	//m_pPlayer->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	ExplosiveObject::PrepareExplosion(pd3dDevice, pd3dCommandList);
}

void MenuScene::ReleaseObjects()
{
	Scene::ReleaseObjects();
}

void MenuScene::Update(float fTimeElapsed)
{
	if (auto p = FindObjectInScene("Level1_Text")) {
		if (static_pointer_cast<ExplosiveObject>(p)->IsExploded()) {
			GameFramework::SignalChangeScene(TAG_SCENE_LEVEL1);
			m_bSceneChanged = TRUE;
			return;
		}
	}
	if (auto p = FindObjectInScene("Level2_Text")) {
		if (static_pointer_cast<ExplosiveObject>(p)->IsExploded()) {
			GameFramework::SignalChangeScene(TAG_SCENE_LEVEL2);
			m_bSceneChanged = TRUE;
			return;
		}
	}
	if (auto p = FindObjectInScene("Start_Text")) {
		if (static_pointer_cast<ExplosiveObject>(p)->IsExploded()) {
			GameFramework::SignalChangeScene(TAG_SCENE_LEVEL1);
			m_bSceneChanged = TRUE;
			return;
		}
	}
	if (auto p = FindObjectInScene("End_Text")) {
		if (static_pointer_cast<ExplosiveObject>(p)->IsExploded()) {
			PostQuitMessage(0);
		}
	}

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

void MenuScene::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	Scene::Render(pd3dCommandList);
}

void MenuScene::ProcessMouseInput(float fTimeElapsed)
{
	if (INPUT.GetButtonDown(VK_LBUTTON)) {
		POINT ptCursorPos = INPUT.GetCurrentCursorPos();
		std::shared_ptr<GameObject> pPickedObj = PickObjectPointedByCursor(ptCursorPos.x, ptCursorPos.y, m_pPlayer->GetCamera());
		if (pPickedObj) {
			pPickedObj->OnPicked();
			m_pPickedObject = pPickedObj;
		}
	}
}

void MenuScene::ProcessKeyboardInput(float fTimeElapsed)
{
}
