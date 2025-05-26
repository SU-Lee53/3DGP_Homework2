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
	virtual void Initialize() override;
	virtual void Update(float fTimeElapsed) override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> m_pd3dCommandList, std::shared_ptr<Camera> pCamera);

private:
	std::weak_ptr<Player> m_wpOwner;

};

