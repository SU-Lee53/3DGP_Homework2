#pragma once

struct AxisAngle {
	XMFLOAT3	m_xmf3Axis = XMFLOAT3{ 1.f,1.f,1.f };
	float		m_fAngle;

	XMMATRIX GenerateRotationMatrix() {
		XMVECTOR xmvAxis = XMLoadFloat3(&m_xmf3Axis);
		if (m_fAngle == 0 || XMVector3Equal(xmvAxis, XMVectorZero())) {
			return XMMatrixIdentity();
		}
		return XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Axis), XMConvertToRadians(m_fAngle));
	}
};

class Transform
{
public:
	// Setter
	BOOL SetPosition(const XMFLOAT3& xmf3NewPosition);
	BOOL SetPosition(const XMVECTOR& xmvNewPosition);
	BOOL SetPosition(float fXPos, float fYPos, float fZPos);
	
	BOOL AddPosition(const XMFLOAT3& xmf3AddPosition);
	BOOL AddPosition(const XMVECTOR& xmvAddPosition);
	BOOL AddPosition(float fXPos, float fYPos, float fZPos);
	
	BOOL SetRotationEuler(const XMFLOAT3& xmf3NewRotation);
	BOOL SetRotationEuler(const XMVECTOR& xmvNewRotation);
	BOOL SetRotationEuler(float fPitch/* x */, float fYaw/* y */, float fRoll/* z */);
	BOOL SetRotationAxisAngle(const XMFLOAT3& xmf3Axis, float fAngle);
	BOOL SetRotationAxisAngle(const XMVECTOR& xmvAxis, float fAngle);
	BOOL SetRotationAxisAngle(const AxisAngle& AxisAngle);

	BOOL AddRotationEuler(const XMFLOAT3& xmf3AddRotation);
	BOOL AddRotationEuler(const XMVECTOR& xmvAddRotation);
	BOOL AddRotationEuler(float fPitch/* x */, float fYaw/* y */, float fRoll/* z */);

	BOOL SetWorldMatrix(const XMFLOAT4X4 xmf4x4World) {
		m_xmf4x4World = xmf4x4World;
		m_bManualManagement = TRUE;
		return TRUE;
	}

	// Getter
	XMFLOAT3& GetPosition() { return m_xmf3Position; }
	XMFLOAT3& GetRotationEuler() { return m_xmf3RotationEuler; }
	AxisAngle& GetRotationAxisAngle() { return m_AxisAngle; }

	XMFLOAT4X4& GetWorldMatrix() { return m_xmf4x4World; }

	XMFLOAT3 GetLook() const;
	XMFLOAT3 GetUp() const;
	XMFLOAT3 GetRight() const;

	void CacheLastFrameMovement() {
		m_xmf3BeforePosition = m_xmf3Position;
		m_xmf3BeforeRotationEuler = m_xmf3RotationEuler;
		m_BeforeAxisAngle = m_AxisAngle;
	}

	void InvalidateMovement() {
		m_xmf3Position = m_xmf3BeforePosition;
		m_xmf3RotationEuler = m_xmf3BeforeRotationEuler;
		m_BeforeAxisAngle = m_BeforeAxisAngle;
		m_bUpdated = TRUE;

		Update();
	}

public:
	void Update();

private:
	XMFLOAT3	m_xmf3Position = XMFLOAT3{ 0.f, 0.f, 0.f };
	XMFLOAT3	m_xmf3RotationEuler = XMFLOAT3{ 0.f, 0.f, 0.f };
	AxisAngle	m_AxisAngle = {};

	// Cache for block movement
	XMFLOAT3	m_xmf3BeforePosition = XMFLOAT3{ 0.f, 0.f, 0.f };
	XMFLOAT3	m_xmf3BeforeRotationEuler = XMFLOAT3{ 0.f, 0.f, 0.f };
	AxisAngle	m_BeforeAxisAngle = {};

	XMFLOAT4X4	m_xmf4x4World = Matrix4x4::Identity();
	BOOL m_bUpdated = TRUE;

	BOOL m_bManualManagement = FALSE;

};

