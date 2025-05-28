#pragma once
#include "GameObject.h"

class ShieldObject : public GameObject
{
public:
	ShieldObject();
	virtual ~ShieldObject();

public:
	void SetOwner(std::shared_ptr<Player> pObj) { m_wpOwner = pObj; }

public:
	virtual void Initialize(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;
	virtual void Update(float fTimeElapsed) override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Camera> pCamera) override;

private:
	std::weak_ptr<Player> m_wpOwner;

};

