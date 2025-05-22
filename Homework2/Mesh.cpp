#include "stdafx.h"
#include "Mesh.h"

using namespace std;

void Mesh::ReleaseUploadBuffers()
{
	// Destroy upload buffer for vertex buffer
	if (m_pd3dVertexUploadBuffer) m_pd3dVertexUploadBuffer->Release();
	m_pd3dVertexUploadBuffer = nullptr;

	if (m_pd3dIndexUploadBuffer) m_pd3dIndexUploadBuffer->Release();
	m_pd3dIndexUploadBuffer = nullptr;
}

void Mesh::Create(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
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
		(void*)m_xmf3Indices.data(),
		sizeof(UINT) * m_xmf3Indices.size(),
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&m_pd3dIndexUploadBuffer
	);

	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_xmf3Indices.size();
}

void Mesh::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	// Set mesh's primitive topology
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	// Set mesh's vertex buffer view
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dVertexBufferView);

	if (m_pd3dIndexBuffer) {
		pd3dCommandList->IASetIndexBuffer(&m_d3dIndexBufferView);
		pd3dCommandList->DrawIndexedInstanced(m_xmf3Indices.size(), 1, 0, 0, 0);
	}
	else {
		pd3dCommandList->DrawInstanced(m_xmf3Vertices.size(), 1, m_nOffset, 0);
	}
}

void Mesh::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nInstances)
{
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dVertexBufferView);
	if (m_pd3dIndexBuffer) {
		pd3dCommandList->IASetIndexBuffer(&m_d3dIndexBufferView);
		pd3dCommandList->DrawIndexedInstanced(m_xmf3Indices.size(), nInstances, 0, 0, 0);
	}
	else {
		pd3dCommandList->DrawInstanced(m_xmf3Vertices.size(), nInstances, m_nOffset, 0);
	}
}

BOOL Mesh::RayIntersectionByTriangle(const XMVECTOR& xmvRayOrigin, const XMVECTOR& xmvRayDirection, const XMVECTOR& v0, const XMVECTOR v1, const XMVECTOR v2, float& fNearHitDistance)
{
	float fHitDistance;
	BOOL bIntersected = TriangleTests::Intersects(xmvRayOrigin, xmvRayDirection, v0, v1, v2, fHitDistance);
	if (bIntersected && (fHitDistance < fNearHitDistance)) {
		fNearHitDistance = fHitDistance;
	}

	return bIntersected;
}

int Mesh::CheckRayIntersection(const XMVECTOR& xmvPickRayOrigin, const XMVECTOR& xmvPickRayDirection, float& fNearHitDistance)
{
	int nIntersections = 0;
	bool bIntersected = m_xmOBB.Intersects(xmvPickRayOrigin, xmvPickRayDirection, fNearHitDistance);
	
	for (auto polygon : m_xmf3Indices | std::views::chunk(3)) {
		XMVECTOR v0 = XMLoadFloat3(&m_xmf3Vertices[polygon[0]]);
		XMVECTOR v1 = XMLoadFloat3(&m_xmf3Vertices[polygon[1]]);
		XMVECTOR v2 = XMLoadFloat3(&m_xmf3Vertices[polygon[2]]);
		BOOL bIntersected = RayIntersectionByTriangle(xmvPickRayOrigin, xmvPickRayDirection, v0, v1, v2, fNearHitDistance);
		if (bIntersected) nIntersections++;
	}

	return nIntersections;
}

// ==========
//  AxisMesh
// ==========

AxisMesh::AxisMesh(float fWidth, float fHeight, float fDepth)
{
	/*
	m_pPolygons.resize(3);
	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;

	shared_ptr<struct Polygon> pxAxis = make_shared<struct Polygon>(2);
	pxAxis->SetVertex(0, Vertex(-fHalfWidth, 0.0f, 0.0f));
	pxAxis->SetVertex(1, Vertex(+fHalfWidth, 0.0f, 0.0f));
	SetPolygon(0, pxAxis);

	shared_ptr<struct Polygon> pyAxis = make_shared<struct Polygon>(2);
	pyAxis->SetVertex(0, Vertex(0.0f, -fHalfHeight, 0.0f));
	pyAxis->SetVertex(1, Vertex(0.0f, +fHalfHeight, 0.0f));
	SetPolygon(1, pyAxis);

	shared_ptr<struct Polygon> pzAxis = make_shared<struct Polygon>(2);
	pzAxis->SetVertex(0, Vertex(0.0f, 0.0f, -fHalfDepth));
	pzAxis->SetVertex(1, Vertex(0.0f, 0.0f, +fHalfDepth));
	SetPolygon(2, pzAxis);
	*/
}

void AxisMesh::Render(HDC hDCFrameBuffer)
{
}
