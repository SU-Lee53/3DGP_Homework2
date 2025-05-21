#include "stdafx.h"
//#include <fstream>
//#include "MeshHelper.h"
//#include "Mesh.h"
//
//using namespace std;
//
//void MeshHelper::CreateCubeMesh(shared_ptr<Mesh> pMesh, float fWidth, float fHeight, float fDepth)
//{
//	pMesh->m_pPolygons.resize(6);
//
//	float fHalfWidth = fWidth * 0.5f;
//	float fHalfHeight = fHeight * 0.5f;
//	float fHalfDepth = fDepth * 0.5f;
//
//	shared_ptr<struct Polygon> pFrontFace = make_shared<struct Polygon>(4);
//	pFrontFace->SetVertex(0, Vertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
//	pFrontFace->SetVertex(1, Vertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
//	pFrontFace->SetVertex(2, Vertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
//	pFrontFace->SetVertex(3, Vertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
//	pMesh->SetPolygon(0, pFrontFace);
//
//	shared_ptr<struct Polygon> pTopFace = make_shared<struct Polygon>(4);
//	pTopFace->SetVertex(0, Vertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
//	pTopFace->SetVertex(1, Vertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
//	pTopFace->SetVertex(2, Vertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
//	pTopFace->SetVertex(3, Vertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
//	pMesh->SetPolygon(1, pTopFace);
//
//	shared_ptr<struct Polygon> pBackFace = make_shared<struct Polygon>(4);
//	pBackFace->SetVertex(0, Vertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
//	pBackFace->SetVertex(1, Vertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
//	pBackFace->SetVertex(2, Vertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
//	pBackFace->SetVertex(3, Vertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
//	pMesh->SetPolygon(2, pBackFace);
//
//	shared_ptr<struct Polygon> pBottomFace = make_shared<struct Polygon>(4);
//	pBottomFace->SetVertex(0, Vertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
//	pBottomFace->SetVertex(1, Vertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
//	pBottomFace->SetVertex(2, Vertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
//	pBottomFace->SetVertex(3, Vertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
//	pMesh->SetPolygon(3, pBottomFace);
//
//	shared_ptr<struct Polygon> pLeftFace = make_shared<struct Polygon>(4);
//	pLeftFace->SetVertex(0, Vertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
//	pLeftFace->SetVertex(1, Vertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
//	pLeftFace->SetVertex(2, Vertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
//	pLeftFace->SetVertex(3, Vertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
//	pMesh->SetPolygon(4, pLeftFace);
//
//	shared_ptr<struct Polygon> pRightFace = make_shared<struct Polygon>(4);
//	pRightFace->SetVertex(0, Vertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
//	pRightFace->SetVertex(1, Vertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
//	pRightFace->SetVertex(2, Vertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
//	pRightFace->SetVertex(3, Vertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
//	pMesh->SetPolygon(5, pRightFace);
//
//	pMesh->m_xmOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
//}
//
//void MeshHelper::CreateWallMesh(shared_ptr<Mesh> pMesh, float fWidth, float fHeight, float fDepth, int nSubRects)
//{
//	pMesh->m_pPolygons.resize((6 * nSubRects * nSubRects));
//
//	float fHalfWidth = fWidth * 0.5f;
//	float fHalfHeight = fHeight * 0.5f;
//	float fHalfDepth = fDepth * 0.5f;
//	float fCellWidth = fWidth * (1.0f / nSubRects);
//	float fCellHeight = fHeight * (1.0f / nSubRects);
//	float fCellDepth = fDepth * (1.0f / nSubRects);
//
//	int k = 0;
//	shared_ptr<struct Polygon> pLeftFace;
//	for (int i = 0; i < nSubRects; i++){
//		for (int j = 0; j < nSubRects; j++){
//			pLeftFace = make_shared<struct Polygon>(4);
//			pLeftFace->SetVertex(0, Vertex(-fHalfWidth, -fHalfHeight + (i * fCellHeight),			-fHalfDepth + (j * fCellDepth)));
//			pLeftFace->SetVertex(1, Vertex(-fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight),		-fHalfDepth + (j * fCellDepth)));
//			pLeftFace->SetVertex(2, Vertex(-fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight),		-fHalfDepth + ((j + 1) * fCellDepth)));
//			pLeftFace->SetVertex(3, Vertex(-fHalfWidth, -fHalfHeight + (i * fCellHeight),			-fHalfDepth + ((j + 1) * fCellDepth)));
//			pMesh->SetPolygon(k++, pLeftFace);
//		}
//	}
//
//	shared_ptr<struct Polygon> pRightFace;
//	for (int i = 0; i < nSubRects; i++){
//		for (int j = 0; j < nSubRects; j++){
//			pRightFace = make_shared<struct Polygon>(4);
//			pRightFace->SetVertex(0, Vertex(+fHalfWidth, -fHalfHeight + (i * fCellHeight),			-fHalfDepth + (j * fCellDepth)));
//			pRightFace->SetVertex(1, Vertex(+fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight),	-fHalfDepth + (j * fCellDepth)));
//			pRightFace->SetVertex(2, Vertex(+fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight),	-fHalfDepth + ((j + 1) * fCellDepth)));
//			pRightFace->SetVertex(3, Vertex(+fHalfWidth, -fHalfHeight + (i * fCellHeight),			-fHalfDepth + ((j + 1) * fCellDepth)));
//			pMesh->SetPolygon(k++, pRightFace);
//		}
//	}
//
//	shared_ptr<struct Polygon> pTopFace;
//	for (int i = 0; i < nSubRects; i++){
//		for (int j = 0; j < nSubRects; j++){
//			pTopFace = make_shared<struct Polygon>(4);
//			pTopFace->SetVertex(0, Vertex(-fHalfWidth + (i * fCellWidth),			+fHalfHeight,	-fHalfDepth + (j * fCellDepth)));
//			pTopFace->SetVertex(1, Vertex(-fHalfWidth + ((i + 1) * fCellWidth),		+fHalfHeight,	-fHalfDepth + (j * fCellDepth)));
//			pTopFace->SetVertex(2, Vertex(-fHalfWidth + ((i + 1) * fCellWidth),		+fHalfHeight,	-fHalfDepth + ((j + 1) * fCellDepth)));
//			pTopFace->SetVertex(3, Vertex(-fHalfWidth + (i * fCellWidth),			+fHalfHeight,	-fHalfDepth + ((j + 1) * fCellDepth)));
//			pMesh->SetPolygon(k++, pTopFace);
//		}
//	}
//
//	shared_ptr<struct Polygon> pBottomFace;
//	for (int i = 0; i < nSubRects; i++){
//		for (int j = 0; j < nSubRects; j++){
//			pBottomFace = make_shared<struct Polygon>(4);
//			pBottomFace->SetVertex(0, Vertex(-fHalfWidth + (i * fCellWidth),		-fHalfHeight,	-fHalfDepth + (j * fCellDepth)));
//			pBottomFace->SetVertex(1, Vertex(-fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight,	-fHalfDepth + (j * fCellDepth)));
//			pBottomFace->SetVertex(2, Vertex(-fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight,	-fHalfDepth + ((j + 1) * fCellDepth)));
//			pBottomFace->SetVertex(3, Vertex(-fHalfWidth + (i * fCellWidth),		-fHalfHeight,	-fHalfDepth + ((j + 1) * fCellDepth)));
//			pMesh->SetPolygon(k++, pBottomFace);
//		}
//	}
//
//	shared_ptr<struct Polygon> pFrontFace;
//	for (int i = 0; i < nSubRects; i++) {
//		for (int j = 0; j < nSubRects; j++) {
//			pFrontFace = make_shared<struct Polygon>(4);
//			pFrontFace->SetVertex(0, Vertex(-fHalfWidth + (i * fCellWidth),			-fHalfHeight + (j * fCellHeight),		+fHalfDepth));
//			pFrontFace->SetVertex(1, Vertex(-fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight + (j * fCellHeight),		+fHalfDepth));
//			pFrontFace->SetVertex(2, Vertex(-fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight + ((j + 1) * fCellHeight), +fHalfDepth));
//			pFrontFace->SetVertex(3, Vertex(-fHalfWidth + (i * fCellWidth),			-fHalfHeight + ((j + 1) * fCellHeight), +fHalfDepth));
//			pMesh->SetPolygon(k++, pFrontFace);
//		}
//	}
//
//	shared_ptr<struct Polygon> pBackFace;
//	for (int i = 0; i < nSubRects; i++) {
//		for (int j = 0; j < nSubRects; j++) {
//			pBackFace = make_shared<struct Polygon>(4);
//			pBackFace->SetVertex(0, Vertex(-fHalfWidth + (i * fCellWidth),			-fHalfHeight + (j * fCellHeight),		-fHalfDepth));
//			pBackFace->SetVertex(1, Vertex(-fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight + (j * fCellHeight),		-fHalfDepth));
//			pBackFace->SetVertex(2, Vertex(-fHalfWidth + ((i + 1) * fCellWidth),	-fHalfHeight + ((j + 1) * fCellHeight),	-fHalfDepth));
//			pBackFace->SetVertex(3, Vertex(-fHalfWidth + (i * fCellWidth),			-fHalfHeight + ((j + 1) * fCellHeight),	-fHalfDepth));
//			pMesh->SetPolygon(k++, pBackFace);
//		}
//	}
//
//	pMesh->m_xmOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
//}
//
//void MeshHelper::CreateAirplaneMesh(shared_ptr<Mesh> pMesh, float fWidth, float fHeight, float fDepth)
//{
//	pMesh->m_pPolygons.resize(24);
//
//	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;
//
//	float x1 = fx * 0.2f, y1 = fy * 0.2f, x2 = fx * 0.1f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1) * x2 + (fy - y3);
//	int i = 0;
//
//	//Upper Plane
//	shared_ptr<struct Polygon> pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(0.0f, +(fy + y3), -fz));
//	pFace->SetVertex(1, Vertex(+x1, -y1, -fz));
//	pFace->SetVertex(2, Vertex(0.0f, 0.0f, -fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(0.0f, +(fy + y3), -fz));
//	pFace->SetVertex(1, Vertex(0.0f, 0.0f, -fz));
//	pFace->SetVertex(2, Vertex(-x1, -y1, -fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(+x2, +y2, -fz));
//	pFace->SetVertex(1, Vertex(+fx, -y3, -fz));
//	pFace->SetVertex(2, Vertex(+x1, -y1, -fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(-x2, +y2, -fz));
//	pFace->SetVertex(1, Vertex(-x1, -y1, -fz));
//	pFace->SetVertex(2, Vertex(-fx, -y3, -fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	//Lower Plane
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(0.0f, +(fy + y3), +fz));
//	pFace->SetVertex(1, Vertex(0.0f, 0.0f, +fz));
//	pFace->SetVertex(2, Vertex(+x1, -y1, +fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(0.0f, +(fy + y3), +fz));
//	pFace->SetVertex(1, Vertex(-x1, -y1, +fz));
//	pFace->SetVertex(2, Vertex(0.0f, 0.0f, +fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(+x2, +y2, +fz));
//	pFace->SetVertex(1, Vertex(+x1, -y1, +fz));
//	pFace->SetVertex(2, Vertex(+fx, -y3, +fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(-x2, +y2, +fz));
//	pFace->SetVertex(1, Vertex(-fx, -y3, +fz));
//	pFace->SetVertex(2, Vertex(-x1, -y1, +fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	//Right Plane
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(0.0f, +(fy + y3), -fz));
//	pFace->SetVertex(1, Vertex(0.0f, +(fy + y3), +fz));
//	pFace->SetVertex(2, Vertex(+x2, +y2, -fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(+x2, +y2, -fz));
//	pFace->SetVertex(1, Vertex(0.0f, +(fy + y3), +fz));
//	pFace->SetVertex(2, Vertex(+x2, +y2, +fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(+x2, +y2, -fz));
//	pFace->SetVertex(1, Vertex(+x2, +y2, +fz));
//	pFace->SetVertex(2, Vertex(+fx, -y3, -fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(+fx, -y3, -fz));
//	pFace->SetVertex(1, Vertex(+x2, +y2, +fz));
//	pFace->SetVertex(2, Vertex(+fx, -y3, +fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	//Back/Right Plane
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(+x1, -y1, -fz));
//	pFace->SetVertex(1, Vertex(+fx, -y3, -fz));
//	pFace->SetVertex(2, Vertex(+fx, -y3, +fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(+x1, -y1, -fz));
//	pFace->SetVertex(1, Vertex(+fx, -y3, +fz));
//	pFace->SetVertex(2, Vertex(+x1, -y1, +fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(0.0f, 0.0f, -fz));
//	pFace->SetVertex(1, Vertex(+x1, -y1, -fz));
//	pFace->SetVertex(2, Vertex(+x1, -y1, +fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(0.0f, 0.0f, -fz));
//	pFace->SetVertex(1, Vertex(+x1, -y1, +fz));
//	pFace->SetVertex(2, Vertex(0.0f, 0.0f, +fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	//Left Plane
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(0.0f, +(fy + y3), +fz));
//	pFace->SetVertex(1, Vertex(0.0f, +(fy + y3), -fz));
//	pFace->SetVertex(2, Vertex(-x2, +y2, -fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(0.0f, +(fy + y3), +fz));
//	pFace->SetVertex(1, Vertex(-x2, +y2, -fz));
//	pFace->SetVertex(2, Vertex(-x2, +y2, +fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(-x2, +y2, +fz));
//	pFace->SetVertex(1, Vertex(-x2, +y2, -fz));
//	pFace->SetVertex(2, Vertex(-fx, -y3, -fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(-x2, +y2, +fz));
//	pFace->SetVertex(1, Vertex(-fx, -y3, -fz));
//	pFace->SetVertex(2, Vertex(-fx, -y3, +fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	//Back/Left Plane
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(0.0f, 0.0f, -fz));
//	pFace->SetVertex(1, Vertex(0.0f, 0.0f, +fz));
//	pFace->SetVertex(2, Vertex(-x1, -y1, +fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(0.0f, 0.0f, -fz));
//	pFace->SetVertex(1, Vertex(-x1, -y1, +fz));
//	pFace->SetVertex(2, Vertex(-x1, -y1, -fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(-x1, -y1, -fz));
//	pFace->SetVertex(1, Vertex(-x1, -y1, +fz));
//	pFace->SetVertex(2, Vertex(-fx, -y3, +fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pFace = make_shared<struct Polygon>(3);
//	pFace->SetVertex(0, Vertex(-x1, -y1, -fz));
//	pFace->SetVertex(1, Vertex(-fx, -y3, +fz));
//	pFace->SetVertex(2, Vertex(-fx, -y3, -fz));
//	pMesh->SetPolygon(i++, pFace);
//
//	pMesh->m_xmOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fx, fy, fz), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
//}
//
//BOOL MeshHelper::CreateMeshFromOBJFiles(shared_ptr<Mesh> pMesh, wstring_view wstrObjPath)
//{
//	ifstream in{ wstrObjPath.data() };
//
//	if (!in) return FALSE;
//
//	// 1. 파일에서 읽어옴
//	vector<XMFLOAT3> LoadedVertices;
//	vector<XMINT3> LoadedIndices;
//	std::string strRead{};
//	while (in >> strRead) {
//		if (strRead == "v") {
//			float fX, fY, fZ;
//			in >> fX >> fY >> fZ;
//			LoadedVertices.emplace_back(fX, fY, fZ);
//		}
//
//		if (strRead == "f") {
//			float n1, n2, n3;
//			in >> n1 >> n2 >> n3;
//			LoadedIndices.emplace_back(n1, n2, n3);
//		}
//	}
//
//	// 2. 읽어온 메쉬의 중심을 구함
//	auto [itMinX, itMaxX] = std::minmax_element(LoadedVertices.begin(), LoadedVertices.end(), [](const XMFLOAT3& xmf3_1, const XMFLOAT3& xmf3_2) { return xmf3_1.x < xmf3_2.x; });
//	auto [itMinY, itMaxY] = std::minmax_element(LoadedVertices.begin(), LoadedVertices.end(), [](const XMFLOAT3& xmf3_1, const XMFLOAT3& xmf3_2) { return xmf3_1.y < xmf3_2.y; });
//	auto [itMinZ, itMaxZ] = std::minmax_element(LoadedVertices.begin(), LoadedVertices.end(), [](const XMFLOAT3& xmf3_1, const XMFLOAT3& xmf3_2) { return xmf3_1.z < xmf3_2.z; });
//
//	XMFLOAT3 xmf3Center = { (itMaxX->x + itMinX->x) / 2, (itMaxY->y + itMinY->y) / 2, (itMaxZ->z + itMinZ->z) / 2 };
//
//	// 3. 메쉬의 중심을 0,0,0 으로 옮김
//	std::for_each(LoadedVertices.begin(), LoadedVertices.end(), [&xmf3Center](XMFLOAT3& xmf3Vertex) -> void {
//		xmf3Vertex.x -= xmf3Center.x;
//		xmf3Vertex.y -= xmf3Center.y;
//		xmf3Vertex.z -= xmf3Center.z;
//	});
//
//	// 4. 다시 메쉬의 중심을 구함 -> 0,0,0 에서 float 오차 이상 벗어나지 않아야 함
//	auto [itNewMinX, itNewMaxX] = std::minmax_element(LoadedVertices.begin(), LoadedVertices.end(), [](const XMFLOAT3& xmf3_1, const XMFLOAT3& xmf3_2) { return xmf3_1.x < xmf3_2.x; });
//	auto [itNewMinY, itNewMaxY] = std::minmax_element(LoadedVertices.begin(), LoadedVertices.end(), [](const XMFLOAT3& xmf3_1, const XMFLOAT3& xmf3_2) { return xmf3_1.y < xmf3_2.y; });
//	auto [itNewMinZ, itNewMaxZ] = std::minmax_element(LoadedVertices.begin(), LoadedVertices.end(), [](const XMFLOAT3& xmf3_1, const XMFLOAT3& xmf3_2) { return xmf3_1.z < xmf3_2.z; });
//
//	XMFLOAT3 xmf3NewCenter = { (itNewMaxX->x + itNewMinX->x) / 2, (itNewMaxY->y + itNewMinY->y) / 2, (itNewMaxZ->z + itNewMinZ->z) / 2 };
//
//	pMesh->m_pPolygons.resize(LoadedIndices.size());
//	for (const auto& [index, xmi3Indices] : std::views::enumerate(LoadedIndices)) {
//		shared_ptr<struct Polygon> pPolygon = make_shared<struct Polygon>(3);
//		pPolygon->SetVertex(0, Vertex{ LoadedVertices[xmi3Indices.x - 1] });
//		pPolygon->SetVertex(1, Vertex{ LoadedVertices[xmi3Indices.y - 1] });
//		pPolygon->SetVertex(2, Vertex{ LoadedVertices[xmi3Indices.z - 1] });
//		pMesh->SetPolygon(index, pPolygon);
//	}
//
//	XMFLOAT3 xmf3ObbExtent = Vector3::Subtract(XMFLOAT3{ itNewMaxX->x, itNewMaxY->y, itNewMaxZ->z }, xmf3NewCenter);
//	pMesh->m_xmOBB = BoundingOrientedBox(xmf3NewCenter, xmf3ObbExtent, XMFLOAT4{ 0.f, 0.f, 0.f, 1.f });
//
//}
//
//void GenerateRollercoasterPillarPolygon(shared_ptr<Mesh> pMesh, XMFLOAT3 xmf3TopPosition, float fWidth, float fDepth)
//{
//	XMFLOAT3 xmf3PillarCenter;
//	XMStoreFloat3(&xmf3PillarCenter, XMVectorMultiply(XMLoadFloat3(&xmf3TopPosition), XMVectorSet(1.0f, 0.5f, 1.0f, 0.0f)));
//
//	float fHalfWidth = fWidth * 0.5f;
//	float fHalfHeight = xmf3PillarCenter.y;
//	float fHalfDepth = fDepth * 0.5f;
//
//	shared_ptr<struct Polygon> pFrontFace = make_shared<struct Polygon>(4);
//	pFrontFace->SetVertex(0, Vertex(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z - fHalfDepth));
//	pFrontFace->SetVertex(1, Vertex(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z - fHalfDepth));
//	pFrontFace->SetVertex(2, Vertex(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z - fHalfDepth));
//	pFrontFace->SetVertex(3, Vertex(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z - fHalfDepth));
//	pMesh->AddPolygon(pFrontFace);
//
//	shared_ptr<struct Polygon> pTopFace = make_shared<struct Polygon>(4);
//	pTopFace->SetVertex(0, Vertex(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z + fHalfDepth));
//	pTopFace->SetVertex(1, Vertex(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z + fHalfDepth));
//	pTopFace->SetVertex(2, Vertex(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z - fHalfDepth));
//	pTopFace->SetVertex(3, Vertex(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z - fHalfDepth));
//	pMesh->AddPolygon(pTopFace);
//
//	shared_ptr<struct Polygon> pBackFace = make_shared<struct Polygon>(4);
//	pBackFace->SetVertex(0, Vertex(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z + fHalfDepth));
//	pBackFace->SetVertex(1, Vertex(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z + fHalfDepth));
//	pBackFace->SetVertex(2, Vertex(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z + fHalfDepth));
//	pBackFace->SetVertex(3, Vertex(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z + fHalfDepth));
//	pMesh->AddPolygon(pBackFace);
//
//	shared_ptr<struct Polygon> pBottomFace = make_shared<struct Polygon>(4);
//	pBottomFace->SetVertex(0, Vertex(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z - fHalfDepth));
//	pBottomFace->SetVertex(1, Vertex(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z - fHalfDepth));
//	pBottomFace->SetVertex(2, Vertex(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z + fHalfDepth));
//	pBottomFace->SetVertex(3, Vertex(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z + fHalfDepth));
//	pMesh->AddPolygon(pBottomFace);
//
//	shared_ptr<struct Polygon> pLeftFace = make_shared<struct Polygon>(4);
//	pLeftFace->SetVertex(0, Vertex(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z + fHalfDepth));
//	pLeftFace->SetVertex(1, Vertex(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z - fHalfDepth));
//	pLeftFace->SetVertex(2, Vertex(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z - fHalfDepth));
//	pLeftFace->SetVertex(3, Vertex(xmf3PillarCenter.x - fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z + fHalfDepth));
//	pMesh->AddPolygon(pLeftFace);
//
//	shared_ptr<struct Polygon> pRightFace = make_shared<struct Polygon>(4);
//	pRightFace->SetVertex(0, Vertex(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z - fHalfDepth));
//	pRightFace->SetVertex(1, Vertex(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y + fHalfHeight, xmf3PillarCenter.z + fHalfDepth));
//	pRightFace->SetVertex(2, Vertex(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z + fHalfDepth));
//	pRightFace->SetVertex(3, Vertex(xmf3PillarCenter.x + fHalfWidth, xmf3PillarCenter.y - fHalfHeight, xmf3PillarCenter.z - fHalfDepth));
//	pMesh->AddPolygon(pRightFace);
//
//}
//
//void MeshHelper::CreateRollercoasterRailMesh(shared_ptr<Mesh> pMesh, OUT std::vector<XMFLOAT3>& RollercoasterRoute, float fWidth, float fCourseRadius, int nControlPoints, int nInterpolateBias)
//{
//	assert(nControlPoints != 0);
//
//	vector<XMFLOAT3> ControlPoints(nControlPoints);
//	vector<XMFLOAT3> Tangents(nControlPoints);
//	
//	// 1. 컨트롤 포인트 생성
//	auto GenerateControlPoint = [fCourseRadius, nControlPoints](int idx) {
//		XMFLOAT3 v;
//		v.x = fCourseRadius * XMScalarCos(XMConvertToRadians((360.0f / nControlPoints) * idx));
//		v.y = RandomGenerator::GenerateRandomFloatInRange(5.0f, 150.0f);
//		v.z = fCourseRadius * XMScalarSin(XMConvertToRadians((360.0f / nControlPoints) * idx));
//		return v;
//	};
//
//
//	for (int i = 0; i < nControlPoints; ++i) {
//		ControlPoints[i] = GenerateControlPoint(i);
//	}
//
//
//	// 2. 접선
//	XMStoreFloat3(&Tangents[0], XMVectorZero());
//	XMStoreFloat3(&Tangents[nControlPoints - 1], XMVectorZero());
//
//	int count = 1;
//	for (auto& [v1, v2, v3] : ControlPoints | views::adjacent<3>) {
//		XMVECTOR xmvCP1 = XMLoadFloat3(&v1);
//		XMVECTOR xmvCP3 = XMLoadFloat3(&v3);
//
//		XMStoreFloat3(&Tangents[count++], XMVectorScale(XMVectorSubtract(xmvCP3, xmvCP1), 0.5f));
//	}
//
//	// 시작점과 끝점의 접선
//	// 시작점
//	{
//		XMVECTOR xmvCP1 = XMLoadFloat3(&ControlPoints[nControlPoints - 1]);
//		XMVECTOR xmvCP3 = XMLoadFloat3(&ControlPoints[1]);
//
//		XMStoreFloat3(&Tangents[0], XMVectorScale(XMVectorSubtract(xmvCP3, xmvCP1), 0.5f));
//	}
//
//	// 끝점
//	{
//		XMVECTOR xmvCP1 = XMLoadFloat3(&ControlPoints[nControlPoints - 2]);
//		XMVECTOR xmvCP3 = XMLoadFloat3(&ControlPoints[0]);
//
//		XMStoreFloat3(&Tangents[nControlPoints - 1], XMVectorScale(XMVectorSubtract(xmvCP3, xmvCP1), 0.5f));
//	}
//
//	// 3. 스플라인 곡선의 점 구함
//
//	vector<XMFLOAT3> SplinePoints;
//
//	for (int i = 0; i < nControlPoints - 1; ++i) {
//		XMVECTOR xmvControlPoint1 = XMLoadFloat3(&ControlPoints[i]);
//		XMVECTOR xmvControlPoint2 = XMLoadFloat3(&ControlPoints[i + 1]);
//		XMVECTOR xmvTangent1 = XMLoadFloat3(&Tangents[i]);
//		XMVECTOR xmvTangent2 = XMLoadFloat3(&Tangents[i + 1]);
//
//		for (int j = 0; j < nInterpolateBias; j++) {
//			float t = (float)j / nInterpolateBias;
//			XMVECTOR xmvPoint = XMVectorHermite(xmvControlPoint1, xmvTangent1, xmvControlPoint2, xmvTangent2, t);
//			XMFLOAT3 xmf3Point;
//			XMStoreFloat3(&xmf3Point, xmvPoint);
//			SplinePoints.push_back(xmf3Point);
//		}
//	}
//
//	// 마지막과 시작점의 점을 추가
//	{
//		XMVECTOR xmvControlPoint1 = XMLoadFloat3(&ControlPoints.back());
//		XMVECTOR xmvControlPoint2 = XMLoadFloat3(&ControlPoints.front());
//		XMVECTOR xmvTangent1 = XMLoadFloat3(&Tangents.back());
//		XMVECTOR xmvTangent2 = XMLoadFloat3(&Tangents.front());
//
//		for (int j = 0; j <= nInterpolateBias; j++) {
//			float t = (float)j / nInterpolateBias;
//			XMVECTOR xmvPoint = XMVectorHermite(xmvControlPoint1, xmvTangent1, xmvControlPoint2, xmvTangent2, t);
//			XMFLOAT3 xmf3Point;
//			XMStoreFloat3(&xmf3Point, xmvPoint);
//			SplinePoints.push_back(xmf3Point);
//		}
//	}
//
//	/*
//		SP : SplinePoints
//		         SP[0]                      SP[1]                      SP[2]        
//		  ---------+--------------------------+--------------------------+----------
//		           |0                        3|0                        3|
//				   |1                        2|1                        2|
//		  ---------+--------------------------+--------------------------+----------
//
//
//
//		  v0 ? ----------- ? v3
//			 |             |
//		p[i] ? ----------- ? p[i+1]
//			 |             |
//		  v1 ? ----------- ? v2
//
//		  v0, v1 : normalize(cross(p[i] - p[i-1]), up)) 하여 방향을 찾고 너비만큼의 간격으로 점을 찾으면 됨 (+- 방향 * (width / 2))
//		  v2, v3 : normalize(cross(p[i+1] - p[i]), up)) 하여 방향을 찾고 적절한 너비만큼의 간격으로 점을 찾으면 됨 (+- 방향 * (width / 2))
//
//	*/
//	
//	std::ofstream out{ "roll.txt" };
//
//	// 4. 레일
//	vector<shared_ptr<struct Polygon>> pPolygons;
//	for (int i = 1; i < SplinePoints.size() - 1; ++i) {
//		shared_ptr<struct Polygon> pRail = make_shared<struct Polygon>(4);
//		XMVECTOR xmvCurPoint = XMLoadFloat3(&SplinePoints[i]);
//		XMVECTOR xmvNextPoint = XMLoadFloat3(&SplinePoints[i + 1]);
//		XMVECTOR xmvPrevPoint = XMLoadFloat3(&SplinePoints[i - 1]);
//
//		// 현재 점 : 우 -> 좌 
//		XMVECTOR xmvCurRailDirection = XMVectorSubtract(xmvCurPoint, xmvPrevPoint);
//		XMVECTOR xmvCurRailUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
//		XMVECTOR xmvCurRailRight = XMVector3Normalize(XMVector3Cross(xmvCurRailDirection, xmvCurRailUp));
//		XMVECTOR xmvCurRailLeft = XMVector3Normalize(XMVectorScale(xmvCurRailRight, -1.f));
//
//		XMVECTOR xmvVertex1 = XMVector3TransformCoord(xmvCurPoint, XMMatrixTranslationFromVector(XMVectorScale(xmvCurRailLeft, fWidth / 2)));
//		XMVECTOR xmvVertex2 = XMVector3TransformCoord(xmvCurPoint, XMMatrixTranslationFromVector(XMVectorScale(xmvCurRailRight, fWidth / 2)));
//
//		// 다음 점 : 우 -> 좌 
//		XMVECTOR xmvNextRailDirection = XMVectorSubtract(xmvNextPoint, xmvCurPoint);
//		XMVECTOR xmvNextRailUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
//		XMVECTOR xmvNextRailRight = XMVector3Normalize(XMVector3Cross(xmvNextRailDirection, xmvNextRailUp));
//		XMVECTOR xmvNextRailLeft = XMVector3Normalize(XMVectorScale(xmvNextRailRight, -1.f));
//
//		XMVECTOR xmvVertex3 = XMVector3TransformCoord(xmvNextPoint, XMMatrixTranslationFromVector(XMVectorScale(xmvNextRailRight, fWidth / 2)));
//		XMVECTOR xmvVertex4 = XMVector3TransformCoord(xmvNextPoint, XMMatrixTranslationFromVector(XMVectorScale(xmvNextRailLeft, fWidth / 2)));
//
//		XMFLOAT3 xmf3Vertex1;
//		XMFLOAT3 xmf3Vertex2;
//		XMFLOAT3 xmf3Vertex3;
//		XMFLOAT3 xmf3Vertex4;
//		XMStoreFloat3(&xmf3Vertex1, xmvVertex1);
//		XMStoreFloat3(&xmf3Vertex2, xmvVertex2);
//		XMStoreFloat3(&xmf3Vertex3, xmvVertex3);
//		XMStoreFloat3(&xmf3Vertex4, xmvVertex4);
//
//		pRail->SetVertex(0, Vertex{ xmf3Vertex1 });
//		pRail->SetVertex(1, Vertex{ xmf3Vertex2 });
//		pRail->SetVertex(2, Vertex{ xmf3Vertex3 });
//		pRail->SetVertex(3, Vertex{ xmf3Vertex4 });
//
//		pPolygons.push_back(pRail);
//
//		out << i << "번째" << std::endl;
//		out << "1" << xmf3Vertex1.x << ", " << xmf3Vertex1.y << ", " << xmf3Vertex1.z << std::endl;
//		out << "2" << xmf3Vertex2.x << ", " << xmf3Vertex2.y << ", " << xmf3Vertex2.z << std::endl;
//		out << "3" << xmf3Vertex1.x << ", " << xmf3Vertex1.y << ", " << xmf3Vertex1.z << std::endl;
//		out << "4" << xmf3Vertex1.x << ", " << xmf3Vertex1.y << ", " << xmf3Vertex1.z << std::endl;
//
//
//	}
//
//	pMesh->m_pPolygons.resize((SplinePoints.size() - 2));
//	for (const auto& [index, pPolygon] : views::enumerate(pPolygons)) {
//		pMesh->SetPolygon(index, pPolygon);
//	}
//
//	// 5. 기둥
//	for (int i = 1; i < nControlPoints; ++i) {
//		int nPillarIndex = i * nInterpolateBias;
//		GenerateRollercoasterPillarPolygon(pMesh, pPolygons[nPillarIndex]->m_Vertices[0], 0.5f, 0.5f);
//		GenerateRollercoasterPillarPolygon(pMesh, pPolygons[nPillarIndex]->m_Vertices[1], 0.5f, 0.5f);
//	}
//
//	pMesh->m_xmOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fCourseRadius, fCourseRadius, fCourseRadius), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
//
//	RollercoasterRoute.assign(SplinePoints.begin(), SplinePoints.end());
//
//}
