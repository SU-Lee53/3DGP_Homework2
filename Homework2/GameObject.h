#pragma once
#include "Transform.h"
#include "Mesh.h"

class Shader;

struct VS_TRANSFORM_DATA {
	XMFLOAT4X4 xmf4x4Model;
	XMFLOAT4X4 xmf4x4World;
};

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
	void SetShader(const std::shared_ptr<Shader>& pShader) { m_pShader = pShader; }
	void SetColor(const XMFLOAT4& color) { m_xmf4Color = color; }
	void SetColor(COLORREF color) {
		m_xmf4Color = ::ConvertWinColorToD3DColor(color); 
	}
	void SetName(std::string_view svName) { m_strObjectName = svName; }
	void SetMeshDefaultOrientation(const XMFLOAT3& xmf3Orientation) { m_xmf3DefaultOrientation = xmf3Orientation; }

	BoundingOrientedBox& GetOBB() { return m_xmOBB; }
	TAG_GAMEOBJECT_TYPE GetObjectType() { return m_eObjectType; }
	std::string_view GetName() { return m_strObjectName; }
	std::shared_ptr<Shader>& GetShader() { return m_pShader; }
	std::shared_ptr<Mesh_Base>& GetMesh() { return m_pMesh; }
	XMFLOAT4& GetColor() { return m_xmf4Color; }
	XMFLOAT4X4 GetModelTransform();


	void LookTo(const XMFLOAT3& xmf3LookTo, const XMFLOAT3& xmf3Up);
	void LookAt(const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up);

	void UpdateBoundingBox();

	virtual void Initialize(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	virtual void Update(float fElapsedTime);
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<class Camera> pCamera);


	virtual void OnPicked() { }
	virtual void BeginCollision(std::shared_ptr<GameObject> pOther) {}
	virtual void EndCollision(std::shared_ptr<GameObject> pOther) {}


	void GenerateRayForPicking(XMVECTOR& xmvPickPosition, const XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection) const;
	BOOL PickObjectByRayIntersection(XMVECTOR& xmvPickPosition, const XMMATRIX& xmmtxView, float& fHitDistance) const;

	std::shared_ptr<Transform>& GetTransform() { return m_pTransform; }

	std::unordered_set<std::shared_ptr<GameObject>> GetCollisionSet() { return m_pCollisionSet; }

public:
	virtual void CreateShaderVariables(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	virtual void UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);

private:
	void RenderObject(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<class Camera> pCamera);

protected:
	BOOL							m_bActive = TRUE;
	
	std::shared_ptr<Mesh_Base>		m_pMesh = nullptr;
	std::shared_ptr<Shader>			m_pShader = nullptr;
	std::shared_ptr<Transform>		m_pTransform = nullptr;

	BoundingOrientedBox				m_xmOBB = BoundingOrientedBox{};

	XMFLOAT4						m_xmf4Color = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f };

	TAG_GAMEOBJECT_TYPE				m_eObjectType = TAG_GAMEOBJECT_TYPE_DEFAULT;
	std::string						m_strObjectName = "";

	XMFLOAT3						m_xmf3DefaultOrientation = {};

	// Collision
	std::unordered_set<std::shared_ptr<GameObject>> m_pCollisionSet = {};

	// Shader Variables
	std::unique_ptr<ConstantBuffer<VS_TRANSFORM_DATA>> m_upcbTransfromBuffer;

//#define _DEBUG_COLLISION

#ifdef _DEBUG_COLLISION
	std::shared_ptr<class Mesh> m_pObjectOBBMesh;
	std::shared_ptr<class Mesh> m_pMeshOBBMesh;
#endif


};

