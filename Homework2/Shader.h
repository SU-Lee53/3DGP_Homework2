#pragma once

class Shader {
public:
	Shader();
	virtual ~Shader();

public:
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual void CreateVertexShader() {}
	virtual void CreatePixelShader() {}

	virtual void CreateShader(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);

	virtual void CreateRootSignature(ComPtr<ID3D12Device> pd3dDevice) {}

	virtual void CreateShaderVariables(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) {}
	virtual void UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, void* pData, UINT nDatas = 1) {}

	virtual void OnPrepareRender(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) {}
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, Camera* pCamera) {}

	void SetRootSignature(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) {
		pd3dCommandList->SetGraphicsRootSignature(m_pd3dRootSignature.Get());
	}
	void SetPipelineState(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) {
		pd3dCommandList->SetPipelineState(m_pd3dPipelineState.Get());
	}

	D3D12_SHADER_BYTECODE GetVSByteCode() {
		return D3D12_SHADER_BYTECODE{ m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize() };
	}
	
	D3D12_SHADER_BYTECODE GetPSByteCode() {
		return D3D12_SHADER_BYTECODE{ m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize() };
	}

protected:
	void CompileShaderFromFile(std::wstring_view wstrFileName, std::string_view strShaderName,
		std::string_view strShaderProfile);

protected:
	ComPtr<ID3D12RootSignature> m_pd3dRootSignature = nullptr;
	ComPtr<ID3D12PipelineState> m_pd3dPipelineState = nullptr;

	ComPtr<ID3DBlob> m_pVSBlob = nullptr;
	ComPtr<ID3DBlob> m_pPSBlob = nullptr;

};

class DiffusedShader : public Shader {
public:
	DiffusedShader();
	virtual ~DiffusedShader();

public:
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual void CreateVertexShader();
	virtual void CreatePixelShader();

	virtual void CreateShader(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);

	virtual void CreateRootSignature(ComPtr<ID3D12Device> pd3dDevice);

	virtual void CreateShaderVariables(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	virtual void UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, void* pData, UINT nDatas = 1);

	virtual void OnPrepareRender(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, Camera* pCamera);

};

class WireframeShader : public DiffusedShader {
public:
	WireframeShader();
	virtual ~WireframeShader();

public:
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState() override;

};


struct VS_INSTANCING_DATA {
	XMFLOAT4X4 xmf4x4Model;
	XMFLOAT4X4 xmf4x4World;
	XMFLOAT4 color;
};

class InstancingShader : public Shader {
public:
	InstancingShader();
	virtual ~InstancingShader();

public:
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual void CreateVertexShader();
	virtual void CreatePixelShader();

	virtual void CreateShader(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);

	virtual void CreateRootSignature(ComPtr<ID3D12Device> pd3dDevice);

	virtual void CreateShaderVariables(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	virtual void UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, void* pData, UINT nDatas = 1);

	virtual void OnPrepareRender(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, Camera* pCamera);

private:
	std::unique_ptr<StructuredBuffer<VS_INSTANCING_DATA>> m_upStructuredBuffer;

};