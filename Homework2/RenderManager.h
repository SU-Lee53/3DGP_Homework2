#pragma once
#include <unordered_map>

class Shader;

using RenderMap = std::unordered_map<std::shared_ptr<Shader>, std::unordered_map<std::shared_ptr<Mesh_Base>, std::vector<std::shared_ptr<GameObject>>>>;

class RenderManager {
	DECLARE_SINGLE(RenderManager)
public:
	void OnCreate() {}
	void Render(std::shared_ptr<Camera> pCamera);

public:
	void Reset() { m_RenderMap.clear(); }
	void Add(std::shared_ptr<GameObject> pObject) { m_RenderMap[pObject->GetShader()][pObject->GetMesh()].push_back(pObject); }



private:
	RenderMap m_RenderMap = {};


};

