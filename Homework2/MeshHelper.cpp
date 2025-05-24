#include "stdafx.h"
#include <fstream>
#include "MeshHelper.h"
#include "Mesh.h"

using namespace std;

void MeshHelper::CreateCubeMesh(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, 
	shared_ptr<Mesh<DiffusedVertex>> pMesh, float fWidth, float fHeight, float fDepth, const XMFLOAT4& xmf4Color)
{
	pMesh->m_xmf3Vertices.resize(8);
	pMesh->m_nStride = sizeof(DiffusedVertex);
	pMesh->m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// fWidth	-> x-axis length
	// fHeight	-> y-axis length
	// fDepth	-> z-axis length

	float fx = fWidth * 0.5f;
	float fy = fHeight * 0.5f;
	float fz = fDepth * 0.5f;

	pMesh->m_xmf3Vertices[0] = DiffusedVertex(XMFLOAT3(-fx, +fy, -fz), xmf4Color);
	pMesh->m_xmf3Vertices[1] = DiffusedVertex(XMFLOAT3(+fx, +fy, -fz), xmf4Color);
	pMesh->m_xmf3Vertices[2] = DiffusedVertex(XMFLOAT3(+fx, +fy, +fz), xmf4Color);
	pMesh->m_xmf3Vertices[3] = DiffusedVertex(XMFLOAT3(-fx, +fy, +fz), xmf4Color);
	pMesh->m_xmf3Vertices[4] = DiffusedVertex(XMFLOAT3(-fx, -fy, -fz), xmf4Color);
	pMesh->m_xmf3Vertices[5] = DiffusedVertex(XMFLOAT3(+fx, -fy, -fz), xmf4Color);
	pMesh->m_xmf3Vertices[6] = DiffusedVertex(XMFLOAT3(+fx, -fy, +fz), xmf4Color);
	pMesh->m_xmf3Vertices[7] = DiffusedVertex(XMFLOAT3(-fx, -fy, +fz), xmf4Color);


	pMesh->m_pd3dVertexBuffer = ::CreateBufferResources(pd3dDevice, pd3dCommandList, pMesh->m_xmf3Vertices.data(),
		pMesh->m_nStride * pMesh->m_xmf3Vertices.size(), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &pMesh->m_pd3dVertexUploadBuffer);

	pMesh->m_d3dVertexBufferView.BufferLocation = pMesh->m_pd3dVertexBuffer->GetGPUVirtualAddress();
	pMesh->m_d3dVertexBufferView.StrideInBytes = pMesh->m_nStride;
	pMesh->m_d3dVertexBufferView.SizeInBytes = pMesh->m_nStride * pMesh->m_xmf3Vertices.size();


	pMesh->m_uiIndices.resize(12);

	// Front
	pMesh->m_uiIndices[0] = { 3,1,0 };
	pMesh->m_uiIndices[1] = { 2,1,3 };

	// Top
	pMesh->m_uiIndices[2] = { 0,5,4 };
	pMesh->m_uiIndices[3] = { 1,5,0 };

	// Back
	pMesh->m_uiIndices[4] = { 3,4,7 };
	pMesh->m_uiIndices[5] = { 0,4,3 };

	// Bottom
	pMesh->m_uiIndices[6] = { 1,6,5 };
	pMesh->m_uiIndices[7] = { 2,6,1 };

	// Left
	pMesh->m_uiIndices[8] = { 2,7,6 };
	pMesh->m_uiIndices[9] = { 3,7,2 };

	// Right
	pMesh->m_uiIndices[10] = { 6,4,5 };
	pMesh->m_uiIndices[11] = { 7,4,6 };

	pMesh->m_pd3dIndexBuffer = ::CreateBufferResources(
		pd3dDevice,
		pd3dCommandList,
		pMesh->m_uiIndices.data(),
		sizeof(UINT) * (pMesh->m_uiIndices.size() * 3),
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&pMesh->m_pd3dIndexUploadBuffer
	);

	pMesh->m_d3dIndexBufferView.BufferLocation = pMesh->m_pd3dIndexBuffer->GetGPUVirtualAddress();
	pMesh->m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	pMesh->m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * (pMesh->m_uiIndices.size() * 3);

	pMesh->m_xmOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fx, fy, fz), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

void MeshHelper::CreateWallMesh(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, 
	shared_ptr<Mesh<DiffusedVertex>> pMesh, float fWidth, float fHeight, float fDepth, int nSubRects, const XMFLOAT4& xmf4Color)
{
	pMesh->m_xmf3Vertices.reserve((6 * nSubRects * nSubRects));

	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;
	float fCellWidth = fWidth * (1.0f / nSubRects);
	float fCellHeight = fHeight * (1.0f / nSubRects);
	float fCellDepth = fDepth * (1.0f / nSubRects);

	int k = 0;
	for (int i = 0; i < nSubRects; i++){
		for (int j = 0; j < nSubRects; j++){
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth, -fHalfHeight + (i * fCellHeight),		-fHalfDepth + (j * fCellDepth) },xmf4Color });			// 0
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{-fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight),	-fHalfDepth + (j * fCellDepth) }, xmf4Color });			// 1
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{-fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight),	-fHalfDepth + ((j + 1) * fCellDepth) }, xmf4Color });	// 2
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{-fHalfWidth, -fHalfHeight + (i * fCellHeight),			-fHalfDepth + ((j + 1) * fCellDepth) }, xmf4Color });	// 3
			
			UINT lastVertexIndex = pMesh->m_xmf3Vertices.size();
			pMesh->m_uiIndices.push_back(Index{ lastVertexIndex - 4, lastVertexIndex - 3, lastVertexIndex - 2 });
			pMesh->m_uiIndices.push_back(Index{ lastVertexIndex - 4, lastVertexIndex - 2, lastVertexIndex - 1 });
		}
	}

	shared_ptr<struct Polygon> pRightFace;
	for (int i = 0; i < nSubRects; i++){
		for (int j = 0; j < nSubRects; j++){
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ +fHalfWidth, -fHalfHeight + (i * fCellHeight),		-fHalfDepth + (j * fCellDepth) }, xmf4Color});
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ +fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight),	-fHalfDepth + (j * fCellDepth) }, xmf4Color });
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ +fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight),	-fHalfDepth + ((j + 1) * fCellDepth) }, xmf4Color });
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ +fHalfWidth, -fHalfHeight + (i * fCellHeight),		-fHalfDepth + ((j + 1) * fCellDepth) }, xmf4Color });
			
			UINT lastVertexIndex = pMesh->m_xmf3Vertices.size();
			pMesh->m_uiIndices.push_back(Index{ lastVertexIndex - 4, lastVertexIndex - 3, lastVertexIndex - 2 });
			pMesh->m_uiIndices.push_back(Index{ lastVertexIndex - 4, lastVertexIndex - 2, lastVertexIndex - 1 });
		}
	}

	for (int i = 0; i < nSubRects; i++){
		for (int j = 0; j < nSubRects; j++){
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),			+fHalfHeight,	-fHalfDepth + (j * fCellDepth) }, xmf4Color });
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),		+fHalfHeight,	-fHalfDepth + (j * fCellDepth) }, xmf4Color });
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),		+fHalfHeight,	-fHalfDepth + ((j + 1) * fCellDepth) }, xmf4Color});
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),			+fHalfHeight,	-fHalfDepth + ((j + 1) * fCellDepth) }, xmf4Color});

			UINT lastVertexIndex = pMesh->m_xmf3Vertices.size();
			pMesh->m_uiIndices.push_back(Index{ lastVertexIndex - 4, lastVertexIndex - 3, lastVertexIndex - 2 });
			pMesh->m_uiIndices.push_back(Index{ lastVertexIndex - 4, lastVertexIndex - 2, lastVertexIndex - 1 });
		}
	}

	for (int i = 0; i < nSubRects; i++){
		for (int j = 0; j < nSubRects; j++){
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),		-fHalfHeight,	-fHalfDepth + (j * fCellDepth) }, xmf4Color });
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight,	-fHalfDepth + (j * fCellDepth) }, xmf4Color });
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight,	-fHalfDepth + ((j + 1) * fCellDepth) }, xmf4Color });
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),		-fHalfHeight,	-fHalfDepth + ((j + 1) * fCellDepth) }, xmf4Color });

			UINT lastVertexIndex = pMesh->m_xmf3Vertices.size();
			pMesh->m_uiIndices.push_back(Index{ lastVertexIndex - 4, lastVertexIndex - 3, lastVertexIndex - 2 });
			pMesh->m_uiIndices.push_back(Index{ lastVertexIndex - 4, lastVertexIndex - 2, lastVertexIndex - 1 });
		}
	}

	for (int i = 0; i < nSubRects; i++) {
		for (int j = 0; j < nSubRects; j++) {
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),		-fHalfHeight + (j * fCellHeight),		+fHalfDepth }, xmf4Color });
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight + (j * fCellHeight),		+fHalfDepth }, xmf4Color });
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight + ((j + 1) * fCellHeight), +fHalfDepth }, xmf4Color });
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),		-fHalfHeight + ((j + 1) * fCellHeight), +fHalfDepth }, xmf4Color });

			UINT lastVertexIndex = pMesh->m_xmf3Vertices.size();
			pMesh->m_uiIndices.push_back(Index{ lastVertexIndex - 4, lastVertexIndex - 3, lastVertexIndex - 2 });
			pMesh->m_uiIndices.push_back(Index{ lastVertexIndex - 4, lastVertexIndex - 2, lastVertexIndex - 1 });
		}
	}

	shared_ptr<struct Polygon> pBackFace;
	for (int i = 0; i < nSubRects; i++) {
		for (int j = 0; j < nSubRects; j++) {
			pBackFace = make_shared<struct Polygon>(4);
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),		-fHalfHeight + (j * fCellHeight),		-fHalfDepth }, xmf4Color });
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight + (j * fCellHeight),		-fHalfDepth }, xmf4Color });
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight + ((j + 1) * fCellHeight),	-fHalfDepth }, xmf4Color });
			pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),		-fHalfHeight + ((j + 1) * fCellHeight),	-fHalfDepth }, xmf4Color });

			UINT lastVertexIndex = pMesh->m_xmf3Vertices.size();
			pMesh->m_uiIndices.push_back(Index{ lastVertexIndex - 4, lastVertexIndex - 3, lastVertexIndex - 2 });
			pMesh->m_uiIndices.push_back(Index{ lastVertexIndex - 4, lastVertexIndex - 2, lastVertexIndex - 1 });
		}
	}


	pMesh->m_pd3dVertexBuffer = ::CreateBufferResources(pd3dDevice, pd3dCommandList, pMesh->m_xmf3Vertices.data(),
		pMesh->m_nStride * pMesh->m_xmf3Vertices.size(), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &pMesh->m_pd3dVertexUploadBuffer);

	pMesh->m_d3dVertexBufferView.BufferLocation = pMesh->m_pd3dVertexBuffer->GetGPUVirtualAddress();
	pMesh->m_d3dVertexBufferView.StrideInBytes = pMesh->m_nStride;
	pMesh->m_d3dVertexBufferView.SizeInBytes = pMesh->m_nStride * pMesh->m_xmf3Vertices.size();

	pMesh->m_pd3dIndexBuffer = ::CreateBufferResources(
		pd3dDevice,
		pd3dCommandList,
		pMesh->m_uiIndices.data(),
		sizeof(UINT) * (pMesh->m_uiIndices.size() * 3),
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&pMesh->m_pd3dIndexUploadBuffer
	);

	pMesh->m_d3dIndexBufferView.BufferLocation = pMesh->m_pd3dIndexBuffer->GetGPUVirtualAddress();
	pMesh->m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	pMesh->m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * (pMesh->m_uiIndices.size() * 3);



	pMesh->m_xmOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

BOOL MeshHelper::CreateMeshFromOBJFiles(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, 
	shared_ptr<Mesh<DiffusedVertex>> pMesh, wstring_view wstrObjPath, const XMFLOAT4& xmf4Color)
{
	ifstream in{ wstrObjPath.data() };

	if (!in) return FALSE;

	// 1. 파일에서 읽어옴
	vector<XMFLOAT3> LoadedVertices;
	vector<Index> LoadedIndices;
	std::string strRead{};
	while (in >> strRead) {
		if (strRead == "v") {
			float fX, fY, fZ;
			in >> fX >> fY >> fZ;
			LoadedVertices.emplace_back(fX, fY, fZ);
		}

		if (strRead == "f") {
			UINT n1, n2, n3;
			in >> n1 >> n2 >> n3;
			LoadedIndices.push_back(Index{ n1, n2, n3 });
		}
	}

	// 2. 읽어온 메쉬의 중심을 구함
	auto [itMinX, itMaxX] = std::minmax_element(LoadedVertices.begin(), LoadedVertices.end(), [](const XMFLOAT3& xmf3_1, const XMFLOAT3& xmf3_2) { return xmf3_1.x < xmf3_2.x; });
	auto [itMinY, itMaxY] = std::minmax_element(LoadedVertices.begin(), LoadedVertices.end(), [](const XMFLOAT3& xmf3_1, const XMFLOAT3& xmf3_2) { return xmf3_1.y < xmf3_2.y; });
	auto [itMinZ, itMaxZ] = std::minmax_element(LoadedVertices.begin(), LoadedVertices.end(), [](const XMFLOAT3& xmf3_1, const XMFLOAT3& xmf3_2) { return xmf3_1.z < xmf3_2.z; });

	XMFLOAT3 xmf3Center = { (itMaxX->x + itMinX->x) / 2, (itMaxY->y + itMinY->y) / 2, (itMaxZ->z + itMinZ->z) / 2 };

	// 3. 메쉬의 중심을 0,0,0 으로 옮김
	std::for_each(LoadedVertices.begin(), LoadedVertices.end(), [&xmf3Center](XMFLOAT3& xmf3Vertex) -> void {
		xmf3Vertex.x -= xmf3Center.x;
		xmf3Vertex.y -= xmf3Center.y;
		xmf3Vertex.z -= xmf3Center.z;
	});

	// 4. 다시 메쉬의 중심을 구함 -> 0,0,0 에서 float 오차 이상 벗어나지 않아야 함
	auto [itNewMinX, itNewMaxX] = std::minmax_element(LoadedVertices.begin(), LoadedVertices.end(), [](const XMFLOAT3& xmf3_1, const XMFLOAT3& xmf3_2) { return xmf3_1.x < xmf3_2.x; });
	auto [itNewMinY, itNewMaxY] = std::minmax_element(LoadedVertices.begin(), LoadedVertices.end(), [](const XMFLOAT3& xmf3_1, const XMFLOAT3& xmf3_2) { return xmf3_1.y < xmf3_2.y; });
	auto [itNewMinZ, itNewMaxZ] = std::minmax_element(LoadedVertices.begin(), LoadedVertices.end(), [](const XMFLOAT3& xmf3_1, const XMFLOAT3& xmf3_2) { return xmf3_1.z < xmf3_2.z; });

	XMFLOAT3 xmf3NewCenter = { (itNewMaxX->x + itNewMinX->x) / 2, (itNewMaxY->y + itNewMinY->y) / 2, (itNewMaxZ->z + itNewMinZ->z) / 2 };

	for (const XMFLOAT3& xmf3Vertex : LoadedVertices) {
		pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ xmf3Vertex, xmf4Color });
	}

	std::copy(LoadedIndices.begin(), LoadedIndices.end(), std::back_inserter(pMesh->m_uiIndices));


	pMesh->m_pd3dVertexBuffer = ::CreateBufferResources(pd3dDevice, pd3dCommandList, pMesh->m_xmf3Vertices.data(),
		pMesh->m_nStride * pMesh->m_xmf3Vertices.size(), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &pMesh->m_pd3dVertexUploadBuffer);

	pMesh->m_d3dVertexBufferView.BufferLocation = pMesh->m_pd3dVertexBuffer->GetGPUVirtualAddress();
	pMesh->m_d3dVertexBufferView.StrideInBytes = pMesh->m_nStride;
	pMesh->m_d3dVertexBufferView.SizeInBytes = pMesh->m_nStride * pMesh->m_xmf3Vertices.size();

	pMesh->m_pd3dIndexBuffer = ::CreateBufferResources(
		pd3dDevice,
		pd3dCommandList,
		pMesh->m_uiIndices.data(),
		sizeof(UINT) * (pMesh->m_uiIndices.size() * 3),
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&pMesh->m_pd3dIndexUploadBuffer
	);

	pMesh->m_d3dIndexBufferView.BufferLocation = pMesh->m_pd3dIndexBuffer->GetGPUVirtualAddress();
	pMesh->m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	pMesh->m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * (pMesh->m_uiIndices.size() * 3);

	XMFLOAT3 xmf3ObbExtent = Vector3::Subtract(XMFLOAT3{ itNewMaxX->x, itNewMaxY->y, itNewMaxZ->z }, xmf3NewCenter);
	pMesh->m_xmOBB = BoundingOrientedBox(xmf3NewCenter, xmf3ObbExtent, XMFLOAT4{ 0.f, 0.f, 0.f, 1.f });

}

void GenerateRollercoasterPillarPolygon(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, 
	shared_ptr<Mesh<DiffusedVertex>> pMesh, XMFLOAT3 xmf3TopPosition, float fWidth, float fDepth, const XMFLOAT4& xmf4Color)
{
	XMFLOAT3 xmf3PillarCenter;
	XMStoreFloat3(&xmf3PillarCenter, XMVectorMultiply(XMLoadFloat3(&xmf3TopPosition), XMVectorSet(1.0f, 0.5f, 1.0f, 0.0f)));

	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = xmf3PillarCenter.y;
	float fHalfDepth = fDepth * 0.5f;

	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z - fHalfDepth }, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z - fHalfDepth }, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z - fHalfDepth }, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z - fHalfDepth }, xmf4Color });

	UINT uiLastVertexIdx = pMesh->m_xmf3Vertices.size();
	pMesh->m_uiIndices.push_back(Index{ uiLastVertexIdx - 4, uiLastVertexIdx - 3, uiLastVertexIdx - 1 });
	pMesh->m_uiIndices.push_back(Index{ uiLastVertexIdx - 3, uiLastVertexIdx - 2, uiLastVertexIdx - 1 });


	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z + fHalfDepth}, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z + fHalfDepth}, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z - fHalfDepth}, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z - fHalfDepth}, xmf4Color });

	uiLastVertexIdx = pMesh->m_xmf3Vertices.size();
	pMesh->m_uiIndices.push_back(Index{ uiLastVertexIdx - 4, uiLastVertexIdx - 3, uiLastVertexIdx - 1 });
	pMesh->m_uiIndices.push_back(Index{ uiLastVertexIdx - 3, uiLastVertexIdx - 2, uiLastVertexIdx - 1 });

	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z + fHalfDepth}, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z + fHalfDepth}, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z + fHalfDepth}, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z + fHalfDepth}, xmf4Color });

	uiLastVertexIdx = pMesh->m_xmf3Vertices.size();
	pMesh->m_uiIndices.push_back(Index{ uiLastVertexIdx - 4, uiLastVertexIdx - 3, uiLastVertexIdx - 1 });
	pMesh->m_uiIndices.push_back(Index{ uiLastVertexIdx - 3, uiLastVertexIdx - 2, uiLastVertexIdx - 1 });

	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z - fHalfDepth}, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z - fHalfDepth}, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z + fHalfDepth}, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z + fHalfDepth}, xmf4Color });
	
	uiLastVertexIdx = pMesh->m_xmf3Vertices.size();
	pMesh->m_uiIndices.push_back(Index{ uiLastVertexIdx - 4, uiLastVertexIdx - 3, uiLastVertexIdx - 1 });
	pMesh->m_uiIndices.push_back(Index{ uiLastVertexIdx - 3, uiLastVertexIdx - 2, uiLastVertexIdx - 1 });

	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z + fHalfDepth}, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z - fHalfDepth}, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z - fHalfDepth}, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z + fHalfDepth}, xmf4Color });

	uiLastVertexIdx = pMesh->m_xmf3Vertices.size();
	pMesh->m_uiIndices.push_back(Index{ uiLastVertexIdx - 4, uiLastVertexIdx - 3, uiLastVertexIdx - 1 });
	pMesh->m_uiIndices.push_back(Index{ uiLastVertexIdx - 3, uiLastVertexIdx - 2, uiLastVertexIdx - 1 });

	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z - fHalfDepth}, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z + fHalfDepth}, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z + fHalfDepth}, xmf4Color });
	pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ XMFLOAT3{ xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z - fHalfDepth}, xmf4Color });

	uiLastVertexIdx = pMesh->m_xmf3Vertices.size();
	pMesh->m_uiIndices.push_back(Index{ uiLastVertexIdx - 4, uiLastVertexIdx - 3, uiLastVertexIdx - 1 });
	pMesh->m_uiIndices.push_back(Index{ uiLastVertexIdx - 3, uiLastVertexIdx - 2, uiLastVertexIdx - 1 });

}

void MeshHelper::CreateRollercoasterRailMesh(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, 
	shared_ptr<Mesh<DiffusedVertex>> pMesh, OUT std::vector<XMFLOAT3>& RollercoasterRoute, float fWidth, float fCourseRadius, int nControlPoints, int nInterpolateBias, const XMFLOAT4& xmf4Color)
{
	assert(nControlPoints != 0);

	vector<XMFLOAT3> ControlPoints(nControlPoints);
	vector<XMFLOAT3> Tangents(nControlPoints);
	
	// 1. 컨트롤 포인트 생성
	auto GenerateControlPoint = [fCourseRadius, nControlPoints](int idx) {
		XMFLOAT3 v;
		v.x = fCourseRadius * XMScalarCos(XMConvertToRadians((360.0f / nControlPoints) * idx));
		v.y = RandomGenerator::GenerateRandomFloatInRange(5.0f, 150.0f);
		v.z = fCourseRadius * XMScalarSin(XMConvertToRadians((360.0f / nControlPoints) * idx));
		return v;
	};


	for (int i = 0; i < nControlPoints; ++i) {
		ControlPoints[i] = GenerateControlPoint(i);
	}


	// 2. 접선
	XMStoreFloat3(&Tangents[0], XMVectorZero());
	XMStoreFloat3(&Tangents[nControlPoints - 1], XMVectorZero());

	int count = 1;
	for (auto& [v1, v2, v3] : ControlPoints | views::adjacent<3>) {
		XMVECTOR xmvCP1 = XMLoadFloat3(&v1);
		XMVECTOR xmvCP3 = XMLoadFloat3(&v3);

		XMStoreFloat3(&Tangents[count++], XMVectorScale(XMVectorSubtract(xmvCP3, xmvCP1), 0.5f));
	}

	// 시작점과 끝점의 접선
	// 시작점
	{
		XMVECTOR xmvCP1 = XMLoadFloat3(&ControlPoints[nControlPoints - 1]);
		XMVECTOR xmvCP3 = XMLoadFloat3(&ControlPoints[1]);

		XMStoreFloat3(&Tangents[0], XMVectorScale(XMVectorSubtract(xmvCP3, xmvCP1), 0.5f));
	}

	// 끝점
	{
		XMVECTOR xmvCP1 = XMLoadFloat3(&ControlPoints[nControlPoints - 2]);
		XMVECTOR xmvCP3 = XMLoadFloat3(&ControlPoints[0]);

		XMStoreFloat3(&Tangents[nControlPoints - 1], XMVectorScale(XMVectorSubtract(xmvCP3, xmvCP1), 0.5f));
	}

	// 3. 스플라인 곡선의 점 구함

	vector<XMFLOAT3> SplinePoints;

	for (int i = 0; i < nControlPoints - 1; ++i) {
		XMVECTOR xmvControlPoint1 = XMLoadFloat3(&ControlPoints[i]);
		XMVECTOR xmvControlPoint2 = XMLoadFloat3(&ControlPoints[i + 1]);
		XMVECTOR xmvTangent1 = XMLoadFloat3(&Tangents[i]);
		XMVECTOR xmvTangent2 = XMLoadFloat3(&Tangents[i + 1]);

		for (int j = 0; j < nInterpolateBias; j++) {
			float t = (float)j / nInterpolateBias;
			XMVECTOR xmvPoint = XMVectorHermite(xmvControlPoint1, xmvTangent1, xmvControlPoint2, xmvTangent2, t);
			XMFLOAT3 xmf3Point;
			XMStoreFloat3(&xmf3Point, xmvPoint);
			SplinePoints.push_back(xmf3Point);
		}
	}

	// 마지막과 시작점의 점을 추가
	{
		XMVECTOR xmvControlPoint1 = XMLoadFloat3(&ControlPoints.back());
		XMVECTOR xmvControlPoint2 = XMLoadFloat3(&ControlPoints.front());
		XMVECTOR xmvTangent1 = XMLoadFloat3(&Tangents.back());
		XMVECTOR xmvTangent2 = XMLoadFloat3(&Tangents.front());

		for (int j = 0; j <= nInterpolateBias; j++) {
			float t = (float)j / nInterpolateBias;
			XMVECTOR xmvPoint = XMVectorHermite(xmvControlPoint1, xmvTangent1, xmvControlPoint2, xmvTangent2, t);
			XMFLOAT3 xmf3Point;
			XMStoreFloat3(&xmf3Point, xmvPoint);
			SplinePoints.push_back(xmf3Point);
		}
	}

	/*
		SP : SplinePoints
		         SP[0]                      SP[1]                      SP[2]        
		  ---------+--------------------------+--------------------------+----------
		           |0                        3|0                        3|
				   |1                        2|1                        2|
		  ---------+--------------------------+--------------------------+----------



		  v0 ? ----------- ? v3
			 |             |
		p[i] ? ----------- ? p[i+1]
			 |             |
		  v1 ? ----------- ? v2

		  v0, v1 : normalize(cross(p[i] - p[i-1]), up)) 하여 방향을 찾고 너비만큼의 간격으로 점을 찾으면 됨 (+- 방향 * (width / 2))
		  v2, v3 : normalize(cross(p[i+1] - p[i]), up)) 하여 방향을 찾고 적절한 너비만큼의 간격으로 점을 찾으면 됨 (+- 방향 * (width / 2))

	*/

	// 4. 레일
	vector<shared_ptr<struct Polygon>> pPolygons;
	for (int i = 1; i < SplinePoints.size() - 1; ++i) {
		shared_ptr<struct Polygon> pRail = make_shared<struct Polygon>(4);
		XMVECTOR xmvCurPoint = XMLoadFloat3(&SplinePoints[i]);
		XMVECTOR xmvNextPoint = XMLoadFloat3(&SplinePoints[i + 1]);
		XMVECTOR xmvPrevPoint = XMLoadFloat3(&SplinePoints[i - 1]);

		// 현재 점 : 우 -> 좌 
		XMVECTOR xmvCurRailDirection = XMVectorSubtract(xmvCurPoint, xmvPrevPoint);
		XMVECTOR xmvCurRailUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		XMVECTOR xmvCurRailRight = XMVector3Normalize(XMVector3Cross(xmvCurRailDirection, xmvCurRailUp));
		XMVECTOR xmvCurRailLeft = XMVector3Normalize(XMVectorScale(xmvCurRailRight, -1.f));

		XMVECTOR xmvVertex1 = XMVector3TransformCoord(xmvCurPoint, XMMatrixTranslationFromVector(XMVectorScale(xmvCurRailLeft, fWidth / 2)));
		XMVECTOR xmvVertex2 = XMVector3TransformCoord(xmvCurPoint, XMMatrixTranslationFromVector(XMVectorScale(xmvCurRailRight, fWidth / 2)));

		// 다음 점 : 우 -> 좌 
		XMVECTOR xmvNextRailDirection = XMVectorSubtract(xmvNextPoint, xmvCurPoint);
		XMVECTOR xmvNextRailUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		XMVECTOR xmvNextRailRight = XMVector3Normalize(XMVector3Cross(xmvNextRailDirection, xmvNextRailUp));
		XMVECTOR xmvNextRailLeft = XMVector3Normalize(XMVectorScale(xmvNextRailRight, -1.f));

		XMVECTOR xmvVertex3 = XMVector3TransformCoord(xmvNextPoint, XMMatrixTranslationFromVector(XMVectorScale(xmvNextRailRight, fWidth / 2)));
		XMVECTOR xmvVertex4 = XMVector3TransformCoord(xmvNextPoint, XMMatrixTranslationFromVector(XMVectorScale(xmvNextRailLeft, fWidth / 2)));

		XMFLOAT3 xmf3Vertex1;
		XMFLOAT3 xmf3Vertex2;
		XMFLOAT3 xmf3Vertex3;
		XMFLOAT3 xmf3Vertex4;
		XMStoreFloat3(&xmf3Vertex1, xmvVertex1);
		XMStoreFloat3(&xmf3Vertex2, xmvVertex2);
		XMStoreFloat3(&xmf3Vertex3, xmvVertex3);
		XMStoreFloat3(&xmf3Vertex4, xmvVertex4);

		pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ xmf3Vertex1, xmf4Color });
		pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ xmf3Vertex2, xmf4Color });
		pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ xmf3Vertex3, xmf4Color });
		pMesh->m_xmf3Vertices.push_back(DiffusedVertex{ xmf3Vertex4, xmf4Color });

		UINT uiLastVertexIdx = pMesh->m_xmf3Vertices.size();
		pMesh->m_uiIndices.push_back(Index{ uiLastVertexIdx - 3, uiLastVertexIdx - 4, uiLastVertexIdx - 1 });
		pMesh->m_uiIndices.push_back(Index{ uiLastVertexIdx - 3, uiLastVertexIdx - 1, uiLastVertexIdx - 2 });
	}

	// 5. 기둥
	for (int i = 1; i < nControlPoints; ++i) {
		int nPillarIndex = i * nInterpolateBias * 2;
		GenerateRollercoasterPillarPolygon(pd3dDevice, pd3dCommandList, pMesh, pMesh->m_xmf3Vertices[pMesh->m_uiIndices[nPillarIndex][0]].m_xmf3Position, 0.5f, 0.5f, xmf4Color);
		GenerateRollercoasterPillarPolygon(pd3dDevice, pd3dCommandList, pMesh, pMesh->m_xmf3Vertices[pMesh->m_uiIndices[nPillarIndex][1]].m_xmf3Position, 0.5f, 0.5f, xmf4Color);
	}


	pMesh->m_pd3dVertexBuffer = ::CreateBufferResources(pd3dDevice, pd3dCommandList, pMesh->m_xmf3Vertices.data(),
		pMesh->m_nStride * pMesh->m_xmf3Vertices.size(), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &pMesh->m_pd3dVertexUploadBuffer);

	pMesh->m_d3dVertexBufferView.BufferLocation = pMesh->m_pd3dVertexBuffer->GetGPUVirtualAddress();
	pMesh->m_d3dVertexBufferView.StrideInBytes = pMesh->m_nStride;
	pMesh->m_d3dVertexBufferView.SizeInBytes = pMesh->m_nStride * pMesh->m_xmf3Vertices.size();

	pMesh->m_pd3dIndexBuffer = ::CreateBufferResources(
		pd3dDevice,
		pd3dCommandList,
		pMesh->m_uiIndices.data(),
		sizeof(UINT) * (pMesh->m_uiIndices.size() * 3),
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&pMesh->m_pd3dIndexUploadBuffer
	);

	pMesh->m_d3dIndexBufferView.BufferLocation = pMesh->m_pd3dIndexBuffer->GetGPUVirtualAddress();
	pMesh->m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	pMesh->m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * (pMesh->m_uiIndices.size() * 3);



	pMesh->m_xmOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fCourseRadius, fCourseRadius, fCourseRadius), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	RollercoasterRoute.assign(SplinePoints.begin(), SplinePoints.end());

}
