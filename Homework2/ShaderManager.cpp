#include "stdafx.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager() 
{

}

ShaderManager::~ShaderManager() 
{

}

void ShaderManager::OnCreate(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	m_pShaders[TAG_SHADER_DIFFUSED] = std::make_shared<DiffusedShader>();
	m_pShaders[TAG_SHADER_DIFFUSED]->CreateShader(pd3dDevice);


	m_pShaders[TAG_SHADER_DIFFUSED] = std::make_shared<InstancedShader>();
}
