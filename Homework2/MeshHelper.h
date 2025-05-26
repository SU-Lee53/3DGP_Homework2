#pragma once
struct Polygon;

namespace MeshHelper {

	void CreateCubeMesh(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Mesh<DiffusedVertex>> pMesh,
		float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f, const XMFLOAT4& xmf4Color = XMFLOAT4{0.f, 0.f, 0.f, 1.f});

	void CreateWallMesh(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Mesh<DiffusedVertex>> pMesh,
		float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f, int nSubRects = 20, const XMFLOAT4& xmf4Color = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f });

	BOOL CreateMeshFromOBJFiles(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, 
		std::shared_ptr<Mesh<DiffusedVertex>> pMesh, std::wstring_view wstrObjPath, const XMFLOAT4& xmf4Color = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f });

	void CreateRollercoasterRailMesh(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, 
		std::shared_ptr<Mesh<DiffusedVertex>> pMesh, OUT std::vector<XMFLOAT3>& RollercoasterRoute,
		float fWidth = 20.0f, float fCourseRadius = 90.0f, int nControlPoints = 5, int nInterpolateBias = 10, const XMFLOAT4& xmf4Color = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f });

};

