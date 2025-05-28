#include "stdafx.h"
#include "MeshHelper.h"
#include "VertexType.h"
#include "Mesh.h"
#include "RandomGenerator.h"
#include <fstream>
#include <ranges>

void MeshHelper::CreateCubeMesh(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, 
	std::shared_ptr<Mesh<DiffusedVertex>> pMesh, float fWidth, float fHeight, float fDepth, const XMFLOAT4& xmf4Color)
{
	std::vector<DiffusedVertex> vertices;
	std::vector<UINT> indices;

	// fWidth	-> x-axis length
	// fHeight	-> y-axis length
	// fDepth	-> z-axis length

	float fx = fWidth * 0.5f;
	float fy = fHeight * 0.5f;
	float fz = fDepth * 0.5f;

	// Vertex
	vertices.resize(8);
	vertices[0] = DiffusedVertex(XMFLOAT3(-fx, +fy, -fz), xmf4Color);
	vertices[1] = DiffusedVertex(XMFLOAT3(+fx, +fy, -fz), xmf4Color);
	vertices[2] = DiffusedVertex(XMFLOAT3(+fx, +fy, +fz), xmf4Color);
	vertices[3] = DiffusedVertex(XMFLOAT3(-fx, +fy, +fz), xmf4Color);
	vertices[4] = DiffusedVertex(XMFLOAT3(-fx, -fy, -fz), xmf4Color);
	vertices[5] = DiffusedVertex(XMFLOAT3(+fx, -fy, -fz), xmf4Color);
	vertices[6] = DiffusedVertex(XMFLOAT3(+fx, -fy, +fz), xmf4Color);
	vertices[7] = DiffusedVertex(XMFLOAT3(-fx, -fy, +fz), xmf4Color);

	indices.resize(36);

	// Front
	indices[0] = 3; indices[1] = 1; indices[2] = 0;
	indices[3] = 2; indices[4] = 1; indices[5] = 3;

	// Top
	indices[6] = 0; indices[7] = 5; indices[8] = 4;
	indices[9] = 1; indices[10] = 5; indices[11] = 0;

	// Back
	indices[12] = 3; indices[13] = 4; indices[14] = 7;
	indices[15] = 0; indices[16] = 4; indices[17] = 3;

	// Bottom
	indices[18] = 1; indices[19] = 6; indices[20] = 5;
	indices[21] = 2; indices[22] = 6; indices[23] = 1;

	// Left
	indices[24] = 2; indices[25] = 7; indices[26] = 6;
	indices[27] = 3; indices[28] = 7; indices[29] = 2;

	// Right
	indices[30] = 6; indices[31] = 4; indices[32] = 5;
	indices[33] = 7; indices[34] = 4; indices[35] = 6;


	pMesh->SetVertices(vertices);
	pMesh->SetIndices(indices);
	pMesh->SetStride(sizeof(DiffusedVertex));

	pMesh->Create(pd3dDevice, pd3dCommandList);

	pMesh->SetOBB(BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fx, fy, fz), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)));
}

void MeshHelper::CreateWallMesh(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, 
	std::shared_ptr<Mesh<DiffusedVertex>> pMesh, float fWidth, float fHeight, float fDepth, int nSubRects, const XMFLOAT4& xmf4Color)
{
	std::vector<DiffusedVertex> vertices;
	std::vector<UINT> indices;

	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;
	float fCellWidth = fWidth * (1.0f / nSubRects);
	float fCellHeight = fHeight * (1.0f / nSubRects);
	float fCellDepth = fDepth * (1.0f / nSubRects);

	int k = 0;

	// Left
	for (int i = 0; i < nSubRects; i++){
		for (int j = 0; j < nSubRects; j++){
			UINT indexStart = vertices.size();

			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth, -fHalfHeight + (i * fCellHeight),			-fHalfDepth + (j * fCellDepth) },RandomGenerator::GenerateRandomColor() });			// 0
			vertices.push_back(DiffusedVertex{ XMFLOAT3{-fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight),	-fHalfDepth + (j * fCellDepth) }, RandomGenerator::GenerateRandomColor() });			// 1
			vertices.push_back(DiffusedVertex{ XMFLOAT3{-fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight),	-fHalfDepth + ((j + 1) * fCellDepth) }, RandomGenerator::GenerateRandomColor() });	// 2
			vertices.push_back(DiffusedVertex{ XMFLOAT3{-fHalfWidth, -fHalfHeight + (i * fCellHeight),			-fHalfDepth + ((j + 1) * fCellDepth) }, RandomGenerator::GenerateRandomColor() });	// 3
			// 3 -> 1 -> 0
			// 2 -> 1 -> 3

			indices.push_back(indexStart + 3);
			indices.push_back(indexStart + 0);
			indices.push_back(indexStart + 1);

			indices.push_back(indexStart + 2);
			indices.push_back(indexStart + 3);
			indices.push_back(indexStart + 1);
		}
	}

	// Right
	for (int i = 0; i < nSubRects; i++){
		for (int j = 0; j < nSubRects; j++){
			UINT indexStart = vertices.size();

			vertices.push_back(DiffusedVertex{ XMFLOAT3{ +fHalfWidth, -fHalfHeight + (i * fCellHeight),			-fHalfDepth + (j * fCellDepth) }, RandomGenerator::GenerateRandomColor()});
			vertices.push_back(DiffusedVertex{ XMFLOAT3{ +fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight),	-fHalfDepth + (j * fCellDepth) }, RandomGenerator::GenerateRandomColor() });
			vertices.push_back(DiffusedVertex{ XMFLOAT3{ +fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight),	-fHalfDepth + ((j + 1) * fCellDepth) }, RandomGenerator::GenerateRandomColor() });
			vertices.push_back(DiffusedVertex{ XMFLOAT3{ +fHalfWidth, -fHalfHeight + (i * fCellHeight),			-fHalfDepth + ((j + 1) * fCellDepth) }, RandomGenerator::GenerateRandomColor() });

			indices.push_back(indexStart + 3);
			indices.push_back(indexStart + 1);
			indices.push_back(indexStart + 0);

			indices.push_back(indexStart + 2);
			indices.push_back(indexStart + 1);
			indices.push_back(indexStart + 3);
		}
	}

	// Top
	for (int i = 0; i < nSubRects; i++){
		for (int j = 0; j < nSubRects; j++){
			UINT indexStart = vertices.size();

			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),		+fHalfHeight,	-fHalfDepth + (j * fCellDepth) }, RandomGenerator::GenerateRandomColor() });
			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),	+fHalfHeight,	-fHalfDepth + (j * fCellDepth) }, RandomGenerator::GenerateRandomColor() });
			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),	+fHalfHeight,	-fHalfDepth + ((j + 1) * fCellDepth) }, RandomGenerator::GenerateRandomColor()});
			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),		+fHalfHeight,	-fHalfDepth + ((j + 1) * fCellDepth) }, RandomGenerator::GenerateRandomColor()});

			indices.push_back(indexStart + 3);
			indices.push_back(indexStart + 1);
			indices.push_back(indexStart + 0);

			indices.push_back(indexStart + 2);
			indices.push_back(indexStart + 1);
			indices.push_back(indexStart + 3);
		}
	}

	// Bottom
	for (int i = 0; i < nSubRects; i++){
		for (int j = 0; j < nSubRects; j++){
			UINT indexStart = vertices.size();

			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),		-fHalfHeight,	-fHalfDepth + (j * fCellDepth) }, RandomGenerator::GenerateRandomColor() });
			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight,	-fHalfDepth + (j * fCellDepth) }, RandomGenerator::GenerateRandomColor() });
			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight,	-fHalfDepth + ((j + 1) * fCellDepth) }, RandomGenerator::GenerateRandomColor() });
			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),		-fHalfHeight,	-fHalfDepth + ((j + 1) * fCellDepth) }, RandomGenerator::GenerateRandomColor() });

			indices.push_back(indexStart + 3);
			indices.push_back(indexStart + 1);
			indices.push_back(indexStart + 0);

			indices.push_back(indexStart + 2);
			indices.push_back(indexStart + 1);
			indices.push_back(indexStart + 3);
		}
	}

	// Front
	for (int i = 0; i < nSubRects; i++) {
		for (int j = 0; j < nSubRects; j++) {
			UINT indexStart = vertices.size();

			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),		-fHalfHeight + (j * fCellHeight),		+fHalfDepth }, RandomGenerator::GenerateRandomColor() });
			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight + (j * fCellHeight),		+fHalfDepth }, RandomGenerator::GenerateRandomColor() });
			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight + ((j + 1) * fCellHeight), +fHalfDepth }, RandomGenerator::GenerateRandomColor() });
			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),		-fHalfHeight + ((j + 1) * fCellHeight), +fHalfDepth }, RandomGenerator::GenerateRandomColor() });

			indices.push_back(indexStart + 3);
			indices.push_back(indexStart + 1);
			indices.push_back(indexStart + 0);

			indices.push_back(indexStart + 2);
			indices.push_back(indexStart + 1);
			indices.push_back(indexStart + 3);
		}
	}

	// Back
	for (int i = 0; i < nSubRects; i++) {
		for (int j = 0; j < nSubRects; j++) {
			UINT indexStart = vertices.size();

			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),		-fHalfHeight + (j * fCellHeight),		-fHalfDepth }, RandomGenerator::GenerateRandomColor() });
			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight + (j * fCellHeight),		-fHalfDepth }, RandomGenerator::GenerateRandomColor() });
			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight + ((j + 1) * fCellHeight),	-fHalfDepth }, RandomGenerator::GenerateRandomColor() });
			vertices.push_back(DiffusedVertex{ XMFLOAT3{ -fHalfWidth + (i * fCellWidth),		-fHalfHeight + ((j + 1) * fCellHeight),	-fHalfDepth }, RandomGenerator::GenerateRandomColor() });

			indices.push_back(indexStart + 3);
			indices.push_back(indexStart + 0);
			indices.push_back(indexStart + 1);

			indices.push_back(indexStart + 2);
			indices.push_back(indexStart + 3);
			indices.push_back(indexStart + 1);
		}
	}


	pMesh->SetVertices(vertices);
	pMesh->SetIndices(indices);

	pMesh->SetStride(sizeof(DiffusedVertex));
	pMesh->Create(pd3dDevice, pd3dCommandList);



	pMesh->SetOBB(BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)));
}

BOOL MeshHelper::CreateMeshFromOBJFiles(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, 
	std::shared_ptr<Mesh<DiffusedVertex>> pMesh, std::wstring_view wstrObjPath, const XMFLOAT4& xmf4Color)
{
	std::ifstream in{ wstrObjPath.data() };

	if (!in) return FALSE;

	// 1. 파일에서 읽어옴
	std::vector<XMFLOAT3> LoadedVertices;
	std::vector<UINT> LoadedIndices;

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
			LoadedIndices.push_back(n1 - 1);
			LoadedIndices.push_back(n2 - 1);
			LoadedIndices.push_back(n3 - 1);
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


	std::vector<DiffusedVertex> vertices;
	std::vector<UINT> indices;

	vertices.reserve(LoadedVertices.size());
	for (const XMFLOAT3& xmf3Vertex : LoadedVertices) {
		vertices.push_back(DiffusedVertex{ xmf3Vertex, xmf4Color });
	}

	indices.reserve(LoadedIndices.size());
	std::copy(LoadedIndices.begin(), LoadedIndices.end(), std::back_inserter(indices));


	pMesh->SetVertices(vertices);
	pMesh->SetIndices(indices);

	pMesh->SetStride(sizeof(DiffusedVertex));
	pMesh->Create(pd3dDevice, pd3dCommandList);

	XMFLOAT3 xmf3ObbExtent = Vector3::Subtract(XMFLOAT3{ itNewMaxX->x, itNewMaxY->y, itNewMaxZ->z }, xmf3NewCenter);
	pMesh->SetOBB(BoundingOrientedBox(xmf3NewCenter, xmf3ObbExtent, XMFLOAT4{ 0.f, 0.f, 0.f, 1.f }));
}

void GenerateRollercoasterPillarPolygon(std::vector<DiffusedVertex>& vertices, std::vector<UINT>& indices, XMFLOAT3 xmf3TopPosition, float fWidth, float fDepth, const XMFLOAT4& xmf4Color)
{
	XMFLOAT3 xmf3PillarCenter;
	XMStoreFloat3(&xmf3PillarCenter, XMVectorMultiply(XMLoadFloat3(&xmf3TopPosition), XMVectorSet(1.0f, 0.5f, 1.0f, 0.0f)));

	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = xmf3PillarCenter.y;
	float fHalfDepth = fDepth * 0.5f;
	{
		UINT indexStart = vertices.size();

		vertices.push_back(DiffusedVertex(XMFLOAT3(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z - fHalfDepth), RandomGenerator::GenerateRandomColor()));
		vertices.push_back(DiffusedVertex(XMFLOAT3(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z - fHalfDepth), RandomGenerator::GenerateRandomColor()));
		vertices.push_back(DiffusedVertex(XMFLOAT3(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z + fHalfDepth), RandomGenerator::GenerateRandomColor()));
		vertices.push_back(DiffusedVertex(XMFLOAT3(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z + fHalfDepth), RandomGenerator::GenerateRandomColor()));
		vertices.push_back(DiffusedVertex(XMFLOAT3(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z - fHalfDepth), RandomGenerator::GenerateRandomColor()));
		vertices.push_back(DiffusedVertex(XMFLOAT3(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z - fHalfDepth), RandomGenerator::GenerateRandomColor()));
		vertices.push_back(DiffusedVertex(XMFLOAT3(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z + fHalfDepth), RandomGenerator::GenerateRandomColor()));
		vertices.push_back(DiffusedVertex(XMFLOAT3(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z + fHalfDepth), RandomGenerator::GenerateRandomColor()));


		// Front
		indices.push_back(indexStart + 3); 
		indices.push_back(indexStart + 1); 
		indices.push_back(indexStart + 0);
		indices.push_back(indexStart + 2); 
		indices.push_back(indexStart + 1); 
		indices.push_back(indexStart + 3);

		// Top
		indices.push_back(indexStart + 0); 
		indices.push_back(indexStart + 5); 
		indices.push_back(indexStart + 4);
		indices.push_back(indexStart + 1); 
		indices.push_back(indexStart + 5);
		indices.push_back(indexStart + 0);

		// Back
		indices.push_back(indexStart + 3); 
		indices.push_back(indexStart + 4); 
		indices.push_back(indexStart + 7);
		indices.push_back(indexStart + 0); 
		indices.push_back(indexStart + 4); 
		indices.push_back(indexStart + 3);

		// Bottom
		indices.push_back(indexStart + 1); 
		indices.push_back(indexStart + 6); 
		indices.push_back(indexStart + 5);
		indices.push_back(indexStart + 2);
		indices.push_back(indexStart + 6); 
		indices.push_back(indexStart + 1);

		// Left
		indices.push_back(indexStart + 2); 
		indices.push_back(indexStart + 7); 
		indices.push_back(indexStart + 6);
		indices.push_back(indexStart + 3); 
		indices.push_back(indexStart + 7); 
		indices.push_back(indexStart + 2);

		// Right
		indices.push_back(indexStart + 6); 
		indices.push_back(indexStart + 4); 
		indices.push_back(indexStart + 5);
		indices.push_back(indexStart + 7); 
		indices.push_back(indexStart + 4); 
		indices.push_back(indexStart + 6);
	}
}

void GenerateRollercoasterRailCube(std::vector<DiffusedVertex>& vertices, std::vector<UINT>& indices,const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2, const XMFLOAT3& v3)
{
	/*
			  v1 ? ----------- ? v3
				 |             |
			p[i] ? ----------- ? p[i+1]
				 |             |
			  v0 ? ----------- ? v2
	
               v1                 v3
			   + ---------------- +
			  /|                / |
		  v0 + +---------------+ v2
             | |               |  |
			 | |               |  |
	       bv1 + ------------- +- +  bv3
			 |/                | / 
             + ----------------+
		  bv0                 bv2

	*/


	float fRailThickness = 3.f;

	XMFLOAT3 xmf3BottomVertex0;
	XMFLOAT3 xmf3BottomVertex1;
	XMFLOAT3 xmf3BottomVertex2;
	XMFLOAT3 xmf3BottomVertex3;

	XMStoreFloat3(&xmf3BottomVertex0, XMVector3TransformCoord(XMLoadFloat3(&v0), XMMatrixTranslationFromVector(XMVectorScale(XMVectorSet(0.f, -1.f, 0.f, 1.f), fRailThickness))));
	XMStoreFloat3(&xmf3BottomVertex1, XMVector3TransformCoord(XMLoadFloat3(&v1), XMMatrixTranslationFromVector(XMVectorScale(XMVectorSet(0.f, -1.f, 0.f, 1.f), fRailThickness))));
	XMStoreFloat3(&xmf3BottomVertex2, XMVector3TransformCoord(XMLoadFloat3(&v2), XMMatrixTranslationFromVector(XMVectorScale(XMVectorSet(0.f, -1.f, 0.f, 1.f), fRailThickness))));
	XMStoreFloat3(&xmf3BottomVertex3, XMVector3TransformCoord(XMLoadFloat3(&v3), XMMatrixTranslationFromVector(XMVectorScale(XMVectorSet(0.f, -1.f, 0.f, 1.f), fRailThickness))));

	UINT indexStart = vertices.size();
	vertices.push_back(DiffusedVertex{ v0, RandomGenerator::GenerateRandomColor() });
	vertices.push_back(DiffusedVertex{ v1, RandomGenerator::GenerateRandomColor() });
	vertices.push_back(DiffusedVertex{ v2, RandomGenerator::GenerateRandomColor() });
	vertices.push_back(DiffusedVertex{ v3, RandomGenerator::GenerateRandomColor() });
	vertices.push_back(DiffusedVertex{ xmf3BottomVertex0, RandomGenerator::GenerateRandomColor() });
	vertices.push_back(DiffusedVertex{ xmf3BottomVertex1, RandomGenerator::GenerateRandomColor() });
	vertices.push_back(DiffusedVertex{ xmf3BottomVertex2, RandomGenerator::GenerateRandomColor() });
	vertices.push_back(DiffusedVertex{ xmf3BottomVertex3, RandomGenerator::GenerateRandomColor() });

	// Front
	indices.push_back(indexStart + 3);
	indices.push_back(indexStart + 1);
	indices.push_back(indexStart + 0);
	indices.push_back(indexStart + 2);
	indices.push_back(indexStart + 1);
	indices.push_back(indexStart + 3);

	// Top
	indices.push_back(indexStart + 0);
	indices.push_back(indexStart + 5);
	indices.push_back(indexStart + 4);
	indices.push_back(indexStart + 1);
	indices.push_back(indexStart + 5);
	indices.push_back(indexStart + 0);

	// Back
	indices.push_back(indexStart + 3);
	indices.push_back(indexStart + 4);
	indices.push_back(indexStart + 7);
	indices.push_back(indexStart + 0);
	indices.push_back(indexStart + 4);
	indices.push_back(indexStart + 3);

	// Bottom
	indices.push_back(indexStart + 1);
	indices.push_back(indexStart + 6);
	indices.push_back(indexStart + 5);
	indices.push_back(indexStart + 2);
	indices.push_back(indexStart + 6);
	indices.push_back(indexStart + 1);

	// Left
	indices.push_back(indexStart + 2);
	indices.push_back(indexStart + 7);
	indices.push_back(indexStart + 6);
	indices.push_back(indexStart + 3);
	indices.push_back(indexStart + 7);
	indices.push_back(indexStart + 2);

	// Right
	indices.push_back(indexStart + 6);
	indices.push_back(indexStart + 4);
	indices.push_back(indexStart + 5);
	indices.push_back(indexStart + 7);
	indices.push_back(indexStart + 4);
	indices.push_back(indexStart + 6);

}

void MeshHelper::CreateRollercoasterRailMesh(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, 
	std::shared_ptr<Mesh<DiffusedVertex>> pMesh, OUT std::vector<XMFLOAT3>& RollercoasterRoute, float fWidth, float fCourseRadius, int nControlPoints, int nInterpolateBias, const XMFLOAT4& xmf4Color)
{
	assert(nControlPoints != 0);

	std::vector<XMFLOAT3> ControlPoints(nControlPoints);
	std::vector<XMFLOAT3> Tangents(nControlPoints);
	
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
	for (auto&& [v1, v2, v3] : ControlPoints | std::views::adjacent<3>) {
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

	std::vector<XMFLOAT3> SplinePoints;

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
		           |1                        3|0                        3|
				   |0                        2|1                        2|
		  ---------+--------------------------+--------------------------+----------


				                   v1            v3
				     ? ----------- ? ----------- ?
					 |             |             |
			p[i-1]   ? ----------- ? p[i]	     ? p[i+1]
					 |             |		     |
				     ? ----------- ? ----------- ?
					               v0            v2
				  v0, v1 : normalize(cross(p[i] - p[i-1]), up)) 하여 방향을 찾고 너비만큼의 간격으로 점을 찾으면 됨 (+- 방향 * (width / 2))
				  v2, v3 : normalize(cross(p[i+1] - p[i]), up)) 하여 방향을 찾고 적절한 너비만큼의 간격으로 점을 찾으면 됨 (+- 방향 * (width / 2))

	*/

	// 4. 레일
	std::vector<DiffusedVertex> vertices{};
	std::vector<UINT> indices{};

	for (int i = 1; i < SplinePoints.size() - 1; ++i) {
		XMVECTOR xmvCurPoint = XMLoadFloat3(&SplinePoints[i]);
		XMVECTOR xmvNextPoint = XMLoadFloat3(&SplinePoints[i + 1]);
		XMVECTOR xmvPrevPoint = XMLoadFloat3(&SplinePoints[i - 1]);

		// 현재 점 : 우 -> 좌 
		XMVECTOR xmvCurRailDirection = XMVector3Normalize(XMVectorSubtract(xmvCurPoint, xmvPrevPoint));
		XMVECTOR xmvCurRailUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		XMVECTOR xmvCurRailRight = XMVector3Normalize(XMVector3Cross(xmvCurRailUp, xmvCurRailDirection));
		XMVECTOR xmvCurRailLeft = XMVector3Normalize(XMVectorScale(xmvCurRailRight, -1.f));

		XMVECTOR xmvVertex1 = XMVector3TransformCoord(xmvCurPoint, XMMatrixTranslationFromVector(XMVectorScale(xmvCurRailLeft, fWidth / 2)));
		XMVECTOR xmvVertex2 = XMVector3TransformCoord(xmvCurPoint, XMMatrixTranslationFromVector(XMVectorScale(xmvCurRailRight, fWidth / 2)));

		// 다음 점 : 우 -> 좌 
		XMVECTOR xmvNextRailDirection = XMVector3Normalize(XMVectorSubtract(xmvNextPoint, xmvCurPoint));
		XMVECTOR xmvNextRailUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		XMVECTOR xmvNextRailRight = XMVector3Normalize(XMVector3Cross(xmvNextRailUp, xmvNextRailDirection));
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

		GenerateRollercoasterRailCube(vertices, indices, xmf3Vertex1, xmf3Vertex2, xmf3Vertex3, xmf3Vertex4);

	}

	// 5. 기둥
	for (int i = 1; i < nControlPoints; ++i) {
		int nPillarIndex = ((i * nInterpolateBias) * 36);
		GenerateRollercoasterPillarPolygon(vertices, indices, vertices[indices[nPillarIndex]].m_xmf3Position, 0.5f, 0.5f, xmf4Color);
		GenerateRollercoasterPillarPolygon(vertices, indices, vertices[indices[nPillarIndex + 3]].m_xmf3Position, 0.5f, 0.5f, xmf4Color);
	}


	pMesh->SetVertices(vertices);
	pMesh->SetIndices(indices);
	pMesh->SetStride(sizeof(DiffusedVertex));
	pMesh->Create(pd3dDevice, pd3dCommandList);

	pMesh->SetOBB(BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fCourseRadius, fCourseRadius, fCourseRadius), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)));

	RollercoasterRoute.assign(SplinePoints.begin(), SplinePoints.end());

}
