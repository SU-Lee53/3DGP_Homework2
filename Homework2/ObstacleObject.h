#pragma once
#include "GameObject.h"
class ObstacleObject : public GameObject {
public:
	ObstacleObject() {}
	virtual ~ObstacleObject() {}

public:
	virtual void Initialize(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	virtual void Update(float fElapsedTime);
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<class Camera> pCamera) override;

public:
	virtual void BeginCollision(std::shared_ptr<GameObject> pOther) override;


private:
	BoundingOrientedBox			m_xmOBBPlayerMoveCheck = {};
	std::array<XMFLOAT4, 6>		m_xmf4WallPlanes = {};
};

