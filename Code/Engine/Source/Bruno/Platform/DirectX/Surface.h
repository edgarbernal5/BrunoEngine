#pragma once

#include "D3DCommon.h"
#include "Resources.h"
#include "DepthBuffer.h"

namespace Bruno
{
	struct SurfaceWindowParameters
	{
		uint32_t Width;
		uint32_t Height;
		DXGI_FORMAT BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM; //DXGI_FORMAT_B8G8R8A8_UNORM;
		uint32_t BackBufferCount = Graphics::Core::FRAME_BUFFER_COUNT;
		HWND WindowHandle;
		bool Fullscreen = false;
	};

	class GraphicsDevice;

	class Surface
	{
	public:
		Surface(const SurfaceWindowParameters& parameters);
		~Surface();

		void Initialize();
		void Present() const;
		void Resize(uint32_t width, uint32_t height);

		constexpr ID3D12Resource* const GetBackBuffer() const { return m_renderTargetData[m_currentBackBufferIndex].Resource; }
		constexpr const D3D12_CPU_DESCRIPTOR_HANDLE GetRtv() const { return m_renderTargetData[m_currentBackBufferIndex].Rtv.Cpu; }
		constexpr const D3D12_CPU_DESCRIPTOR_HANDLE GetDsv() const { return m_depthBuffer->GetDsv().Cpu; }
		constexpr const D3D12_VIEWPORT& GetViewport() const { return m_viewport; }
		constexpr const D3D12_RECT& GetScissorRect() const { return m_scissorRect; }
	private:
		struct RenderTargetData
		{
			ID3D12Resource* Resource{ nullptr };
			DescriptorHandle Rtv{};
		};

		SurfaceWindowParameters					m_parameters;

		Microsoft::WRL::ComPtr<IDXGISwapChain4> m_swapChain;
		RenderTargetData						m_renderTargetData[Graphics::Core::FRAME_BUFFER_COUNT]{};
		mutable uint32_t						m_currentBackBufferIndex{ 0 };

		// Depth buffer.
		std::unique_ptr<DepthBuffer>			m_depthBuffer;

		D3D12_VIEWPORT							m_viewport;
		D3D12_RECT								m_scissorRect{};

		void CreateDependentResources();
		void Finalize();
		void Release();
	};
}