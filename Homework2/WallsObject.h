#pragma once
#include "GameObject.h"

class WallsObject : public GameObject {
public:
	WallsObject();
	virtual ~WallsObject();

	virtual void Update(float fTimeElapsed) override;
	virtual void Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera) override;

	void SetWallPlane(int nIndex, const XMFLOAT4& xmf4Plane) {
		m_xmf4WallPlanes[nIndex] = xmf4Plane;
	}

	void SetOBB(const BoundingOrientedBox& xmOBB) { m_xmOBBPlayerMoveCheck = xmOBB; }

	std::array<XMFLOAT4, 6>& GetWallPlanes() { return m_xmf4WallPlanes; }

private:
	BoundingOrientedBox			m_xmOBBPlayerMoveCheck = {};
	std::array<XMFLOAT4, 6>		m_xmf4WallPlanes = {};

};

