#include "stdafx.h"
#include <sstream>

ID3D12Resource* CreateBufferResources(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, 
	void* pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType, 
	D3D12_RESOURCE_STATES d3dResourceStates, ID3D12Resource** ppd3dUploadBuffer)
{
	ID3D12Resource* pd3dBuffer = nullptr;

	D3D12_HEAP_PROPERTIES d3dHeapPropertiesDesc;
	::ZeroMemory(&d3dHeapPropertiesDesc, sizeof(D3D12_HEAP_PROPERTIES));
	{
		d3dHeapPropertiesDesc.Type = d3dHeapType;
		d3dHeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		d3dHeapPropertiesDesc.CreationNodeMask = 1;
		d3dHeapPropertiesDesc.VisibleNodeMask = 1;
	}

	D3D12_RESOURCE_DESC d3dResourceDesc;
	::ZeroMemory(&d3dResourceDesc, sizeof(D3D12_RESOURCE_DESC));
	{
		d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		d3dResourceDesc.Alignment = 0;
		d3dResourceDesc.Width = nBytes;
		d3dResourceDesc.Height = 1;
		d3dResourceDesc.DepthOrArraySize = 1;
		d3dResourceDesc.MipLevels = 1;
		d3dResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		d3dResourceDesc.SampleDesc.Count = 1;
		d3dResourceDesc.SampleDesc.Quality = 0;
		d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	}

	D3D12_RESOURCE_STATES d3dResourceInitialStates = D3D12_RESOURCE_STATE_COPY_DEST;
	if (d3dHeapType == D3D12_HEAP_TYPE_UPLOAD)
		d3dResourceInitialStates = D3D12_RESOURCE_STATE_GENERIC_READ;
	else if (d3dHeapType == D3D12_HEAP_TYPE_READBACK)
		d3dResourceInitialStates = D3D12_RESOURCE_STATE_COPY_DEST;

	HRESULT hr = pd3dDevice->CreateCommittedResource(
		&d3dHeapPropertiesDesc,
		D3D12_HEAP_FLAG_NONE,
		&d3dResourceDesc,
		d3dResourceInitialStates,
		NULL,
		IID_PPV_ARGS(&pd3dBuffer)
	);

	if (FAILED(hr)) {
		__debugbreak();
		return nullptr;
	}

	if (pData)
	{
		switch (d3dHeapType)
		{
		case D3D12_HEAP_TYPE_DEFAULT:
			if (ppd3dUploadBuffer)
			{
				// Create upload heap
				d3dHeapPropertiesDesc.Type = D3D12_HEAP_TYPE_UPLOAD;
				hr = pd3dDevice->CreateCommittedResource(
					&d3dHeapPropertiesDesc,
					D3D12_HEAP_FLAG_NONE,
					&d3dResourceDesc,
					D3D12_RESOURCE_STATE_GENERIC_READ,
					NULL,
					IID_PPV_ARGS(ppd3dUploadBuffer)
				);

				if (FAILED(hr)) {
					__debugbreak();
					return nullptr;
				}

				// map upload buffer and copy initial data. 
				D3D12_RANGE d3dReadRange = { 0,0 };
				UINT8* pBufferDataBegin = NULL;
				(*ppd3dUploadBuffer)->Map(0, &d3dReadRange, (void**)&pBufferDataBegin);
				{
					memcpy(pBufferDataBegin, pData, nBytes);
				}
				(*ppd3dUploadBuffer)->Unmap(0, NULL);

				// copy upload buffer data to default buffer.
				pd3dCommandList->CopyResource(pd3dBuffer, *ppd3dUploadBuffer);

				D3D12_RESOURCE_BARRIER d3dResourceBarrier;
				::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
				{
					d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
					d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
					d3dResourceBarrier.Transition.pResource = pd3dBuffer;
					d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
					d3dResourceBarrier.Transition.StateAfter = d3dResourceStates;
					d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				}
				pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);

				break;
			}

		case D3D12_HEAP_TYPE_UPLOAD:
		{
			// map upload buffer and copy initial data. 
			D3D12_RANGE d3dReadRange = { 0,0 };
			UINT8* pBufferDataBegin = NULL;
			pd3dBuffer->Map(0, &d3dReadRange, (void**)&pBufferDataBegin);
			{
				memcpy(pBufferDataBegin, pData, nBytes);
			}
			pd3dBuffer->Unmap(0, NULL);

			break;
		}

		case D3D12_HEAP_TYPE_READBACK:
			break;

		}
	}

	return pd3dBuffer;
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

