#pragma once

struct Viewport {
	void SetViewport(int nLeft, int nTop, int nWidth, int nHeight) {
		m_nLeft = nLeft;
		m_nTop = nTop;
		m_nWidth = nWidth;
		m_nHeight = nHeight;
	}

	int m_nLeft = 0;
	int m_nTop = 0;
	int m_nWidth = 0;
	int m_nHeight = 0;
};

class Player;
class Transform;

struct CB_CAMERA_DATA {
	XMFLOAT4X4 xmf4ViewProjection;
};


class Camera {
public:
	Camera();
	virtual ~Camera();

public:
	// Projection parameters Setter
	void SetFOVAngle(float fAngle = 90.0f);
	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight);
	void SetNearZ(float fNearZ);
	void SetFarZ(float fFarZ);

	BOOL SetPosition(const XMFLOAT3& xmf3NewPosition);
	BOOL SetPosition(const XMVECTOR& xmvNewPosition);
	BOOL SetPosition(float fXPos, float fYPos, float fZPos);

	void SetLookAt(const XMFLOAT3& xmf3Position, const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up);
	void SetLookAt(const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up);

	virtual BOOL Rotate(const XMFLOAT3& xmf3NewRotation);
	virtual BOOL Rotate(const XMVECTOR& xmvNewRotation);
	virtual BOOL Rotate(float fPitch, float fYaw, float fRoll);

	D3D12_VIEWPORT& GetViewport() { return m_d3dViewport; }

	XMFLOAT4X4& GetViewMatrix() { return m_xmf4x4View; }
	XMFLOAT4X4& GetInverseViewMatrix() { return m_xmf4x4View; }
	XMFLOAT4X4& GetPerspectiveProjectMatrix() { return m_xmf4x4PerspectiveProject; }
	XMFLOAT4X4& GetViewPerspectiveProjectMatrix();
	XMFLOAT4X4& GetOrthographicProjectMatrix() { return m_xmf4x4OrthographicProject; }
	XMFLOAT4X4& GetViewOrthographicProjectMatrix();

public:
	void SetViewportAndScissorRects(ComPtr<ID3D12GraphicsCommandList> pd3dGraphicsCommansList);
	void CreateShaderVariables(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	void UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);


public:
	virtual void Initialize(std::shared_ptr<Player> pOwnerPlayer);
	virtual void Update(float fElapsedTime);

	bool IsInFrustum(const BoundingOrientedBox& xmBoundingBox) const;

protected:
	virtual void GenerateViewMatrix();
	void GeneratePerspectiveProjectionMatrix();
	void GenerateOrthographicProjectionMatrix();

protected:
	XMFLOAT3 m_xmf3Position = {};
	
	XMFLOAT3 m_xmf3Right	= XMFLOAT3{ 1.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Up		= XMFLOAT3{ 0.f, 1.f, 0.f };
	XMFLOAT3 m_xmf3Look		= XMFLOAT3{ 0.f, 0.f, 1.f };

	// Y angle
	float m_fFOVAngle = 0.f;
	float m_fAspectRatio = 0.f;
	float m_fProjectPlaneDistance = 0.f;
	float m_fNearZ = 0.f;
	float m_fFarZ = 0.f;

	BoundingFrustum m_xmFrustumView = {};
	BoundingFrustum m_xmFrustumWorld = {};

	XMFLOAT4X4 m_xmf4x4View = Matrix4x4::Identity();
	XMFLOAT4X4 m_xmf4x4InverseView = Matrix4x4::Identity();
	XMFLOAT4X4 m_xmf4x4PerspectiveProject = Matrix4x4::Identity();
	XMFLOAT4X4 m_xmf4x4ViewPerspectiveProject = Matrix4x4::Identity();
	XMFLOAT4X4 m_xmf4x4OrthographicProject = Matrix4x4::Identity(); 
	XMFLOAT4X4 m_xmf4x4ViewOrthographicProject = Matrix4x4::Identity(); 

	D3D12_VIEWPORT	m_d3dViewport = {};
	D3D12_RECT		m_d3dScissorRect = {};

	std::weak_ptr<Player> m_wpOwner;

	BOOL m_bViewUpdated = TRUE;
	BOOL m_bProjectionUpdated = TRUE;

protected:
	std::unique_ptr<ConstantBuffer<CB_CAMERA_DATA>> m_upcbCameraData;


};

