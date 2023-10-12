#pragma once

#include "D3DCommon.h"
#include "Resources.h"
#include "Resource.h"

#include "Bruno/Core/Base.h"
#include <Bruno/Core/RTTI.h>

namespace Bruno
{
	enum class TextureViewFlags : uint8_t
	{
		None = 0,
		Rtv = 1,
		Dsv = 2,
		Srv = 4,
		Uav = 8
	};
	BR_DEFINITION_FLAG_FROM_ENUM(TextureViewFlags);

	struct TextureCreationDesc
	{
		TextureCreationDesc()
		{
			mResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
			mResourceDesc.Width = 0;
			mResourceDesc.Height = 0;
			mResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			mResourceDesc.DepthOrArraySize = 1;
			mResourceDesc.MipLevels = 1;
			mResourceDesc.SampleDesc.Count = 1;
			mResourceDesc.SampleDesc.Quality = 0;
			mResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			mResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			mResourceDesc.Alignment = 0;
		}

		D3D12_RESOURCE_DESC mResourceDesc{};
		TextureViewFlags mViewFlags = TextureViewFlags::None;

		static TextureCreationDesc Create(uint32_t width, uint32_t height, DXGI_FORMAT format = DXGI_FORMAT_D32_FLOAT, TextureViewFlags viewFlags = TextureViewFlags::None) {
			TextureCreationDesc desc;
			desc.mResourceDesc.Format = format;
			desc.mResourceDesc.Width = width;
			desc.mResourceDesc.Height = height;
			desc.mViewFlags = viewFlags;
			return desc;
		}
	};

	class Texture;

	struct TextureUpload
	{
		Texture* mTexture = nullptr;
		std::unique_ptr<uint8_t[]> mTextureData;
		size_t mTextureDataSize = 0;
		uint32_t mNumSubResources = 0;
		SubResourceLayouts mSubResourceLayouts{ 0 };
	};

	class Texture : public Resource
	{
		BR_RTTI_DECLARATION(Texture, Resource);

	public:
		struct AssetPipelineInitData
		{
			size_t Width;
			size_t Height;
			size_t Depth;
			size_t ArraySize;
			size_t MipLevels;
			uint32_t Format;
			uint8_t Dimension;

			struct ImageInitData
			{
				int64_t RowPitch;
				int64_t SlicePitch;
				std::vector<uint8_t> Pixels;
			};
			std::vector<ImageInitData> Images;
		};
		Texture();
		Texture(const std::wstring& filename);
		Texture(const AssetPipelineInitData& textureInitData);
		Texture(const TextureCreationDesc& textureDesc);
		~Texture();

		constexpr const DescriptorHandle& GetSrvHandle() const { return mSRVDescriptor; }
		constexpr const DescriptorHandle& GetDsvHandle() const { return mDSVDescriptor; }

		friend class Surface;
		friend class GraphicsContext;

	protected:
		DescriptorHandle mRTVDescriptor{};
		DescriptorHandle mDSVDescriptor{};
		DescriptorHandle mSRVDescriptor{};
		DescriptorHandle mUAVDescriptor{};

	private:
		void CreateTexture(const TextureCreationDesc& desc);
		void CopyTextureSubresource(uint32_t firstSubresource, uint32_t numSubresources, D3D12_SUBRESOURCE_DATA* subresourceData);
		void GenerateMips();
	};
}
