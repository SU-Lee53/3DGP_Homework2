#pragma once
#include "VertexType.h"


class Mesh_Base {
public:
	Mesh_Base() {}
	virtual ~Mesh_Base() {}

public:
	virtual void Create(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) {}
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) {}
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nInstances = 1) {}

	BOOL RayIntersectionByTriangle(const XMVECTOR& xmvRayOrigin, const XMVECTOR& xmvRayDirection,
		const XMVECTOR& v0, const XMVECTOR& v1, const XMVECTOR& v2, float& fNearHitDistance);

	virtual int CheckRayIntersection(const XMVECTOR& xmvPickRayOrigin, const XMVECTOR& xmvPickRayDirection, float& fNearHitDistance) {}

	virtual void ReleaseUploadBuffers() {};

	BoundingOrientedBox& GetOBB() { return m_xmOBB; }

protected:
	BoundingOrientedBox						m_xmOBB = {};

};

template<typename T>
class Mesh : public Mesh_Base {
public:
	Mesh() {}
	virtual ~Mesh() {}

public:

	void Create(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nInstances = 1) override;

	virtual int CheckRayIntersection(const XMVECTOR& xmvPickRayOrigin, const XMVECTOR& xmvPickRayDirection, float& fNearHitDistance) override;


	virtual void ReleaseUploadBuffers() override;

protected:
	std::vector<T>			m_xmf3Vertices = {};
	ComPtr<ID3D12Resource>	m_pd3dVertexBuffer = nullptr;
	ComPtr<ID3D12Resource>	m_pd3dVertexUploadBuffer = nullptr;

	std::vector<Index>		m_uiIndices = {};
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

#pragma region Friend_MeshHelper
	friend void MeshHelper::CreateCubeMesh(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, shared_ptr<Mesh<DiffusedVertex>> pMesh, float fWidth, float fHeight, float fDepth, const XMFLOAT4& xmf4Color);
	friend void MeshHelper::CreateWallMesh(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Mesh<DiffusedVertex>> pMesh, float fWidth, float fHeight, float fDepth, int nSubRects, const XMFLOAT4& xmf4Color);
	friend BOOL MeshHelper::CreateMeshFromOBJFiles(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Mesh<DiffusedVertex>> pMesh, std::wstring_view wstrObjPath, const XMFLOAT4& xmf4Color);
	friend void MeshHelper::CreateRollercoasterRailMesh(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, shared_ptr<Mesh<DiffusedVertex>> pMesh, OUT std::vector<XMFLOAT3>& RollercoasterRoute, float fWidth, float fCourseRadius, int nControlPoints, int nInterpolateBias, const XMFLOAT4& xmf4Color);
	friend void GenerateRollercoasterPillarPolygon(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, shared_ptr<Mesh<DiffusedVertex>> pMesh, XMFLOAT3 xmf3TopPosition, float fWidth, float fDepth, const XMFLOAT4& xmf4Color);
	

#pragma endregion
};

template<typename T>
void Mesh<T>::ReleaseUploadBuffers()
{
	// Destroy upload buffer for vertex buffer
	if (m_pd3dVertexUploadBuffer) m_pd3dVertexUploadBuffer->Release();
	m_pd3dVertexUploadBuffer = nullptr;

	if (m_pd3dIndexUploadBuffer) m_pd3dIndexUploadBuffer->Release();
	m_pd3dIndexUploadBuffer = nullptr;
}

template<typename T>
void Mesh<T>::Create(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	assert(!m_xmf3Vertices.empty());

	// Vertex Buffer + Vertex Buffer View
	m_pd3dVertexBuffer = ::CreateBufferResources(pd3dDevice, pd3dCommandList, (void*)m_xmf3Vertices.data(), m_nStride * m_xmf3Vertices.size(),
		D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, m_pd3dVertexUploadBuffer.GetAddressOf());

	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = m_nStride;
	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_xmf3Vertices.size();

	// Index Buffer + Index Buffer View
	m_pd3dIndexBuffer = ::CreateBufferResources(
		pd3dDevice,
		pd3dCommandList,
		(void*)m_uiIndices.data(),
		sizeof(UINT) * m_uiIndices.size(),
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&m_pd3dIndexUploadBuffer
	);

	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_uiIndices.size();
}

template<typename T>
void Mesh<T>::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	// Set Mesh's primitive topology
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	// Set Mesh's vertex buffer view
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dVertexBufferView);

	if (m_pd3dIndexBuffer) {
		pd3dCommandList->IASetIndexBuffer(&m_d3dIndexBufferView);
		pd3dCommandList->DrawIndexedInstanced(m_uiIndices.size(), 1, 0, 0, 0);
	}
	else {
		pd3dCommandList->DrawInstanced(m_xmf3Vertices.size(), 1, m_nOffset, 0);
	}
}

template<typename T>
void Mesh<T>::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nInstances)
{
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dVertexBufferView);
	if (m_pd3dIndexBuffer) {
		pd3dCommandList->IASetIndexBuffer(&m_d3dIndexBufferView);
		pd3dCommandList->DrawIndexedInstanced(m_uiIndices.size(), nInstances, 0, 0, 0);
	}
	else {
		pd3dCommandList->DrawInstanced(m_xmf3Vertices.size(), nInstances, m_nOffset, 0);
	}
}

template<typename T>
int Mesh<T>::CheckRayIntersection(const XMVECTOR& xmvPickRayOrigin, const XMVECTOR& xmvPickRayDirection, float& fNearHitDistance)
{
	int nIntersections = 0;
	bool bIntersected = m_xmOBB.Intersects(xmvPickRayOrigin, xmvPickRayDirection, fNearHitDistance);

	for (auto polygon : m_uiIndices | std::views::chunk(3)) {
		XMVECTOR v0 = XMLoadFloat3(&m_xmf3Vertices[polygon[0]]);
		XMVECTOR v1 = XMLoadFloat3(&m_xmf3Vertices[polygon[1]]);
		XMVECTOR v2 = XMLoadFloat3(&m_xmf3Vertices[polygon[2]]);
		BOOL bIntersected = RayIntersectionByTriangle(xmvPickRayOrigin, xmvPickRayDirection, v0, v1, v2, fNearHitDistance);
		if (bIntersected) nIntersections++;
	}

	return nIntersections;
}
