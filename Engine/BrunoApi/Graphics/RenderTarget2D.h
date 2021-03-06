#pragma once

#include "TrioApiRequisites.h"
#include "Texture2D.h"

#include "RenderTargetUsage.h"
#include "IRenderTarget.h"

#include <string>

namespace BrunoEngine
{
	class GraphicsDevice;
	class DepthStencilBuffer;

	class BRUNO_API_EXPORT RenderTarget2D : public Texture2D, public IRenderTarget
	{
	public:
		RenderTarget2D(GraphicsDevice* graphicsDevice, int width, int height);
		RenderTarget2D(GraphicsDevice* graphicsDevice, int width, int height, SurfaceFormat preferredFormat);
		RenderTarget2D(GraphicsDevice* graphicsDevice, int width, int height, uint32_t mipmap, SurfaceFormat preferredFormat, DepthFormat preferredDepthFormat);
		RenderTarget2D(GraphicsDevice* graphicsDevice, int width, int height, uint32_t mipmap, SurfaceFormat preferredFormat, DepthFormat preferredDepthFormat, RenderTargetUsage usage);

		~RenderTarget2D();

		int GetWidth();
		int GetHeight();

#if BRUNO_DIRECTX
		ID3D11RenderTargetView* GetRenderTargetView(int arraySlice);
		ID3D11DepthStencilView* GetDepthStencilView();
#endif

	protected:
		void CommonConstructor();

#if BRUNO_DIRECTX
		std::vector<ID3D11RenderTargetView*> m_renderTargetViews;
#endif

		RenderTargetUsage m_renderTargetUsage;
		DepthFormat m_depthFormat;

		std::unique_ptr<DepthStencilBuffer> m_depthStencilBuffer;
	private:
		bool m_initialized;
	};

}