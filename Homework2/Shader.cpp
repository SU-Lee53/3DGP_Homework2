#include "stdafx.h"
#include "Shader.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}


D3D12_RASTERIZER_DESC Shader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	{
		d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		d3dRasterizerDesc.FrontCounterClockwise = FALSE;
		d3dRasterizerDesc.DepthBias = 0;
		d3dRasterizerDesc.DepthBiasClamp = 0.0f;
		d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
		d3dRasterizerDesc.DepthClipEnable = TRUE;
		d3dRasterizerDesc.MultisampleEnable = FALSE;
		d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
		d3dRasterizerDesc.ForcedSampleCount = 0;
		d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	}

	return d3dRasterizerDesc;
}

D3D12_BLEND_DESC Shader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	{
		d3dBlendDesc.AlphaToCoverageEnable = FALSE;
		d3dBlendDesc.IndependentBlendEnable = FALSE;
		d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
		d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
		d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
		d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	}

	return d3dBlendDesc;

}

D3D12_DEPTH_STENCIL_DESC Shader::CreateDepthStencilState() 
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	{
		d3dDepthStencilDesc.DepthEnable = TRUE;
		d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		d3dDepthStencilDesc.StencilEnable = FALSE;
		d3dDepthStencilDesc.StencilReadMask = 0x00;
		d3dDepthStencilDesc.StencilWriteMask = 0x00;
		d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
		d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	}

	return d3dDepthStencilDesc;
}

void Shader::CreateShader(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	CreateRootSignature(pd3dDevice);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	{
		d3dPipelineStateDesc.pRootSignature = m_pd3dRootSignature.Get();
		d3dPipelineStateDesc.VS = GetVSByteCode();
		d3dPipelineStateDesc.PS = GetPSByteCode();
		d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
		d3dPipelineStateDesc.BlendState = CreateBlendState();
		d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
		d3dPipelineStateDesc.InputLayout = DiffusedVertex::GetInputLayout();
		d3dPipelineStateDesc.SampleMask = UINT_MAX;
		d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		d3dPipelineStateDesc.NumRenderTargets = 1;
		d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		d3dPipelineStateDesc.SampleDesc.Count = 1;
		d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	}
	HRESULT hr = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, IID_PPV_ARGS(m_pd3dPipelineState.GetAddressOf()));

	if (FAILED(hr)) {
		::SHOW_ERROR("Create Pipeline Failed");
	}

}

void Shader::CompileShaderFromFile(std::wstring_view wstrFileName, std::string_view strShaderName, std::string_view strShaderProfile)
{
	UINT nCompileFlags = 0;
#ifdef _DEBUG
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> pShaderBlob = nullptr;
	ComPtr<ID3DBlob> pErrorBlob = nullptr;

	HRESULT hr = ::D3DCompileFromFile(wstrFileName.data(), NULL, NULL, strShaderName.data(), strShaderProfile.data(), nCompileFlags, 0, pShaderBlob.GetAddressOf(), pErrorBlob.GetAddressOf());

	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA((const char*)pErrorBlob->GetBufferPointer());
			__debugbreak();
		}
	}

	if (strShaderProfile.contains("vs")) {
		m_pVSBlob = pShaderBlob;
	}
	else if (strShaderProfile.contains("ps")) {
		m_pPSBlob = pShaderBlob;
	}

}

////////////////////
// DiffusedShader // 
////////////////////

DiffusedShader::DiffusedShader()
{
}

DiffusedShader::~DiffusedShader()
{
}

D3D12_RASTERIZER_DESC DiffusedShader::CreateRasterizerState()
{
	return Shader::CreateRasterizerState();
}

D3D12_BLEND_DESC DiffusedShader::CreateBlendState()
{
	return Shader::CreateBlendState();
}

D3D12_DEPTH_STENCIL_DESC DiffusedShader::CreateDepthStencilState()
{
	return Shader::CreateDepthStencilState();
}

void DiffusedShader::CreateVertexShader()
{
	return CompileShaderFromFile(
		L"../Shaders/DiffusedShader.hlsl",
		"VSDiffused",
		"vs_5_1"
	);
}

void DiffusedShader::CreatePixelShader()
{
	return CompileShaderFromFile(
		L"../Shaders/DiffusedShader.hlsl",
		"PSDiffused",
		"ps_5_1"
	);
}

void DiffusedShader::CreateShader(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	CreateVertexShader();
	CreatePixelShader();
	Shader::CreateShader(pd3dDevice, pd3dCommandList);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void DiffusedShader::CreateRootSignature(ComPtr<ID3D12Device> pd3dDevice)
{
	D3D12_ROOT_PARAMETER pd3dRootParameters[2];

	// Camera data (b0)
	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 0;
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	// Transform Data (b1)
	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[1].Descriptor.ShaderRegister = 1;
	pd3dRootParameters[1].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ComPtr<ID3DBlob> pd3dSignatureBlob = nullptr;
	ComPtr<ID3DBlob> pd3dErrorBlob = nullptr;

	HRESULT hr = ::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	if (FAILED(hr)) {
		if (pd3dErrorBlob) {
			OutputDebugStringA((char*)pd3dErrorBlob->GetBufferPointer());
		}
	}

	hr = pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(m_pd3dRootSignature.GetAddressOf()));

	if (FAILED(hr)) __debugbreak();

}

// TODO : Start from here

void DiffusedShader::CreateShaderVariables(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

void DiffusedShader::UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, void* pData, UINT nDatas)
{
}

void DiffusedShader::OnPrepareRender(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

void DiffusedShader::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, Camera* pCamera)
{
}

/////////////////////
// WireframeShader // 
/////////////////////

WireframeShader::WireframeShader()
{
}

WireframeShader::~WireframeShader()
{
}

D3D12_RASTERIZER_DESC WireframeShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	{
		d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
		d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		d3dRasterizerDesc.FrontCounterClockwise = FALSE;
		d3dRasterizerDesc.DepthBias = 0;
		d3dRasterizerDesc.DepthBiasClamp = 0.0f;
		d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
		d3dRasterizerDesc.DepthClipEnable = TRUE;
		d3dRasterizerDesc.MultisampleEnable = FALSE;
		d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
		d3dRasterizerDesc.ForcedSampleCount = 0;
		d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	}

	return d3dRasterizerDesc;
}

/////////////////////
// InstancingShader // 
/////////////////////

InstancingShader::InstancingShader()
{
}

InstancingShader::~InstancingShader()
{
}

D3D12_RASTERIZER_DESC InstancingShader::CreateRasterizerState()
{
	return Shader::CreateRasterizerState();
}

D3D12_BLEND_DESC InstancingShader::CreateBlendState()
{
	return Shader::CreateBlendState();
}

D3D12_DEPTH_STENCIL_DESC InstancingShader::CreateDepthStencilState()
{
	return Shader::CreateDepthStencilState();
}

void InstancingShader::CreateVertexShader()
{
	return CompileShaderFromFile(
		L"../Shaders/InstancingShader.hlsl",
		"VSInstancing",
		"vs_5_1"
	);
}

void InstancingShader::CreatePixelShader()
{
	return CompileShaderFromFile(
		L"../Shaders/InstancingShader.hlsl",
		"PSInstancing",
		"ps_5_1"
	);
}

void InstancingShader::CreateShader(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	CreateVertexShader();
	CreatePixelShader();
	Shader::CreateShader(pd3dDevice, pd3dCommandList);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void InstancingShader::CreateRootSignature(ComPtr<ID3D12Device> pd3dDevice) 
{
	D3D12_ROOT_PARAMETER pd3dRootParameters[2];

	// Camera data (b0)
	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 0;
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	// Transform Data (t0)
	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	pd3dRootParameters[1].Descriptor.ShaderRegister = 0;
	pd3dRootParameters[1].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ComPtr<ID3DBlob> pd3dSignatureBlob = nullptr;
	ComPtr<ID3DBlob> pd3dErrorBlob = nullptr;

	HRESULT hr = ::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	if (FAILED(hr)) {
		if (pd3dErrorBlob) {
			OutputDebugStringA((char*)pd3dErrorBlob->GetBufferPointer());
		}
	}

	hr = pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(m_pd3dRootSignature.GetAddressOf()));

	if (FAILED(hr)) __debugbreak();


}

void InstancingShader::CreateShaderVariables(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

void InstancingShader::UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, void* pData, UINT nDatas)
{
}

void InstancingShader::OnPrepareRender(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

void InstancingShader::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, Camera* pCamera)
{
}