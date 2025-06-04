#include "stdafx.h"
#include "TitleScene.h"
#include "FirstPersonPlayer.h"
#include "ExplosiveObject.h"

using namespace std;

void TitleScene::BuildObjects(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	m_bSceneChanged = FALSE;

	XMFLOAT4 xmf43DGPMeshColor = RandomGenerator::GenerateRandomColor();
	shared_ptr<Mesh<DiffusedVertex>> p3DGPMesh = make_shared<Mesh<DiffusedVertex>>();
	MeshHelper::CreateMeshFromOBJFiles(pd3dDevice, pd3dCommandList, p3DGPMesh, L"../Resources/3D_Game_Programming.obj", xmf43DGPMeshColor);

	XMFLOAT4 xmf4NameMeshColor = RandomGenerator::GenerateRandomColor();
	shared_ptr<Mesh<DiffusedVertex>> pNameMesh = make_shared<Mesh<DiffusedVertex>>();
	MeshHelper::CreateMeshFromOBJFiles(pd3dDevice, pd3dCommandList, pNameMesh, L"../Resources/name.obj", xmf4NameMeshColor);

	m_pObjects.resize(2);
	m_pObjects[0] = make_shared<GameObject>();
	m_pObjects[0]->SetColor(xmf43DGPMeshColor);
	m_pObjects[0]->SetMesh(p3DGPMesh);
	m_pObjects[0]->GetTransform()->SetPosition(0.f, 10.f, 70.f);
	m_pObjects[0]->SetMeshDefaultOrientation(XMFLOAT3{ 90.f, 0.f, 0.f });
	m_pObjects[0]->SetShader(SHADER.GetShader(TAG_SHADER_DIFFUSED));
	m_pObjects[0]->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pObjects[1] = make_shared<ExplosiveObject>();
	m_pObjects[1]->SetColor(xmf4NameMeshColor);
	m_pObjects[1]->SetMesh(pNameMesh);
	m_pObjects[1]->GetTransform()->SetPosition(25.f, -10.f, 70.f);
	m_pObjects[1]->SetMeshDefaultOrientation(XMFLOAT3{ 90.f, 0.f, 0.f });
	m_pObjects[1]->SetShader(SHADER.GetShader(TAG_SHADER_DIFFUSED));
	m_pObjects[1]->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	
	static_pointer_cast<ExplosiveObject>(m_pObjects[1])->SetAutoReset(FALSE);
	
	m_pPlayer = make_shared<FirstPersonPlayer>();
	m_pPlayer->Initialize(pd3dDevice, pd3dCommandList);
	m_pPlayer->GetTransform()->SetPosition(0.f, 0.f, 0.f);

	for (auto& pObj : m_pObjects) {
		pObj->Initialize(pd3dDevice, pd3dCommandList);
	}


	ExplosiveObject::PrepareExplosion(pd3dDevice, pd3dCommandList);
}

void TitleScene::ReleaseObjects()
{
	Scene::ReleaseObjects();
}

void TitleScene::Update(float fTimeElapsed)
{
	if (static_pointer_cast<ExplosiveObject>(m_pObjects[1])->IsExploded()) {
		GameFramework::SignalChangeScene(TAG_SCENE_MENU);
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
		for (auto& pObj : m_pObjects) {
			pObj->Update(fTimeElapsed);
		}
	}
}

void TitleScene::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	Scene::Render(pd3dCommandList);
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
