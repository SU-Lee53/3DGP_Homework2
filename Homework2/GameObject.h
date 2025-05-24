#pragma once
#include "Transform.h"

class Shader;

enum TAG_GAMEOBJECT_TYPE : UINT8 {
	TAG_GAMEOBJECT_TYPE_DEFAULT = 0,
	TAG_GAMEOBJECT_TYPE_PLAYER = 0,
	TAG_GAMEOBJECT_TYPE_EXPLOSIVE,
	TAG_GAMEOBJECT_TYPE_TANK,
	TAG_GAMEOBJECT_TYPE_OBSTACLE,
	TAG_GAMEOBJECT_TYPE_COUNT,

	TAG_GAMEOBJECT_TYPE_UNDEFINED = 99
};

class GameObject : public std::enable_shared_from_this<GameObject> {
public:
	GameObject();
	virtual ~GameObject() { 
		m_pCollisionSet.clear();
	}

public:
	void SetActive(BOOL bActive) { m_bActive = bActive; }
	BOOL IsActive() { return m_bActive; }
	void SetMesh(const std::shared_ptr<Mesh_Base>& pMesh); 

	void SetColor(COLORREF color) { m_Color = color; }
	void SetName(std::string_view svName) { m_strObjectName = svName; }

	void SetMeshDefaultOrientation(const XMFLOAT3& xmf3Orientation) { m_xmf3DefaultOrientation = xmf3Orientation; }

	BoundingOrientedBox& GetOBB() { return m_xmOBB; }

	TAG_GAMEOBJECT_TYPE GetObjectType() { return m_eObjectType; }

	std::string_view GetName() { return m_strObjectName; }

	std::shared_ptr<Shader>& GetShader() { return m_pShader; }
	std::shared_ptr<Mesh_Base>& GetMesh() { return m_pMesh; }


	void LookTo(const XMFLOAT3& xmf3LookTo, const XMFLOAT3& xmf3Up);
	void LookAt(const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up);

	void UpdateBoundingBox();

	virtual void Initialize();
	virtual void Update(float fElapsedTime);
	virtual void Render(std::shared_ptr<class Camera> pCamera);


	virtual void OnPicked() { }
	virtual void BeginCollision(std::shared_ptr<GameObject> pOther) {}
	virtual void EndCollision(std::shared_ptr<GameObject> pOther) {}


	void GenerateRayForPicking(XMVECTOR& xmvPickPosition, const XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection) const;
	int PickObjectByRayIntersection(XMVECTOR& xmvPickPosition, const XMMATRIX& xmmtxView, float& fHitDistance) const;

	std::shared_ptr<Transform>& GetTransform() { return m_pTransform; }

	std::unordered_set<std::shared_ptr<GameObject>> GetCollisionSet() { return m_pCollisionSet; }

protected:
	BOOL							m_bActive = TRUE;
	
	std::shared_ptr<Mesh_Base>		m_pMesh = nullptr;
	std::shared_ptr<Shader>			m_pShader = nullptr;
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

