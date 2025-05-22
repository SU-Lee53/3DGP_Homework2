#include "stdafx.h"
#include <sstream>

ID3D12Resource* CreateBufferResources(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType, D3D12_RESOURCE_STATES d3dResourceStates, ID3D12Resource** ppd3dUploadBuffer)
{
	return nullptr;
}

void ShowErrorMessage(std::string_view file, int line, std::string_view message)
{
	std::string strFileMsg{ file };
	strFileMsg += '\n';

	std::string strLineMsg = "Line : " + std::to_string(line);
	strLineMsg += '\n';

	std::string strDebugMsg{ message };
	strDebugMsg += '\n';

	::OutputDebugStringA("*************** ERROR!! ***************\n");
	::OutputDebugStringA(strFileMsg.data());
	::OutputDebugStringA(strLineMsg.data());
	::OutputDebugStringA(strDebugMsg.data());
	::OutputDebugStringA("***************************************\n");
}

