#include "stdafx.h"
#include "Level1Scene.h"
#include "FirstPersonPlayer.h"
#include "Mesh.h"

using namespace std;

void Level1Scene::BuildObjects()
{
	m_bSceneChanged = FALSE;

	std::vector<XMFLOAT3> RollercoasterRoutes = {};
	shared_ptr<Mesh> pRollercoasterMesh = make_shared<Mesh>();
	MeshHelper::CreateRollercoasterRailMesh(pRollercoasterMesh, m_xmf3MoveRoutes, 10.0f, 150.0f, 30, 5);

	shared_ptr<Mesh> pCubeMesh = make_shared<Mesh>();
	MeshHelper::CreateCubeMesh(pCubeMesh);
	
	shared_ptr<Mesh> pAxis = make_shared<AxisMesh>();

	m_pObjects.resize(2);
	m_pObjects[0] = make_shared<GameObject>();
	m_pObjects[0]->SetColor(RGB(0,0,0));
	m_pObjects[0]->SetMesh(pRollercoasterMesh);
	m_pObjects[0]->GetTransform()->SetPosition(0.f, 0.f, 0.f);

	m_pObjects[1] = make_shared<GameObject>();
	m_pObjects[1]->SetColor(RGB(255, 0, 0));
	m_pObjects[1]->SetMesh(pAxis);
	m_pObjects[1]->GetTransform()->SetPosition(0.f, 0.f, 0.0f);
	
	m_pPlayer = make_shared<FirstPersonPlayer>();
	m_pPlayer->Initialize();
	m_pPlayer->GetTransform()->SetPosition(0.f, 0.f, 0.f);
}

void Level1Scene::ReleaseObjects()
{
	Scene::ReleaseObjects();

	m_bPlayerRide = FALSE;
	m_bRollercoasterEnd = FALSE;
	
	m_xmf3MoveRoutes.clear();
	m_nCurrentRoute = 1;
	m_fInterpolationFactor = 0.f;

}

void Level1Scene::Update(float fTimeElapsed)
{
	ProcessMouseInput(fTimeElapsed);
	ProcessKeyboardInput(fTimeElapsed);

	if (m_bSceneChanged) {
		return;
	}

	if (m_bPlayerRide) UpdatePlayerRide(fTimeElapsed);
	if (m_bRollercoasterEnd) {
		GameFramework::ChangeScene(TAG_SCENE_LEVEL2);
		m_bSceneChanged = TRUE;
		return;
	}

	if(m_pPlayer)
		m_pPlayer->Update(fTimeElapsed);
	
	if (!m_pObjects.empty()) {
		std::for_each(m_pObjects.begin(), m_pObjects.end(), [fTimeElapsed](std::shared_ptr<GameObject>& p) { p->Update(fTimeElapsed); });
	}

}

void Level1Scene::Render(HDC hDCFrameBuffer)
{
}

void Level1Scene::ProcessMouseInput(float fTimeElapsed)
{
	m_pPlayer->ProcessMouseInput(fTimeElapsed);
}

void Level1Scene::ProcessKeyboardInput(float fTimeElapsed)
{
	if (INPUT.GetButtonDown(VK_SPACE)) {
		m_bPlayerRide = !m_bPlayerRide;
		if (!m_bPlayerRide) ResetPlayerRide();
	}
	
	if (INPUT.GetButtonDown('N')) {
		GameFramework::ChangeScene(TAG_SCENE_LEVEL2);
		m_bSceneChanged = TRUE;
		return;
	}

	if (!m_bPlayerRide) {
		m_pPlayer->ProcessKeyboardInput(fTimeElapsed);
	}


	if (INPUT.GetButtonPressed(VK_UP)) {
		m_pObjects[1]->GetTransform()->AddRotationEuler(-1.5f, 0.f, 0.f);
	}

	if (INPUT.GetButtonPressed(VK_DOWN)) {
		m_pObjects[1]->GetTransform()->AddRotationEuler(1.5f, 0.f, 0.f);
	}

	if (INPUT.GetButtonPressed(VK_RIGHT)) {
		m_pObjects[1]->GetTransform()->AddRotationEuler(0.0f, 1.5f, 0.f);
	}

	if (INPUT.GetButtonPressed(VK_LEFT)) {
		m_pObjects[1]->GetTransform()->AddRotationEuler(0.0f, -1.5f, 0.f);
	}


	if (INPUT.GetButtonDown(VK_ESCAPE)) {
		GameFramework::ChangeScene(TAG_SCENE_MENU);
		return;
	}
}

void Level1Scene::UpdatePlayerRide(float fTimeElapsed)
{
	if (!m_bPlayerRide) return;

	if (m_nCurrentRoute < m_xmf3MoveRoutes.size() - 2) {
		if (m_fInterpolationFactor <= 1.0f) {
			XMVECTOR xmvPos1 = XMLoadFloat3(&m_xmf3MoveRoutes[m_nCurrentRoute]);
			XMVECTOR xmvPos2 = XMLoadFloat3(&m_xmf3MoveRoutes[m_nCurrentRoute + 1]);

			XMVECTOR xmvCurrentPos = XMVectorLerp(xmvPos1, xmvPos2, m_fInterpolationFactor);
			m_fInterpolationFactor += 5.0f * fTimeElapsed;

			XMFLOAT3 xmf3CurrentPos;
			XMStoreFloat3(&xmf3CurrentPos, xmvCurrentPos);

			m_pPlayer->GetTransform()->SetPosition(Vector3::Add(xmf3CurrentPos, XMFLOAT3{ 0.f, 15.f, 0.f }));
		}
		else {
			m_fInterpolationFactor = 0.f;
			m_nCurrentRoute++;
		}
	}
	else {
		m_bRollercoasterEnd = TRUE;
	}
}

void Level1Scene::ResetPlayerRide()
{
	m_nCurrentRoute = 1;
	m_fInterpolationFactor = 0.f;
	if (auto p = dynamic_pointer_cast<FirstPersonPlayer>(m_pPlayer)) {
		p->GetTransform()->SetPosition(0.f, 0.f, 0.f);
		p->GetTransform()->SetRotationEuler(0.f, 0.f, 0.f);
		static_pointer_cast<FirstPersonCamera>(p->GetCamera())->SetRotation(0, 0, 0);
	}
}
