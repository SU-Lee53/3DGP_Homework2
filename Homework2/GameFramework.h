#pragma once
#include "GameTimer.h"

class Scene;

class GameFramework {
public:
	GameFramework();
	~GameFramework() {}

public:
	void OnCreate(HINSTANCE hInstance, HWND hMainWnd, bool bEnableDebugLayer);
	void OnDestroy();

	void Update();
	void Render();
	void MoveToNextFrame();

public:
	void BuildObjects();
	void ReleaseObjects();

private:
	void CreateD3DDevice(bool bEnableDebugLayer);
	void CreateSwapChain();
	void CreateRTVAndDSVDescriptorHeaps();
	void CreateCommandQueueAndList();

private:
	void CreateRenderTargetViews();
	void CreateDepthStencilView();

private:
	// Fullscreen
	void ChangeSwapChainState();

public:
	void ProcessInput();

private:
	void RenderBegin();
	void RenderEnd();
	void Present();

private:
	// GPU-CPU Sync
	void WaitForGPUComplete();

public:
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);


#pragma region D3D
private:
	HINSTANCE	m_hInstance = nullptr;
	HWND		m_hWnd = nullptr;

	int m_nWndClientWidth = 0;
	int m_nWndClientHeight = 0;

	ComPtr<IDXGIFactory4>	m_pdxgiFactory = nullptr;
	ComPtr<IDXGISwapChain3> m_pdxgiSwapChain = nullptr;
	ComPtr<ID3D12Device>	m_pd3dDevice = nullptr;

	bool m_bEnableDebugLayer = false;
	bool m_bEnableGBV = false;

	bool m_bMsaa4xEnable = false;
	bool m_nMsaa4xQualityLevels = 0;

	const static UINT m_nSwapChainBuffers = 2;

	UINT m_nSwapChainBufferIndex = 0;

private:
	// Framework resources
	// RTV
	std::array<ComPtr<ID3D12Resource>, m_nSwapChainBuffers> m_ppd3dRenderTargetBuffers = {};
	ComPtr<ID3D12DescriptorHeap>	m_pd3dRTVDescriptorHeap = nullptr;
	UINT							m_nRTVDescriptorIncrementSize = 0;

	// DSV
	ComPtr<ID3D12Resource>			m_pd3dDepthStencilBuffer = nullptr;
	ComPtr<ID3D12DescriptorHeap>	m_pd3dDSVDescriptorHeap = nullptr;
	UINT							m_nDSVDescriptorIncrementSize = 0;

	// Command Queue, Command Allocator, Command List
	ComPtr<ID3D12CommandQueue>			m_pd3dCommandQueue = nullptr;
	ComPtr<ID3D12CommandAllocator>		m_pd3dCommandAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList>	m_pd3dCommandList = nullptr;

	// Pipeline State Object
	ComPtr<ID3D12PipelineState> m_pd3dPipelineState = nullptr;

	// Fence & Fence value
	ComPtr<ID3D12Fence> m_pd3dFence = nullptr;
	HANDLE				m_hFenceEvent = nullptr;
	std::array<UINT64, m_nSwapChainBuffers> m_nFenceValues = {};

#pragma endregion

private:
	GameTimer	m_GameTimer = {};

	POINT		m_ptOldCursorPos = {};

	_TCHAR		m_pszFrameRate[50] = {};

private:
	// Manages Scenes
	static std::shared_ptr<Scene> m_pCurrentScene;






};
