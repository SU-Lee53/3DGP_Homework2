// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// STL
#include <vector>
#include <array>
#include <span>
#include <algorithm>
#include <memory>
#include <type_traits>
#include <random>
#include <unordered_set>
#include <ranges>

using tstring = std::basic_string<_TCHAR>;

// Direct3D related headers
#include <string>	
#include <wrl.h>
#include <shellapi.h>	

// D3D12
#include <d3d12.h>
#include <dxgi1_4.h>
#include <dxgidebug.h>


// D3DCompiler
#include <d3dcompiler.h>

// DirectXMath
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

// dxgidebug
#include <dxgidebug.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

using namespace Microsoft::WRL;


// Import libraries
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

// DirectXMath
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
using namespace DirectX::PackedVector;


constexpr static DWORD FRAME_BUFFER_WIDTH = 640;
constexpr static DWORD FRAME_BUFFER_HEIGHT = 480;

constexpr static UINT8 DIR_FORWARD = 0x01;
constexpr static UINT8 DIR_BACKWARD = DIR_FORWARD << 1;
constexpr static UINT8 DIR_LEFT = DIR_BACKWARD << 1;
constexpr static UINT8 DIR_RIGHT = DIR_LEFT << 1;
constexpr static UINT8 DIR_UP = DIR_RIGHT << 1;
constexpr static UINT8 DIR_DOWN = DIR_UP << 1;

constexpr static UINT8 EXPLOSION_DEBRISES = 240;

// Utility functions
extern ComPtr<ID3D12Resource> CreateBufferResources(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList,
	void* pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType = D3D12_HEAP_TYPE_UPLOAD,
	D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ID3D12Resource** ppd3dUploadBuffer = nullptr);

extern void ShowErrorMessage(std::string_view file, int line, std::string_view message);

extern XMFLOAT4 ConvertWinColorToD3DColor(COLORREF color, float alpha = 1.0f);

#define SHOW_ERROR(strMsg)		ShowErrorMessage(__FILE__, __LINE__, strMsg);


inline bool IsZero(float fValue) { return fabsf(fValue) < std::numeric_limits<float>::epsilon(); }
inline bool IsEqual(float fA, float fB) { return (::IsZero(fA - fB)); }


// Additional Headers
#include "Defines.h"
#include "VertexType.h"

#include "RandomGenerator.h"
#include "XMMathHelper.h"

#include "ConstantBuffer.h"
#include "StructuredBuffer.h"


// Game Framework
#include "GameFramework.h"
#include "Mesh.h"
#include "MeshHelper.h"
#include "GameObject.h"


// Managers
#include "InputManager.h"
#include "RenderManager.h"
#include "ShaderManager.h"

#undef min
#undef max