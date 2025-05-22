#pragma once
#include "VertexType.h"


template<typename T>
class Mesh {
public:
	Mesh() = default;

public:
	void Create(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nInstances = 1);

	BOOL RayIntersectionByTriangle(const XMVECTOR& xmvRayOrigin, const XMVECTOR& xmvRayDirection,
		const XMVECTOR& v0, const XMVECTOR v1, const XMVECTOR v2, float& fNearHitDistance);

	int CheckRayIntersection(const XMVECTOR& xmvPickRayOrigin, const XMVECTOR& xmvPickRayDirection, float& fNearHitDistance);

	void ReleaseUploadBuffers();

protected:
	std::vector<T>			m_xmf3Vertices = {};
	ComPtr<ID3D12Resource>	m_pd3dVertexBuffer = nullptr;
	ComPtr<ID3D12Resource>	m_pd3dVertexUploadBuffer = nullptr;

	std::vector<UINT>		m_xmf3Indices = {};
	ComPtr<ID3D12Resource>	m_pd3dIndexBuffer = nullptr;
	ComPtr<ID3D12Resource>	m_pd3dIndexUploadBuffer = nullptr;

	D3D12_VERTEX_BUFFER_VIEW	m_d3dVertexBufferView = {};
	D3D12_INDEX_BUFFER_VIEW		m_d3dIndexBufferView = {};

	D3D12_PRIMITIVE_TOPOLOGY	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT m_nSlot = 0;
	UINT m_nStride = 0;
	UINT m_nOffset = 0;

	UINT m_nStartIndex = 0;
	UINT m_nBaseVertex = 0;

	BoundingOrientedBox						m_xmOBB = {};


#pragma region Friend_MeshHelper
	friend void MeshHelper::CreateCubeMesh(std::shared_ptr<Mesh> pMesh, float fWidth, float fHeight, float fDepth);
	friend void MeshHelper::CreateWallMesh(std::shared_ptr<Mesh> pMesh, float fWidth, float fHeight, float fDepth, int nSubRects);
	friend void MeshHelper::CreateAirplaneMesh(std::shared_ptr<Mesh> pMesh, float fWidth, float fHeight, float fDepth);
	friend BOOL MeshHelper::CreateMeshFromOBJFiles(std::shared_ptr<Mesh> pMesh, std::wstring_view wstrObjPath);
	friend void MeshHelper::CreateRollercoasterRailMesh(std::shared_ptr<Mesh> pMesh, OUT std::vector<XMFLOAT3>& RollercoasterRoute,
		float fWidth, float fCourseRadius, int nControlPoints, int nInterpolateBias);

#pragma endregion
};

// ==========
//  AxisMesh
// ==========

class AxisMesh final : public Mesh {
public:
	AxisMesh(float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f);
	virtual ~AxisMesh() {}

	virtual void Render(HDC hDCFrameBuffer);
};

