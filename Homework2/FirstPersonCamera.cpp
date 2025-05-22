#include "stdafx.h"
#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera()
{
}

FirstPersonCamera::~FirstPersonCamera()
{
}

BOOL FirstPersonCamera::Rotate(const XMFLOAT3& xmf3Rotation)
{
	return Rotate(XMLoadFloat3(&xmf3Rotation));
}

BOOL FirstPersonCamera::Rotate(const XMVECTOR& xmvRotation)
{
	XMVECTOR xmvCurRotation = XMLoadFloat3(&m_xmf3RotationEuler);
	XMVECTOR xmvAddRotation = XMVectorAdd(xmvCurRotation, xmvRotation);
	
	XMStoreFloat3(&m_xmf3RotationEuler, xmvAddRotation);
	m_bViewUpdated = TRUE;
	return m_bViewUpdated;
}

BOOL FirstPersonCamera::Rotate(float fPitch, float fYaw, float fRoll)
{
	return Rotate(XMVectorSet(fPitch, fYaw, fRoll, 0.0f));
}

BOOL FirstPersonCamera::SetRotation(const XMFLOAT3& xmf3NewRotation)
{
	return SetRotation(XMLoadFloat3(&xmf3NewRotation));
}

BOOL FirstPersonCamera::SetRotation(const XMVECTOR& xmvNewRotation)
{
	XMVECTOR xmvCurRotation = XMLoadFloat3(&m_xmf3RotationEuler);
	if (!XMVector3Equal(xmvCurRotation, xmvNewRotation)) {
		XMStoreFloat3(&m_xmf3RotationEuler, xmvNewRotation);
		m_bViewUpdated = TRUE;
	}

	return m_bViewUpdated;
}

BOOL FirstPersonCamera::SetRotation(float fPitch, float fYaw, float fRoll)
{
	return SetRotation(XMVectorSet(fPitch, fYaw, fRoll, 0.0f));
}

void FirstPersonCamera::GenerateViewMatrix()
{
	// Generate world matrix for extract right, up, look
	float fPitch, fYaw, fRoll;
	fPitch = XMConvertToRadians(m_xmf3RotationEuler.x);
	fYaw = XMConvertToRadians(m_xmf3RotationEuler.y);
	fRoll = XMConvertToRadians(m_xmf3RotationEuler.z);
	XMMATRIX xmmtxWorld = XMMatrixRotationRollPitchYaw(fPitch, fYaw, fRoll);
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, xmmtxWorld);

	m_xmf3Right = XMFLOAT3{ xmf4x4World._11, xmf4x4World._12, xmf4x4World._13 };
	m_xmf3Up = XMFLOAT3{ xmf4x4World._21, xmf4x4World._22, xmf4x4World._23 };
	m_xmf3Look = XMFLOAT3{ xmf4x4World._31, xmf4x4World._32, xmf4x4World._33 };

	m_xmf4x4View._11 = m_xmf3Right.x; m_xmf4x4View._12 = m_xmf3Up.x; m_xmf4x4View._13 = m_xmf3Look.x;
	m_xmf4x4View._21 = m_xmf3Right.y; m_xmf4x4View._22 = m_xmf3Up.y; m_xmf4x4View._23 = m_xmf3Look.y;
	m_xmf4x4View._31 = m_xmf3Right.z; m_xmf4x4View._32 = m_xmf3Up.z; m_xmf4x4View._33 = m_xmf3Look.z;
	m_xmf4x4View._41 = -XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_xmf3Position), XMLoadFloat3(&m_xmf3Right)));
	m_xmf4x4View._42 = -XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_xmf3Position), XMLoadFloat3(&m_xmf3Up)));
	m_xmf4x4View._43 = -XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_xmf3Position), XMLoadFloat3(&m_xmf3Look)));

	m_xmf4x4ViewPerspectiveProject = Matrix4x4::Multiply(m_xmf4x4View, m_xmf4x4PerspectiveProject);
	m_xmf4x4OrthographicProject = Matrix4x4::Multiply(m_xmf4x4View, m_xmf4x4OrthographicProject);

	m_xmf4x4InverseView._11 = m_xmf3Right.x;		m_xmf4x4InverseView._12 = m_xmf3Right.y;		m_xmf4x4InverseView._13 = m_xmf3Right.z;
	m_xmf4x4InverseView._21 = m_xmf3Up.x;			m_xmf4x4InverseView._22 = m_xmf3Up.y;			m_xmf4x4InverseView._23 = m_xmf3Up.z;
	m_xmf4x4InverseView._31 = m_xmf3Look.x;			m_xmf4x4InverseView._32 = m_xmf3Look.y;			m_xmf4x4InverseView._33 = m_xmf3Look.z;
	m_xmf4x4InverseView._41 = m_xmf3Position.x;		m_xmf4x4InverseView._42 = m_xmf3Position.y;		m_xmf4x4InverseView._43 = m_xmf3Position.z;

	m_xmFrustumView.Transform(m_xmFrustumWorld, XMLoadFloat4x4(&m_xmf4x4InverseView));
}

void FirstPersonCamera::Initialize(std::shared_ptr<Player> pOwnerPlayer)
{
	Camera::Initialize(pOwnerPlayer);
}

void FirstPersonCamera::Update(float fElapsedTime)
{
	Camera::Update(fElapsedTime);
}
