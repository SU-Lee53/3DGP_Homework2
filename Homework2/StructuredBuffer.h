#pragma once

template<typename T>
class StructuredBuffer {
public:
	using data_type = T;

public:
	StructuredBuffer(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nDatas);
	~StructuredBuffer();


public:
	void UpdateData(std::span<T> data);
	void UpdateData(T* const data, UINT offset, UINT nDatas);
	void UpdateData(T* const data, UINT index);

	void SetBufferToPipeline(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT uiRootParameterSlot);


private:
	ComPtr<ID3D12Resource> m_pd3dUploadBuffer;
	UINT m_nDatas = 0;
	T* m_pMappedData = nullptr;

};

template<typename T>
inline StructuredBuffer<T>::StructuredBuffer(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nDatas)
{
	m_nDatas = nDatas;

	m_pd3dUploadBuffer = ::CreateBufferResources(
		pd3dDevice,
		pd3dCommandList,
		NULL,
		m_nDatas * sizeof(T),
		D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		NULL
	);

	m_pd3dUploadBuffer->Map(0, NULL, (void**)&m_pMappedData);
}

template<typename T>
inline StructuredBuffer<T>::~StructuredBuffer()
{
	m_pd3dUploadBuffer->Unmap(0, NULL);
}

template<typename T>
inline void StructuredBuffer<T>::UpdateData(std::span<T> data)
{
	assert(data.size() < m_nDatas);
	std::copy(data.begin(), data.end(), m_pMappedData);
}

template<typename T>
inline void StructuredBuffer<T>::UpdateData(T* data, UINT offset, UINT nDatas)
{
	assert(offset + nDatas < m_nDatas);
	::memcpy(m_pMappedData + (offset * sizeof(T)), data, nDatas * sizeof(T));
}

template<typename T>
inline void StructuredBuffer<T>::UpdateData(T* data, UINT index)
{
	assert(index < m_nDatas);
	::memcpy(m_pMappedData + (index * sizeof(T)), data, sizeof(T));
}

template<typename T>
inline void StructuredBuffer<T>::SetBufferToPipeline(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT uiRootParameterIndex)
{
	pd3dCommandList->SetGraphicsRootShaderResourceView(uiRootParameterIndex, m_pd3dUploadBuffer->GetGPUVirtualAddress());
}