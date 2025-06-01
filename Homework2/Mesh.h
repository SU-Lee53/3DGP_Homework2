#pragma once


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

	virtual int CheckRayIntersection(const XMVECTOR& xmvPickRayOrigin, const XMVECTOR& xmvPickRayDirection, float& fNearHitDistance) { __debugbreak(); return 0; }

	virtual void ReleaseUploadBuffers() {};

	BoundingOrientedBox& SetOBB(const BoundingOrientedBox& obb) { return m_xmOBB = obb; }
	BoundingOrientedBox& GetOBB() { return m_xmOBB; }

protected:
	BoundingOrientedBox						m_xmOBB = {};

};

template<VertexType T>
class Mesh : public Mesh_Base {
public:
	Mesh() {}
	virtual ~Mesh() {}

public:
	void SetVertices(std::span<T> vertices) {
		m_Vertices.clear();
		m_Vertices.reserve(vertices.size());
		std::copy(vertices.begin(), vertices.end(), std::back_inserter(m_Vertices));
	}

	void SetIndices(std::span<UINT> indices) {
		m_Indices.clear();
		m_Indices.reserve(indices.size());
		std::copy(indices.begin(), indices.end(), std::back_inserter(m_Indices));
	}

	void SetSlot(UINT nSlot) { m_nSlot = nSlot; }
	void SetStride(UINT nStride) { m_nStride = nStride; }
	void SetOffset(UINT nOffset) { m_nOffset = nOffset; }
	
	void SetStartIndex(UINT nIndex) { m_nStartIndex = nIndex; }
	void SetBaseVertex(UINT nBase) { m_nBaseVertex = nBase; }
	
	void SetMinYPos(const XMFLOAT3& xmf3MinYPos) { m_xmf3MinYPos = xmf3MinYPos; }
	void ComputeMinYPos(const XMFLOAT3& xmf3DefaultOrientation) {

		XMFLOAT4X4 xmf4x4Orientation;

		float fPitch = XMConvertToRadians(xmf3DefaultOrientation.x);
		float fYaw = XMConvertToRadians(xmf3DefaultOrientation.y);
		float fRoll = XMConvertToRadians(xmf3DefaultOrientation.z);

		XMStoreFloat4x4(&xmf4x4Orientation, XMMatrixRotationRollPitchYaw(fPitch, fYaw, fRoll));

		auto pos = std::min_element(m_Vertices.begin(), m_Vertices.end(), [&xmf4x4Orientation](const T& lhs, const T& rhs) {
			float fLhsY = Vector3::TransformNormal(lhs.m_xmf3Position, xmf4x4Orientation).y;
			float fRhsY = Vector3::TransformNormal(rhs.m_xmf3Position, xmf4x4Orientation).y;

			return fLhsY < fRhsY;
		});

		SetMinYPos(pos->m_xmf3Position);
	}

	XMFLOAT3 GetMinYPos() { return m_xmf3MinYPos; }


	void Create(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nInstances = 1) override;

	virtual int CheckRayIntersection(const XMVECTOR& xmvPickRayOrigin, const XMVECTOR& xmvPickRayDirection, float& fNearHitDistance) override;

	virtual void ReleaseUploadBuffers() override;

protected:
	std::vector<T>			m_Vertices = {};
	ComPtr<ID3D12Resource>	m_pd3dVertexBuffer = nullptr;
	ComPtr<ID3D12Resource>	m_pd3dVertexUploadBuffer = nullptr;

	std::vector<UINT>		m_Indices = {};
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

	XMFLOAT3 m_xmf3MinYPos = {};


};

template<VertexType T>
void Mesh<T>::ReleaseUploadBuffers()
{
	// Destroy upload buffer for vertex buffer
	if (m_pd3dVertexUploadBuffer) m_pd3dVertexUploadBuffer->Release();
	m_pd3dVertexUploadBuffer = nullptr;

	if (m_pd3dIndexUploadBuffer) m_pd3dIndexUploadBuffer->Release();
	m_pd3dIndexUploadBuffer = nullptr;
}

template<VertexType T>
void Mesh<T>::Create(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	assert(!m_Vertices.empty());

	// Vertex Buffer + Vertex Buffer View
	m_pd3dVertexBuffer = ::CreateBufferResources(
		pd3dDevice, 
		pd3dCommandList, 
		(void*)m_Vertices.data(),
		m_nStride * m_Vertices.size(),
		D3D12_HEAP_TYPE_DEFAULT, 
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		m_pd3dVertexUploadBuffer.GetAddressOf()
	);

	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = m_nStride;
	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_Vertices.size();

	// Index Buffer + Index Buffer View
	m_pd3dIndexBuffer = ::CreateBufferResources(
		pd3dDevice,
		pd3dCommandList,
		(void*)m_Indices.data(),
		sizeof(UINT) * m_Indices.size(),
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		m_pd3dIndexUploadBuffer.GetAddressOf()
	);

	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_Indices.size();
}

template<VertexType T>
void Mesh<T>::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	// Set Mesh's primitive topology
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	// Set Mesh's vertex buffer view
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dVertexBufferView);

	if (m_pd3dIndexBuffer) {
		pd3dCommandList->IASetIndexBuffer(&m_d3dIndexBufferView);
		pd3dCommandList->DrawIndexedInstanced(m_Indices.size(), 1, 0, 0, 0);
	}
	else {
		pd3dCommandList->DrawInstanced(m_Vertices.size(), 1, m_nOffset, 0);
	}
}

template<VertexType T>
void Mesh<T>::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nInstances)
{
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dVertexBufferView);
	if (m_pd3dIndexBuffer) {
		pd3dCommandList->IASetIndexBuffer(&m_d3dIndexBufferView);
		pd3dCommandList->DrawIndexedInstanced(m_Indices.size(), nInstances, 0, 0, 0);
	}
	else {
		pd3dCommandList->DrawInstanced(m_Vertices.size(), nInstances, m_nOffset, 0);
	}
}

template<VertexType T>
int Mesh<T>::CheckRayIntersection(const XMVECTOR& xmvPickRayOrigin, const XMVECTOR& xmvPickRayDirection, float& fNearHitDistance)
{
	int nIntersections = 0;
	bool bIntersected = m_xmOBB.Intersects(xmvPickRayOrigin, xmvPickRayDirection, fNearHitDistance);

	for (const auto& polygon : m_Indices | std::views::chunk(3)) {
		XMVECTOR v0 = XMLoadFloat3(&m_Vertices[polygon[0]].m_xmf3Position);
		XMVECTOR v1 = XMLoadFloat3(&m_Vertices[polygon[1]].m_xmf3Position);
		XMVECTOR v2 = XMLoadFloat3(&m_Vertices[polygon[2]].m_xmf3Position);
		BOOL bIntersected = RayIntersectionByTriangle(xmvPickRayOrigin, xmvPickRayDirection, v0, v1, v2, fNearHitDistance);
		if (bIntersected) nIntersections++;
	}

	return nIntersections;
}
