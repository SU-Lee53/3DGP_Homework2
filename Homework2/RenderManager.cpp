#include "stdafx.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Shader.h"

void RenderManager::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Camera> pCamera)
{
	UINT nInstanceCount = 0;

	for (const auto& [pMesh, shaderMap] : m_RenderMap) {
		pCamera->SetViewportAndScissorRects(pd3dCommandList);
		pCamera->UpdateShaderVariables(pd3dCommandList);

		for (const auto& [pShader, pObjs] : shaderMap) {
			pShader->SetRootSignature(pd3dCommandList);
			pShader->SetPipelineState(pd3dCommandList);

			if (auto pDiffusedShader = std::dynamic_pointer_cast<DiffusedShader>(pShader)) {
				for (auto& pObj : pObjs) {
					XMMATRIX xmmtxTransform = XMLoadFloat4x4(&pObj->GetTransform()->GetWorldMatrix());
					XMFLOAT4X4 xmf4x4Transform;
					XMStoreFloat4x4(&xmf4x4Transform, XMMatrixTranspose(xmmtxTransform));

					VS_TRANSFORM_DATA data{ xmf4x4Transform, pObj->GetColor() };
					pDiffusedShader->UpdateShaderVariables(pd3dCommandList, &data, 1);

					pMesh->Render(pd3dCommandList, 1);
				}
			}
			else if (auto pInstancingShader = std::dynamic_pointer_cast<InstancedShader>(pShader)) {
				std::vector<VS_TRANSFORM_DATA> transformDatas;
				transformDatas.reserve(pObjs.size());
				for (const auto& pObj : pObjs) {
					XMMATRIX xmmtxTransform = XMLoadFloat4x4(&pObj->GetTransform()->GetWorldMatrix());
					XMFLOAT4X4 xmf4x4Transform;
					XMStoreFloat4x4(&xmf4x4Transform, XMMatrixTranspose(xmmtxTransform));

					VS_TRANSFORM_DATA data{ xmf4x4Transform, pObj->GetColor() };
					transformDatas.push_back(data);
				}

				pInstancingShader->UpdateShaderVariables(pd3dCommandList, transformDatas.data(), transformDatas.size());

				pMesh->Render(pd3dCommandList, pObjs.size());
			}
		}
	}

}
