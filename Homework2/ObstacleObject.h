#pragma once
#include "GameObject.h"
class ObstacleObject : public GameObject {
public:
	ObstacleObject() {}
	virtual ~ObstacleObject() {}

public:
	virtual void Initialize();
	virtual void Update(float fElapsedTime);
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> m_pd3dCommandList, std::shared_ptr<class Camera> pCamera);

public:
	virtual void BeginCollision(std::shared_ptr<GameObject> pOther) override;


private:
	BoundingOrientedBox			m_xmOBBPlayerMoveCheck = {};
	std::array<XMFLOAT4, 6>		m_xmf4WallPlanes = {};
};

