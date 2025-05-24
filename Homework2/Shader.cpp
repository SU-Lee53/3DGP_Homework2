#include "stdafx.h"
#include "Shader.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::CompileShaderFromFile(std::wstring_view wstrFileName, std::string_view strShaderName, std::string_view strShaderProfile)
{
	UINT nCompileFlags = 0;
#ifdef _DEBUG
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> pShaderBlob = nullptr;
	ComPtr<ID3DBlob> pErrorBlob = nullptr;

	if (strShaderProfile.contains("vs")) {
		pShaderBlob = m_pVSBlob;
	}
	else if (strShaderProfile.contains("ps")) {
		pShaderBlob = m_pPSBlob;
	}

	HRESULT hr = ::D3DCompileFromFile(wstrFileName.data(), NULL, NULL, strShaderName.data(), strShaderProfile.data(), nCompileFlags, 0, pShaderBlob.GetAddressOf(), pErrorBlob.GetAddressOf());

	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA((const char*)pErrorBlob->GetBufferPointer());
			__debugbreak();
		}
	}
}

/////////////////
// BasicShader // 
/////////////////

BasicShader::BasicShader()
{
}

BasicShader::~BasicShader()
{
}

D3D12_RASTERIZER_DESC BasicShader::CreateRasterizerState()
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

D3D12_BLEND_DESC BasicShader::CreateBlendState()
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

D3D12_DEPTH_STENCIL_DESC BasicShader::CreateDepthStencilState()
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

void BasicShader::CreateVertexShader()
{
	return CompileShaderFromFile(
		L"BasicShader.hlsl",
		"VSBasic",
		"vs_5_1"
	);
}

void BasicShader::CreatePixelShader()
{
	return CompileShaderFromFile(
		L"BasicShader.hlsl",
		"PSBasic",
		"ps_5_1"
	);
}

void BasicShader::CreateShader(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12RootSignature> pd3dRootSignature)
{
	CreateVertexShader();
	CreatePixelShader();

	m_pd3dPipelineStates.resize(1);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	{
		d3dPipelineStateDesc.pRootSignature = pd3dRootSignature.Get();
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
	pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, IID_PPV_ARGS(m_pd3dPipelineStates[0].GetAddressOf()));

}

// TODO : Start from here

void BasicShader::CreateShaderVariables(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

void BasicShader::UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

void BasicShader::UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, XMFLOAT4X4& pxmf4x4World)
{
}

void BasicShader::ReleaseShaderVariables()
{
}

void BasicShader::OnPrepareRender(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

void BasicShader::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, Camera* pCamera)
{
}
