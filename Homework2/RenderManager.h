#pragma once
#include <unordered_map>
#include "Mesh.h"
#include "Shader.h"
#include "GameObject.h"

class Camera;

using RenderMap = std::unordered_map<std::shared_ptr<Mesh_Base>, std::unordered_map<std::shared_ptr<Shader>, std::vector<std::shared_ptr<GameObject>>>>;

class RenderManager {
	DECLARE_SINGLE(RenderManager)
public:
	void OnCreate() {}
	void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Camera> pCamera);

public:
	void Reset() { m_RenderMap.clear(); }
	void Add(std::shared_ptr<GameObject> pObject) { m_RenderMap[pObject->GetMesh()][pObject->GetShader()].push_back(pObject); }



private:
	RenderMap m_RenderMap = {};


};

