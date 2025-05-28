#include "stdafx.h"
#include "ShaderManager.h"

void ShaderManager::OnCreate(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	m_pShaders[TAG_SHADER_DIFFUSED] = std::make_shared<DiffusedShader>();
	m_pShaders[TAG_SHADER_DIFFUSED]->CreateShader(pd3dDevice, pd3dCommandList);
	
	m_pShaders[TAG_SHADER_WIREFRAME] = std::make_shared<WireframeShader>();
	m_pShaders[TAG_SHADER_WIREFRAME]->CreateShader(pd3dDevice, pd3dCommandList);

	m_pShaders[TAG_SHADER_INSTANCED] = std::make_shared<InstancedShader>();
	m_pShaders[TAG_SHADER_INSTANCED]->CreateShader(pd3dDevice, pd3dCommandList);
}
