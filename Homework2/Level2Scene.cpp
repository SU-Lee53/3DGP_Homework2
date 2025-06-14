#include "stdafx.h"
#include "Level2Scene.h"
#include "TankPlayer.h"
#include "ExplosiveObject.h"
#include "BulletObject.h"
#include "WallsObject.h"
#include "TankObject.h"
#include "ObstacleObject.h"

using namespace std;

void Level2Scene::BuildObjects(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	m_bSceneChanged = FALSE;

	// WallsObject
	{
		float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 45.0f;
		shared_ptr<Mesh<DiffusedVertex>> pWallMesh = make_shared<Mesh<DiffusedVertex>>();
		MeshHelper::CreateWallMesh(pd3dDevice, pd3dCommandList, pWallMesh, fHalfWidth * 2.0f, fHalfHeight * 2.0f, fHalfDepth * 2.0f, 30);

		m_pWallsObject = make_shared<WallsObject>();
		m_pWallsObject->GetTransform()->SetPosition(0.0f, fHalfHeight, 0.0f);
		m_pWallsObject->SetMesh(pWallMesh);
		m_pWallsObject->SetColor(RGB(0, 0, 0));
		m_pWallsObject->SetWallPlane(0, XMFLOAT4{ +1.0f, 0.0f, 0.0f, fHalfWidth });
		m_pWallsObject->SetWallPlane(1, XMFLOAT4{ -1.0f, 0.0f, 0.0f, fHalfWidth });
		m_pWallsObject->SetWallPlane(2, XMFLOAT4{ 0.0f, +1.0f, 0.0f, fHalfHeight });
		m_pWallsObject->SetWallPlane(3, XMFLOAT4{ 0.0f, -1.0f, 0.0f, fHalfHeight });
		m_pWallsObject->SetWallPlane(4, XMFLOAT4{ 0.0f, 0.0f, +1.0f, fHalfDepth });
		m_pWallsObject->SetWallPlane(5, XMFLOAT4{ 0.0f, 0.0f, -1.0f, fHalfDepth });
		m_pWallsObject->SetOBB(BoundingOrientedBox{ XMFLOAT3{0.f, 0.f, 0.f}, XMFLOAT3{fHalfWidth, fHalfHeight, fHalfDepth * 0.05f}, XMFLOAT4{0.f, 0.f, 0.f, 1.f} });
		m_pWallsObject->SetShader(SHADER.GetShader(TAG_SHADER_WIREFRAME));
		m_pWallsObject->CreateShaderVariables(pd3dDevice, pd3dCommandList);

		m_bSceneHasFloor = TRUE;
		m_fFloorHeight = 0.f;
	}

	// Enemy Tank
	for (int i = 0; i < 10; ++i) {
		std::shared_ptr<TankObject> pTank = make_shared<TankObject>();
		pTank->SetColor(RGB(255, 0, 0));
		pTank->SetShader(SHADER.GetShader(TAG_SHADER_DIFFUSED));
		pTank->CreateShaderVariables(pd3dDevice, pd3dCommandList);
		m_pObjects.push_back(pTank);
	}

	// Obstacle
	for (int i = 0; i < 5; ++i) {
		std::shared_ptr<ObstacleObject> pObstacle = make_shared<ObstacleObject>();
		pObstacle->SetColor(RGB(255, 0, 0));
		pObstacle->SetShader(SHADER.GetShader(TAG_SHADER_DIFFUSED));
		pObstacle->CreateShaderVariables(pd3dDevice, pd3dCommandList);
		m_pObjects.push_back(pObstacle);
	}

	m_pPlayer = make_shared<TankPlayer>();
	m_pPlayer->Initialize(pd3dDevice, pd3dCommandList);
	m_pPlayer->SetShader(SHADER.GetShader(TAG_SHADER_DIFFUSED));
	m_pPlayer->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_pPlayer->GetTransform()->SetPosition(0.f, 0.f, 0.f);

	// You Win!
	shared_ptr<Mesh<DiffusedVertex>> pWinMesh = make_shared<Mesh<DiffusedVertex>>();
	MeshHelper::CreateMeshFromOBJFiles(pd3dDevice, pd3dCommandList, pWinMesh, L"../Resources/Win.obj", XMFLOAT4{ 0.f, 0.f, 1.f, 1.f });

	m_pWinTextObject = make_shared<GameObject>();
	m_pWinTextObject->SetColor(RGB(0, 0, 255));
	m_pWinTextObject->SetMesh(pWinMesh);
	m_pWinTextObject->GetTransform()->SetPosition(XMFLOAT3{ 0.f, 5.f, 0.f });
	m_pWinTextObject->SetMeshDefaultOrientation(XMFLOAT3{ 90.f, 0.f, 0.f });
	m_pWinTextObject->SetShader(SHADER.GetShader(TAG_SHADER_DIFFUSED));
	m_pWinTextObject->CreateShaderVariables(pd3dDevice, pd3dCommandList);


	for (auto& pObj : m_pObjects) {
		pObj->Initialize(pd3dDevice, pd3dCommandList);
	}
	m_pWinTextObject->Initialize(pd3dDevice, pd3dCommandList);

	ExplosiveObject::PrepareExplosion(pd3dDevice, pd3dCommandList);
}

void Level2Scene::ReleaseObjects()
{
	Scene::ReleaseObjects();

	m_pWallsObject.reset();
	m_pWinTextObject.reset();
	
	m_nTankDestroyed = 0;
	m_bGameEnded = FALSE;

}

void Level2Scene::Update(float fTimeElapsed)
{
	if (m_pPlayer)
		m_pPlayer->GetTransform()->CacheLastFrameMovement();

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
	
	m_pWallsObject->Update(fTimeElapsed);

	CheckPlayerByWallCollisions();
	CheckObjectByWallCollisions();
	CheckObjectByObjectCollisions();
	CheckObjectByBulletCollisions();
	CheckPlayerByObjectCollisions();

	// Remove destroyed tank
	auto it = std::find_if(m_pObjects.begin(), m_pObjects.end(), [](std::shared_ptr<GameObject> pObj) { return !pObj->IsActive(); });
	if (it != m_pObjects.end()) {
		m_pObjects.erase(it);
		m_nTankDestroyed++;
		it = std::find_if(m_pObjects.begin(), m_pObjects.end(), [](std::shared_ptr<GameObject> pObj) { return !pObj->IsActive(); });
	}

	if (m_nTankDestroyed >= 10) {
		m_bGameEnded = TRUE;
	}

	if (m_bGameEnded) {
		m_pWinTextObject->Update(fTimeElapsed);
	}

}

void Level2Scene::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	Scene::Render(pd3dCommandList);

	m_pWallsObject->Render(pd3dCommandList, m_pPlayer->GetCamera());


	if (m_bGameEnded) {
		m_pWinTextObject->Render(pd3dCommandList, m_pPlayer->GetCamera());
	}
}

void Level2Scene::ProcessMouseInput(float fTimeElapsed)
{
	m_pPlayer->ProcessMouseInput(fTimeElapsed);
}

void Level2Scene::ProcessKeyboardInput(float fTimeElapsed)
{
	if (INPUT.GetButtonDown(VK_RBUTTON)) {
		POINT ptCursorPos = INPUT.GetCurrentCursorPos();
		std::shared_ptr<GameObject> pPickedObj = PickObjectPointedByCursor(ptCursorPos.x, ptCursorPos.y, m_pPlayer->GetCamera());
		if (auto p = dynamic_pointer_cast<ExplosiveObject>(pPickedObj)) {
			static_pointer_cast<TankPlayer>(m_pPlayer)->FireBullet(p);
		}
		else {
			static_pointer_cast<TankPlayer>(m_pPlayer)->FireBullet(nullptr);
		}
	}

	if (INPUT.GetButtonDown('W')) {
		m_bGameEnded = TRUE;
	}

	if (INPUT.GetButtonDown(VK_ESCAPE)) {
		GameFramework::SignalChangeScene(TAG_SCENE_MENU);
		m_bSceneChanged = TRUE;
		return;
	}

	m_pPlayer->ProcessKeyboardInput(fTimeElapsed);
}

void Level2Scene::CheckObjectByBulletCollisions()
{
	auto pBullets = static_pointer_cast<TankPlayer>(m_pPlayer)->GetBullets();

	for (auto& pObj : m_pObjects) {
		for (auto& pBullet : pBullets) {
			if (pBullet->IsActive() && pObj->GetOBB().Intersects(pBullet->GetOBB())) {
				pObj->BeginCollision(pBullet);
				pBullet->BeginCollision(pObj);
			}
		}
	}
}

void Level2Scene::CheckObjectByWallCollisions()
{
	for (shared_ptr<GameObject>& pObj : m_pObjects) {
		if (auto p = dynamic_pointer_cast<TankObject>(pObj))
		{
			XMVECTOR xmvOBBOrientation = XMLoadFloat4(&p->GetOBB().Orientation);
			XMVECTOR xmvOBBNormalizedOrientation = XMQuaternionNormalize(xmvOBBOrientation);
			XMStoreFloat4(&p->GetOBB().Orientation, xmvOBBNormalizedOrientation);

			switch (m_pWallsObject->GetOBB().Contains(p->GetOBB())) {
			case DISJOINT:
			{
				if(!p->GetCollisionSet().contains(m_pWallsObject)) {
					int nPlaneIndex = -1;
					for (auto&& [idx, xmf4Plane] : m_pWallsObject->GetWallPlanes() | std::views::enumerate) {
						PlaneIntersectionType intersectType = p->GetOBB().Intersects(XMLoadFloat4(&xmf4Plane));
						if (intersectType == BACK) {
							nPlaneIndex = idx;
							break;
						}
					}
					if (nPlaneIndex != -1) {
						XMFLOAT3 xmf3CurMovingDirection = p->GetMovingDirection();
						XMFLOAT3 xmf3NewDirection = Vector3::ScalarProduct(xmf3CurMovingDirection, -1);
						p->SetMovingDirection(xmf3NewDirection);
						p->BeginCollision(m_pWallsObject);
					}
					p->GetCollisionSet().insert(m_pWallsObject);
				}
				break;
			}
			case INTERSECTS:
			{
				if (!p->GetCollisionSet().contains(m_pWallsObject)) {
					int nPlaneIndex = -1;
					for (auto&& [idx, xmf4Plane] : m_pWallsObject->GetWallPlanes() | std::views::enumerate) {
						PlaneIntersectionType intersectType = p->GetOBB().Intersects(XMLoadFloat4(&xmf4Plane));
						if (intersectType == INTERSECTING) {
							nPlaneIndex = idx;
							break;
						}
					}
					if (nPlaneIndex != -1) {
						XMFLOAT3 xmf3CurMovingDirection = p->GetMovingDirection();
						XMFLOAT3 xmf3NewDirection = Vector3::ScalarProduct(xmf3CurMovingDirection, -1);
						p->SetMovingDirection(xmf3NewDirection);
						p->BeginCollision(m_pWallsObject);
					}
					p->GetCollisionSet().insert(m_pWallsObject);
				}
				break;

			}
			case CONTAINS:
				if (p->GetCollisionSet().contains(m_pWallsObject)) {
					p->GetCollisionSet().erase(m_pWallsObject);
				}
				break;
			}
		}
	}
}

void Level2Scene::CheckPlayerByWallCollisions()
{
	BoundingOrientedBox xmOBBPlayerMoveCheck;
	m_pWallsObject->GetOBB().Transform(xmOBBPlayerMoveCheck, XMLoadFloat4x4(&m_pWallsObject->GetTransform()->GetWorldMatrix()));
	XMStoreFloat4(&xmOBBPlayerMoveCheck.Orientation, XMQuaternionNormalize(XMLoadFloat4(&xmOBBPlayerMoveCheck.Orientation)));

	if (!xmOBBPlayerMoveCheck.Intersects(m_pPlayer->GetOBB())) {
		if (!m_pPlayer->GetCollisionSet().contains(m_pWallsObject)) {
			m_pPlayer->BeginCollision(m_pWallsObject);
			m_pPlayer->GetCollisionSet().insert(m_pWallsObject);
		}
	}
}

void Level2Scene::CheckObjectByObjectCollisions()
{
	for (int i = 0; i < m_pObjects.size(); ++i) {
		for (int j = i + 1; j < m_pObjects.size(); ++j) {

			shared_ptr<GameObject> pSrc = m_pObjects[i];
			shared_ptr<GameObject> pDest = m_pObjects[j];

			if (pSrc->GetOBB().Intersects(pDest->GetOBB())) {
				if (!pSrc->GetCollisionSet().contains(pDest)) {
					pSrc->BeginCollision(pDest);
					pDest->BeginCollision(pSrc);

					pSrc->GetCollisionSet().insert(pDest);
					pDest->GetCollisionSet().insert(pSrc);
				}
			}
			else {
				if (pSrc->GetCollisionSet().contains(pDest)) {
					pSrc->EndCollision(pDest);
					pDest->EndCollision(pSrc);

					pSrc->GetCollisionSet().erase(pDest);
					pDest->GetCollisionSet().erase(pSrc);
				}
			}
		}
	}
}

void Level2Scene::CheckPlayerByObjectCollisions()
{
	for (shared_ptr<GameObject> pObj : m_pObjects) {
		if (m_pPlayer->GetOBB().Intersects(pObj->GetOBB())) {
			if (!pObj->GetCollisionSet().contains(m_pPlayer)) {
				m_pPlayer->BeginCollision(pObj);
				pObj->BeginCollision(m_pPlayer);

				m_pPlayer->GetCollisionSet().insert(pObj);
				pObj->GetCollisionSet().insert(m_pPlayer);
			}
			else {
				m_pPlayer->InCollision(pObj);
				pObj->InCollision(m_pPlayer);
			}
		}
		else {
			if (pObj->GetCollisionSet().contains(m_pPlayer)) {
				m_pPlayer->EndCollision(pObj);
				pObj->EndCollision(m_pPlayer);

				m_pPlayer->GetCollisionSet().erase(pObj);
				pObj->GetCollisionSet().erase(m_pPlayer);
			}

		}
	}
}
