#pragma once

#include "TrioApiRequisites.h"

#include "SwapChainFlags.h"
#include "DeviceErrorStatus.h"

#include <string>

namespace TrioEngine
{
	class SwapChain 
	{
	public:
		SwapChain(GraphicsDevice* graphicsDevice, int width, int height, HWND window, SurfaceFormat backBufferformat, uint32_t bufferCount);

		DeviceErrorStatus ResizeBuffers(uint32_t bufferCount, int width, int height, SurfaceFormat format, SwapChainFlags flags);
		DeviceErrorStatus Present(uint32_t syncInterval, uint32_t flags);

#if TRIO_DIRECTX
		IDXGISwapChain1* GetDxSwapChain() const { return m_swapChain.Get(); }
#endif

	private:
		GraphicsDevice* m_graphicsDevice;

#if TRIO_DIRECTX

		Microsoft::WRL::ComPtr<IDXGISwapChain1>				m_swapChain;
#endif
	};
}