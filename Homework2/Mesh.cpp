#include "stdafx.h"
#include "Mesh.h"

using namespace std;

BOOL Mesh_Base::RayIntersectionByTriangle(const XMVECTOR& xmvRayOrigin, const XMVECTOR& xmvRayDirection, const XMVECTOR& v0, const XMVECTOR& v1, const XMVECTOR& v2, float& fNearHitDistance)
{
	float fHitDistance;
	BOOL bIntersected = TriangleTests::Intersects(xmvRayOrigin, xmvRayDirection, v0, v1, v2, fHitDistance);
	if (bIntersected && (fHitDistance < fNearHitDistance)) {
		fNearHitDistance = fHitDistance;
	}

	return bIntersected;
}
