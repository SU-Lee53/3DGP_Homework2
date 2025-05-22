#pragma once
#include <unordered_map>

class Shader;

class RenderManager {
	DECLARE_SINGLE(RenderManager)
public:
	void OnCreate();
	void Render(std::shared_ptr<Camera> pCamera);

public:
	void Reset() { m_RenderMap.clear(); }
	void Add(std::shared_ptr<Shader> pMesh, std::shared_ptr<GameObject> pObject) { m_RenderMap[pMesh].push_back(pObject); }



private:
	std::unordered_map<std::shared_ptr<Shader>, std::list<std::shared_ptr<GameObject>>> m_RenderMap = {};


};

