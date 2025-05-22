#include "stdafx.h"
#include "VertexType.h"

/*
	typedef struct D3D12_INPUT_ELEMENT_DESC
		{
		LPCSTR SemanticName;							HLSL semantic
		UINT SemanticIndex;								Semantic index (when 2 or more same semantic is available)
		DXGI_FORMAT Format;								type of element
		UINT InputSlot;									slot (0 ~ 15)
		UINT AlignedByteOffset;							Align offset (byte count until front element) : D3D12_APPEND_ALIGNED_ELEMENT
		D3D12_INPUT_CLASSIFICATION InputSlotClass;		Class of input data (per vertex / per instance)
		UINT InstanceDataStepRate;						Indicates how many instances the instance data repeats on : 0 is for vertex data
		} 	D3D12_INPUT_ELEMENT_DESC;
	
*/

std::vector<D3D12_INPUT_ELEMENT_DESC> Vertex::m_InputElementDesc = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
};

std::vector<D3D12_INPUT_ELEMENT_DESC> DiffusedVertex::m_InputElementDesc = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};