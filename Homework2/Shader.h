#pragma once

class Shader {
public:
	Shader();
	virtual ~Shader();

public:
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState() {}
	virtual D3D12_BLEND_DESC CreateBlendState() {}
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState() {}

	virtual void CreateVertexShader() {}
	virtual void CreatePixelShader() {}

	virtual void CreateShader(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12RootSignature> pd3dRootSignature) {}

	virtual void CreateShaderVariables(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) {}
	virtual void UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) {}
	virtual void UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, XMFLOAT4X4& pxmf4x4World) {}
	virtual void ReleaseShaderVariables() {}

	virtual void OnPrepareRender(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) {}
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, Camera* pCamera) {}

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
	// Array of shader's pipeline
	std::vector<ComPtr<ID3D12PipelineState>> m_pd3dPipelineStates = {};

	ComPtr<ID3DBlob> m_pVSBlob = nullptr;
	ComPtr<ID3DBlob> m_pPSBlob = nullptr;

};

class BasicShader : public Shader {
public:
	BasicShader();
	virtual ~BasicShader();

public:
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual void CreateVertexShader();
	virtual void CreatePixelShader();

	virtual void CreateShader(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12RootSignature> pd3dRootSignature);

	virtual void CreateShaderVariables(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	virtual void UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	virtual void UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, XMFLOAT4X4& pxmf4x4World);
	virtual void ReleaseShaderVariables();

	virtual void OnPrepareRender(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, Camera* pCamera);

};