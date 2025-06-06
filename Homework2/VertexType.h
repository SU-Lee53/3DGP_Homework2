#pragma once

struct Vertex {
	XMFLOAT3 m_xmf3Position{ 0.f, 0.f, 0.f };

	static D3D12_INPUT_LAYOUT_DESC GetInputLayout() {
		return { 
			Vertex::m_InputElementDesc.data(), 
			(UINT)Vertex::m_InputElementDesc.size() 
		};
	}

private:
	static std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputElementDesc;
};

struct DiffusedVertex {
	XMFLOAT3 m_xmf3Position{ 0.f, 0.f, 0.f };
	XMFLOAT4 m_xmf4Color{ 0.f, 0.f, 0.f, 1.f };

	static D3D12_INPUT_LAYOUT_DESC GetInputLayout() {
		return { 
			DiffusedVertex::m_InputElementDesc.data(), 
			(UINT)DiffusedVertex::m_InputElementDesc.size() 
		};
	}

private:
	static std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputElementDesc;
};

template <typename C>
concept VertexType = requires(C c) {
	{ c.m_xmf3Position } -> std::convertible_to<XMFLOAT3>;
};