#pragma once

class FirstPersonCamera : public Camera {
public:
	FirstPersonCamera();
	~FirstPersonCamera();

public:
	virtual BOOL Rotate(const XMFLOAT3& xmf3Rotation) override;
	virtual BOOL Rotate(const XMVECTOR& xmvRotation) override;
	virtual BOOL Rotate(float fPitch, float fYaw, float fRoll) override;

	BOOL SetRotation(const XMFLOAT3& xmf3NewRotation);
	BOOL SetRotation(const XMVECTOR& xmvNewRotation);
	BOOL SetRotation(float fPitch, float fYaw, float fRoll);

	XMFLOAT3& GetRotationEuler() {
		return m_xmf3RotationEuler;
	}

public:
	virtual void GenerateViewMatrix() override;

public:
	virtual void Initialize(std::shared_ptr<Player> pOwnerPlayer) override;
	virtual void Update(float fElapsedTime) override;

private:
	XMFLOAT3 m_xmf3RotationEuler = {};

};

