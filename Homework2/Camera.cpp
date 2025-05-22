#include "stdafx.h"
#include "Camera.h"
#include "Transform.h"
#include "Player.h"

using namespace std;

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::SetFOVAngle(float fAngle)
{
	m_fFOVAngle = fAngle;
	m_fProjectPlaneDistance = float(1.0f / tan(XMConvertToRadians(m_fFOVAngle * 0.5f)));
	m_bProjectionUpdated = TRUE;
}

void Camera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight)
{
	m_Viewport.SetViewport(xTopLeft, yTopLeft, nWidth, nHeight);
	m_fAspectRatio = float(m_Viewport.m_nWidth) / float(m_Viewport.m_nHeight);
	m_bProjectionUpdated = TRUE;
}

void Camera::SetNearZ(float fNearZ)
{
	m_fNearZ = fNearZ;
	m_bProjectionUpdated = TRUE;
}

void Camera::SetFarZ(float fFarZ)
{
	m_fFarZ = fFarZ;
	m_bProjectionUpdated = TRUE;
}

BOOL Camera::SetPosition(const XMFLOAT3& xmf3NewPosition)
{
	return SetPosition(XMLoadFloat3(&xmf3NewPosition));
}

BOOL Camera::SetPosition(const XMVECTOR& xmvNewPosition)
{
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);

	if (!XMVector3Equal(xmvPosition, xmvNewPosition)) {
		XMStoreFloat3(&m_xmf3Position, xmvNewPosition);
		m_bViewUpdated = TRUE;
	}

	return m_bViewUpdated;
}

BOOL Camera::SetPosition(float fXPos, float fYPos, float fZPos)
{
	return SetPosition(XMVectorSet(fXPos, fYPos, fZPos, 0.f));
}

void Camera::SetLookAt(const XMFLOAT3& xmf3Position, const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up)
{
	m_xmf3Position = xmf3Position;
	m_xmf4x4View = Matrix4x4::LookAtLH(m_xmf3Position, xmf3LookAt, xmf3Up);
	m_xmf3Right = Vector3::Normalize(XMFLOAT3(m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31));
	m_xmf3Up = Vector3::Normalize(XMFLOAT3(m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32));
	m_xmf3Look = Vector3::Normalize(XMFLOAT3(m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33));
}

void Camera::SetLookAt(const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View = Matrix4x4::LookAtLH(m_xmf3Position, xmf3LookAt, xmf3Up);
}

BOOL Camera::Rotate(const XMFLOAT3& xmf3NewRotation)
{
	return Rotate(xmf3NewRotation.x, xmf3NewRotation.y, xmf3NewRotation.z);
}

BOOL Camera::Rotate(const XMVECTOR& xmvNewRotation)
{
	XMFLOAT3 xmf3NewRotation;
	XMStoreFloat3(&xmf3NewRotation, xmvNewRotation);
	return Rotate(xmf3NewRotation.x, xmf3NewRotation.y, xmf3NewRotation.z);
}

BOOL Camera::Rotate(float fPitch, float fYaw, float fRoll)
{
	if (fPitch != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(fPitch));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
		m_bViewUpdated = TRUE;
	}
	if (fYaw != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(fYaw));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
		m_bViewUpdated = TRUE;
	}
	if (fRoll != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(fRoll));
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
		m_bViewUpdated = TRUE;
	}

	return m_bViewUpdated;
}

XMFLOAT4X4& Camera::GetViewPerspectiveProjectMatrix()
{
	return m_xmf4x4ViewPerspectiveProject;
}

XMFLOAT4X4& Camera::GetViewOrthographicProjectMatrix()
{
	return m_xmf4x4ViewOrthographicProject;
}

void Camera::Initialize(shared_ptr<Player> pOwnerPlayer)
{
	m_wpOwner = pOwnerPlayer;
}

void Camera::Update(float fElapsedTime)
{
	if (!m_wpOwner.expired()) { // If camera owned by someone(Player)
		if (m_bProjectionUpdated) {
			GeneratePerspectiveProjectionMatrix();
			GenerateOrthographicProjectionMatrix();
			m_bProjectionUpdated = FALSE;
		}

		if (m_bViewUpdated) {
			GenerateViewMatrix();
			m_bViewUpdated = FALSE;
		}
	}
	else {	// If camera stands only
		if (m_bProjectionUpdated) {
			GeneratePerspectiveProjectionMatrix();
			GenerateOrthographicProjectionMatrix();
			m_bProjectionUpdated = FALSE;
		}

		if (m_bViewUpdated) {
			GenerateViewMatrix();
			m_bViewUpdated = FALSE;
		}
	}
}

void Camera::GenerateViewMatrix()
{
	XMFLOAT3 xmf3Look = Vector3::Normalize(m_xmf3Look);
	XMFLOAT3 xmf3Right = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Up, xmf3Look));
	XMFLOAT3 xmf3Up = Vector3::Normalize(Vector3::CrossProduct(xmf3Look, xmf3Right));
	m_xmf4x4View._11 = xmf3Right.x; m_xmf4x4View._12 = xmf3Up.x; m_xmf4x4View._13 = xmf3Look.x;
	m_xmf4x4View._21 = xmf3Right.y; m_xmf4x4View._22 = xmf3Up.y; m_xmf4x4View._23 = xmf3Look.y;
	m_xmf4x4View._31 = xmf3Right.z; m_xmf4x4View._32 = xmf3Up.z; m_xmf4x4View._33 = xmf3Look.z;
	m_xmf4x4View._41 = -Vector3::DotProduct(m_xmf3Position, xmf3Right);
	m_xmf4x4View._42 = -Vector3::DotProduct(m_xmf3Position, xmf3Up);
	m_xmf4x4View._43 = -Vector3::DotProduct(m_xmf3Position, xmf3Look);

	m_xmf4x4ViewPerspectiveProject = Matrix4x4::Multiply(m_xmf4x4View, m_xmf4x4PerspectiveProject);
	m_xmf4x4OrthographicProject = Matrix4x4::Multiply(m_xmf4x4View, m_xmf4x4OrthographicProject);

	m_xmf4x4InverseView._11 = xmf3Right.x;			m_xmf4x4InverseView._12 = xmf3Right.y;			m_xmf4x4InverseView._13 = xmf3Right.z;
	m_xmf4x4InverseView._21 = xmf3Up.x;				m_xmf4x4InverseView._22 = xmf3Up.y;				m_xmf4x4InverseView._23 = xmf3Up.z;
	m_xmf4x4InverseView._31 = xmf3Look.x;			m_xmf4x4InverseView._32 = xmf3Look.y;			m_xmf4x4InverseView._33 = xmf3Look.z;
	m_xmf4x4InverseView._41 = m_xmf3Position.x;		
	m_xmf4x4InverseView._42 = m_xmf3Position.y;		
	m_xmf4x4InverseView._43 = m_xmf3Position.z;

	m_xmFrustumView.Transform(m_xmFrustumWorld, XMLoadFloat4x4(&m_xmf4x4InverseView));
}

void Camera::GeneratePerspectiveProjectionMatrix()
{
	XMMATRIX xmmtxPerspective = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fFOVAngle), m_fAspectRatio, m_fNearZ, m_fFarZ);
	XMStoreFloat4x4(&m_xmf4x4PerspectiveProject, xmmtxPerspective);
	BoundingFrustum::CreateFromMatrix(m_xmFrustumView, xmmtxPerspective);
}

void Camera::GenerateOrthographicProjectionMatrix()
{
	XMMATRIX xmmtxOrthographic = XMMatrixOrthographicLH(m_Viewport.m_nWidth, m_Viewport.m_nHeight, m_fNearZ, m_fFarZ);
	XMStoreFloat4x4(&m_xmf4x4OrthographicProject, xmmtxOrthographic);
}

bool Camera::IsInFrustum(const BoundingOrientedBox& xmBoundingBox) const
{
	return m_xmFrustumWorld.Intersects(xmBoundingBox);
}
