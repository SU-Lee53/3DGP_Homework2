#include "StructuredBuffer.h"
#pragma once

template<typename T>
class ConstantBuffer {
public:
	using data_type = T;

public:
	ConstantBuffer(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	~ConstantBuffer();

public:
	void UpdateData(const T& data);
	void SetBufferToPipeline(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT uiRootParameterSlot);

private:
	ComPtr<ID3D12Resource> m_pd3dUploadBuffer = nullptr;
	T* m_pMappedData = nullptr;

};

template<typename T>
inline ConstantBuffer<T>::ConstantBuffer(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	m_pd3dUploadBuffer = ::CreateBufferResources(
		pd3dDevice,
		pd3dCommandList,
		NULL,
		sizeof(T),
		D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		NULL
	);

	m_pd3dUploadBuffer->Map(0, NULL, (void**)&m_pMappedData);
}

template<typename T>
inline ConstantBuffer<T>::~ConstantBuffer()
{
	m_pd3dUploadBuffer->Unmap(0, NULL);
}

template<typename T>
inline void ConstantBuffer<T>::UpdateData(const T& data)
{
	::memcpy(m_pMappedData, &data, sizeof(T));
}

template<typename T>
inline void ConstantBuffer<T>::SetBufferToPipeline(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT uiRootParameterIndex)
{
	pd3dCommandList->SetGraphicsRootConstantBufferView(uiRootParameterIndex, m_pd3dUploadBuffer->GetGPUVirtualAddress());
}
