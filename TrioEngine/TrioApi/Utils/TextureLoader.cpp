#include "stdafx.h"
#include "TextureLoader.h"


namespace Vago
{
	TextureLoader::TextureLoader() : 
		m_uiImgId(0)
	{
		ilInit();
		iluInit();
	}

	TextureLoader::~TextureLoader()
	{
		for (size_t i = 0; i < m_vBuffers.size(); i++)
		{
			delete[] m_vBuffers[i];
		}
		m_vBuffers.clear();
	}

	TextureLoader::ImageInfo TextureLoader::GetTextureFromFile(std::string csFilename, TextureLoaderType targetType, SurfaceFormat targetFormat)
	{
		// Generate the main image name to use.
		ilGenImages(1, &m_uiImgId);

		// Bind this image name.
		ilBindImage(m_uiImgId);

		wchar_t bufferFilename[256];
		int nChars = MultiByteToWideChar(CP_ACP, 0, csFilename.c_str(), -1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, csFilename.c_str(), -1, bufferFilename, nChars);

		if (!ilLoadImage(bufferFilename))
		{
			return TextureLoader::ImageInfo();
		}

		ILinfo ilInfo;
		iluGetImageInfo(&ilInfo);

		TextureLoader::ImageInfo imageInfo;
		imageInfo.Width = ilInfo.Width;
		imageInfo.Height = ilInfo.Width;
		imageInfo.Bpp = ilInfo.Bpp;
		imageInfo.NumMips = ilInfo.NumMips;
		imageInfo.Type = (TextureLoaderType)ilInfo.Type;
		imageInfo.Format = (TextureLoaderFormat)ilInfo.Format;

		ILint targetToIl = 0;
		int localBpp = 0;
		switch (targetFormat)
		{
		case Vago::SurfaceFormat::ColorSRgb:
		case Vago::SurfaceFormat::Color:
			targetToIl = IL_RGBA;
			localBpp = 4;
			break;
		case Vago::SurfaceFormat::Bgr32:
		case Vago::SurfaceFormat::Bgr32SRgb:
		case Vago::SurfaceFormat::Bgr565:
			targetToIl = IL_BGR;
			localBpp = 3;
			break;
		case Vago::SurfaceFormat::Bgra32:
		case Vago::SurfaceFormat::Bgra4444:
		case Vago::SurfaceFormat::Bgra32SRgb:
		case Vago::SurfaceFormat::Bgra5551:
			targetToIl = IL_BGRA;
			localBpp = 4;
			break;

		case Vago::SurfaceFormat::Alpha8:
			targetToIl = IL_ALPHA;
			localBpp = 1;
			break;
		default:
			targetToIl = IL_RGBA;
			localBpp = 4;
			break;
		}
		uint32_t bufferSize = ilInfo.Width * ilInfo.Height * ilInfo.Depth * localBpp;
		ILubyte	*buffer = new ILubyte[bufferSize];
		if (buffer == nullptr)
		{
			return TextureLoader::ImageInfo();
		}

		imageInfo.Data = buffer;
		m_vBuffers.push_back(buffer);

		ilCopyPixels(0, 0, 0, ilInfo.Width, ilInfo.Height, ilInfo.Depth, targetToIl, (ILenum)targetType, buffer);

		imageInfo.SizeOfData = bufferSize;

		ilDeleteImages(1, &m_uiImgId);

		return imageInfo;
	}
}