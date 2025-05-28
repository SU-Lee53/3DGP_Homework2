#include "stdafx.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Shader.h"

using namespace std;

void RenderManager::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Camera> pCamera)
{
	UINT nInstanceCount = 0;

	for (const auto& [pMesh, shaderMap] : m_RenderMap) {
		if (!pMesh) 
			continue;

		for (const auto& [pShader, pObjs] : shaderMap) {
			pShader->SetRootSignature(pd3dCommandList);
			pShader->SetPipelineState(pd3dCommandList);
			pCamera->SetViewportAndScissorRects(pd3dCommandList);
			pCamera->UpdateShaderVariables(pd3dCommandList);

			if (auto pDiffusedShader = std::dynamic_pointer_cast<DiffusedShader>(pShader)) {
				for (auto& pObj : pObjs) {
					XMMATRIX xmmtxTransform = XMLoadFloat4x4(&pObj->GetTransform()->GetWorldMatrix());
					XMFLOAT4X4 xmf4x4Transform;
					XMStoreFloat4x4(&xmf4x4Transform, XMMatrixTranspose(xmmtxTransform));

					VS_TRANSFORM_DATA data{ pObj->GetModelTransform(), xmf4x4Transform };
					pDiffusedShader->UpdateShaderVariables(pd3dCommandList, &data, 1);

					pMesh->Render(pd3dCommandList, 1);
				}
			}
			else if (auto pInstancingShader = std::dynamic_pointer_cast<InstancedShader>(pShader)) {
				std::vector<VS_INSTANCING_DATA> transformDatas;
				transformDatas.resize(pObjs.size());

				std::transform(pObjs.begin(), pObjs.end(), transformDatas.begin(), [](const shared_ptr<GameObject>& pObj) {
					return VS_INSTANCING_DATA{ pObj->GetModelTransform(), pObj->GetTransform()->GetWorldMatrix(), pObj->GetColor() };
				});

				pInstancingShader->UpdateShaderVariables(pd3dCommandList, transformDatas.data(), transformDatas.size());

				pMesh->Render(pd3dCommandList, pObjs.size());
			}
		}
	}

}
