#pragma once

struct Vertex {
	Vertex() = default;
	Vertex(float x, float y, float z) { m_xmf3Position = XMFLOAT3{ x,y,z }; }
	Vertex(const XMFLOAT3& xmf3Position) { m_xmf3Position = xmf3Position; }

	operator XMFLOAT3(){
		return m_xmf3Position;
	}

	XMFLOAT3 m_xmf3Position{ 0.f, 0.f, 0.f };
};

struct Polygon {
	Polygon() = default;
	Polygon(int nVertices) : m_Vertices{ std::vector<Vertex>(nVertices) } {}

	void SetVertex(int nIndex, Vertex& vertex) {
		if ((0 <= nIndex) && (nIndex < m_Vertices.size()) && !m_Vertices.empty()) {
			m_Vertices[nIndex] = vertex;
		}
	}

	std::vector<Vertex> m_Vertices = {};
};

class Mesh {
public:
	Mesh() = default;
	Mesh(int nPolygons) : m_pPolygons{ std::vector<std::shared_ptr<struct Polygon>>(nPolygons) } {};

public:
	void SetPolygon(int nIndex, std::shared_ptr<struct Polygon> pPolygon) {
		if ((0 <= nIndex) && (nIndex < m_pPolygons.size())) {
			m_pPolygons[nIndex] = pPolygon;
		}
	}

	void AddPolygon(std::shared_ptr<struct Polygon> pPolygon) {
		m_pPolygons.push_back(pPolygon);
	}

	virtual void Render(HDC hDCFrameBuffer);

	BOOL RayIntersectionByTriangle(const XMVECTOR& xmvRayOrigin, const XMVECTOR& xmvRayDirection,
		const XMVECTOR& v0, const XMVECTOR v1, const XMVECTOR v2, float& fNearHitDistance);

	int CheckRayIntersection(const XMVECTOR& xmvPickRayOrigin, const XMVECTOR& xmvPickRayDirection, float& fNearHitDistance);

	BoundingOrientedBox& GetOBB() { return m_xmOBB; }

protected:
	std::vector<std::shared_ptr<struct Polygon>>	m_pPolygons = {};

	BoundingOrientedBox						m_xmOBB = {};

	friend void MeshHelper::CreateCubeMesh(std::shared_ptr<Mesh> pMesh, float fWidth, float fHeight, float fDepth);
	friend void MeshHelper::CreateWallMesh(std::shared_ptr<Mesh> pMesh, float fWidth, float fHeight, float fDepth, int nSubRects);
	friend void MeshHelper::CreateAirplaneMesh(std::shared_ptr<Mesh> pMesh, float fWidth, float fHeight, float fDepth);
	friend BOOL MeshHelper::CreateMeshFromOBJFiles(std::shared_ptr<Mesh> pMesh, std::wstring_view wstrObjPath);
	friend void MeshHelper::CreateRollercoasterRailMesh(std::shared_ptr<Mesh> pMesh, OUT std::vector<XMFLOAT3>& RollercoasterRoute,
		float fWidth, float fCourseRadius, int nControlPoints, int nInterpolateBias);
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

