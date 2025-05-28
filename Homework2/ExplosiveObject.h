#pragma once
#include "GameObject.h"
class ExplosiveObject : public GameObject {
public:
	ExplosiveObject();
	virtual ~ExplosiveObject();

public:
	void SetAutoReset(BOOL bAutoReset) { m_bAutoReset = bAutoReset; }
	BOOL IsBlowingUp() const { return m_bBlowingUp; }
	BOOL IsExploded() const { return m_bExplosionCompleted; }
	void Reset() {
		m_bBlowingUp = FALSE;
		m_bExplosionCompleted = FALSE;
		m_fElapsedTimes = 0.0f;
		std::for_each(m_xmf4x4Transforms.begin(), m_xmf4x4Transforms.end(), [](XMFLOAT4X4& xmf4x4Transform) {
			xmf4x4Transform = Matrix4x4::Identity();
		});
	}

	

public:
	virtual void Initialize(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;
	virtual void Update(float fTimeElapsed) override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Camera> pCamera) override;
	virtual void OnPicked() override;
	virtual	void BeginCollision(std::shared_ptr<GameObject> pOther) override;

public:
	static void PrepareExplosion(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);

protected:
	BOOL										m_bBlowingUp = FALSE;
	BOOL										m_bExplosionCompleted = FALSE;
	BOOL										m_bAutoReset = TRUE;
	std::array<XMFLOAT4X4, EXPLOSION_DEBRISES>	m_xmf4x4Transforms = {};

	float m_fElapsedTimes = 0.f;
	float m_fDuration = 2.0f;
	float m_fExplosionSpeed = 10.0f;
	float m_fExplosionRotation = 720.0f;
	
private:
	static std::shared_ptr<Mesh<DiffusedVertex>>	m_pExplosionMesh;
	static std::array<XMFLOAT3, EXPLOSION_DEBRISES> m_xmf3SphereVectors;



};

