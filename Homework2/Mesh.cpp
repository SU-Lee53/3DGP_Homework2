#include "stdafx.h"
#include "Mesh.h"

using namespace std;

void Mesh::Render(HDC hDCFrameBuffer)
{
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
	
	auto CheckRayIntersects = [=, &nIntersections, &xmvPickRayOrigin, &xmvPickRayDirection, &fNearHitDistance](shared_ptr<struct Polygon> pPolygon) {
		switch (pPolygon->m_Vertices.size()) {
		case 3:
		{
			XMVECTOR v0 = XMLoadFloat3(&(pPolygon->m_Vertices[0].m_xmf3Position));
			XMVECTOR v1 = XMLoadFloat3(&(pPolygon->m_Vertices[1].m_xmf3Position));
			XMVECTOR v2 = XMLoadFloat3(&(pPolygon->m_Vertices[2].m_xmf3Position));
			BOOL bIntersected = RayIntersectionByTriangle(xmvPickRayOrigin, xmvPickRayDirection, v0, v1, v2, fNearHitDistance);
			if (bIntersected) nIntersections++;
			break;
		}

		case 4:
		{
			XMVECTOR v0 = XMLoadFloat3(&(pPolygon->m_Vertices[0].m_xmf3Position));
			XMVECTOR v1 = XMLoadFloat3(&(pPolygon->m_Vertices[1].m_xmf3Position));
			XMVECTOR v2 = XMLoadFloat3(&(pPolygon->m_Vertices[2].m_xmf3Position));
			BOOL bIntersected = RayIntersectionByTriangle(xmvPickRayOrigin, xmvPickRayDirection, v0, v1, v2, fNearHitDistance);
			if (bIntersected) nIntersections++;

			v0 = XMLoadFloat3(&(pPolygon->m_Vertices[0].m_xmf3Position));
			v1 = XMLoadFloat3(&(pPolygon->m_Vertices[2].m_xmf3Position));
			v2 = XMLoadFloat3(&(pPolygon->m_Vertices[3].m_xmf3Position));
			bIntersected = RayIntersectionByTriangle(xmvPickRayOrigin, xmvPickRayDirection, v0, v1, v2, fNearHitDistance);
			if (bIntersected) nIntersections++;
			break;
		}
		}
	};

	if (bIntersected) {
		for_each(m_pPolygons.begin(), m_pPolygons.end(), CheckRayIntersects);
	}


	return nIntersections;
}

// ==========
//  AxisMesh
// ==========

AxisMesh::AxisMesh(float fWidth, float fHeight, float fDepth)
{
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
}

void AxisMesh::Render(HDC hDCFrameBuffer)
{
}
