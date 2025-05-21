#pragma once
class Mesh;
struct Polygon;

namespace MeshHelper {

	void CreateCubeMesh(std::shared_ptr<Mesh> pMesh,
		float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f);

	void CreateWallMesh(std::shared_ptr<Mesh> pMesh,
		float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f, int nSubRects = 20);

	void CreateAirplaneMesh(std::shared_ptr<Mesh> pMesh,
		float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f);

	void AddCircleMeshToPolygonBuffer(std::vector<std::shared_ptr<struct Polygon>> pPolygons, 
		float fRadius, int nVertices, const XMFLOAT2& center = { 0,0 });

	BOOL CreateMeshFromOBJFiles(std::shared_ptr<Mesh> pMesh, std::wstring_view wstrObjPath);

	void CreateRollercoasterRailMesh(std::shared_ptr<Mesh> pMesh, OUT std::vector<XMFLOAT3>& RollercoasterRoute,
		float fWidth = 20.0f, float fCourseRadius = 90.0f, int nControlPoints = 5, int nInterpolateBias = 10);

};

