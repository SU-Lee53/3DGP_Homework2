#pragma once
#include "Shader.h"

enum TAG_SHADER_TYPE : UINT8 {
	TAG_SHADER_DIFFUSED = 0,
	TAG_SHADER_WIREFRAME,
	TAG_SHADER_INSTANCED,

	TAG_SHADER_COUNT
};

struct SHADER_LOAD_INFO {
	std::wstring wstrShaderFile = L"";
	std::string strShaderName = "";
	std::string strShaderProfile = "";
};


class ShaderManager {
	DECLARE_SINGLE(ShaderManager)
public:
	void OnCreate(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	std::shared_ptr<Shader>& GetShader(TAG_SHADER_TYPE eShaderType) { return m_pShaders[eShaderType]; }



private:
	std::array<std::shared_ptr<Shader>, TAG_SHADER_COUNT> m_pShaders = {};
	


};

