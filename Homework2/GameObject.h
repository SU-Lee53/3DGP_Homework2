#pragma once
#include "Transform.h"

enum TAG_GAMEOBJECT_TYPE : UINT8 {
	TAG_GAMEOBJECT_TYPE_DEFAULT = 0,
	TAG_GAMEOBJECT_TYPE_PLAYER = 0,
	TAG_GAMEOBJECT_TYPE_EXPLOSIVE,
	TAG_GAMEOBJECT_TYPE_TANK,
	TAG_GAMEOBJECT_TYPE_OBSTACLE,
	TAG_GAMEOBJECT_TYPE_COUNT,

	TAG_GAMEOBJECT_TYPE_UNDEFINED = 99
};

class GameObject {
public:
	GameObject();
	virtual ~GameObject() { 
		m_pCollisionSet.clear();
	}

public:
	void SetActive(BOOL bActive) { m_bActive = bActive; }
	BOOL IsActive() { return m_bActive; }
	void SetMesh(const std::shared_ptr<Mesh>& pMesh); 

	void SetColor(COLORREF color) { m_Color = color; }
	void SetName(std::string_view svName) { m_strObjectName = svName; }

	void SetMeshDefaultOrientation(const XMFLOAT3& xmf3Orientation) { m_xmf3DefaultOrientation = xmf3Orientation; }

	BoundingOrientedBox& GetOBB() { return m_xmOBB; }

	TAG_GAMEOBJECT_TYPE GetObjectType() { return m_eObjectType; }

	std::string_view GetName() { return m_strObjectName; }


	void LookTo(const XMFLOAT3& xmf3LookTo, const XMFLOAT3& xmf3Up);
	void LookAt(const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up);

	void UpdateBoundingBox();

	virtual void Initialize();
	virtual void Update(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, std::shared_ptr<class Camera> pCamera);
	void Render(HDC hDCFrameBuffer, XMFLOAT4X4& pxmf4x4World, std::shared_ptr<Mesh> pMesh) const;


	virtual void OnPicked() { }
	virtual void BeginCollision(std::shared_ptr<GameObject> pOther) {}
	virtual void EndCollision(std::shared_ptr<GameObject> pOther) {}


	void GenerateRayForPicking(XMVECTOR& xmvPickPosition, const XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection) const;
	int PickObjectByRayIntersection(XMVECTOR& xmvPickPosition, const XMMATRIX& xmmtxView, float& fHitDistance) const;

	std::shared_ptr<Transform>& GetTransform() { return m_pTransform; }

	std::unordered_set<std::shared_ptr<GameObject>> GetCollisionSet() { return m_pCollisionSet; }

protected:
	BOOL							m_bActive = TRUE;
	
	std::shared_ptr<class Mesh>		m_pMesh = nullptr;
	std::shared_ptr<Transform>		m_pTransform = nullptr;

	BoundingOrientedBox				m_xmOBB = BoundingOrientedBox{};

	COLORREF						m_Color = RGB(255, 0, 0);

	TAG_GAMEOBJECT_TYPE				m_eObjectType = TAG_GAMEOBJECT_TYPE_DEFAULT;
	std::string						m_strObjectName = "";

	XMFLOAT3						m_xmf3DefaultOrientation = {};

	// Collision
	std::unordered_set<std::shared_ptr<GameObject>> m_pCollisionSet = {};


//#define _DEBUG_COLLISION

#ifdef _DEBUG_COLLISION
	std::shared_ptr<class Mesh> m_pObjectOBBMesh;
	std::shared_ptr<class Mesh> m_pMeshOBBMesh;
#endif


};

