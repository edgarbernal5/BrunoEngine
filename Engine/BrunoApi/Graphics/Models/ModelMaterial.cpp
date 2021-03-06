#include "stdafx.h"
#include "ModelMaterial.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Utils/StringUtility.h"

namespace TrioEngine
{
	std::map<ModelMaterial::TextureType, uint32_t> ModelMaterial::g_textureTypeMappings;

	ModelMaterial::ModelMaterial()
	{
		InitializeTextureTypeMappings();
	}

	ModelMaterial::ModelMaterial(aiMaterial* material)
	{
		InitializeTextureTypeMappings();

		aiString name;
		material->Get(AI_MATKEY_NAME, name);
		m_name = name.C_Str();

		for (TextureType textureType = (TextureType)0; 
			textureType < ModelMaterial::TextureType::TextureTypeEnd; 
			textureType = (TextureType)(textureType + 1))
		{
			aiTextureType mappedTextureType = (aiTextureType)g_textureTypeMappings[textureType];

			uint32_t textureCount = material->GetTextureCount(mappedTextureType);
			if (textureCount > 0)
			{
				std::vector<std::wstring>* textures = new std::vector<std::wstring>();
				m_textures.insert(std::pair<TextureType, std::vector<std::wstring>*>(textureType, textures));

				textures->reserve(textureCount);
				for (uint32_t textureIndex = 0; textureIndex < textureCount; textureIndex++)
				{
					aiString path;
					if (material->GetTexture(mappedTextureType, textureIndex, &path) == AI_SUCCESS)
					{
						std::wstring wPath;
						TrioUtils::StringUtility::ToWideString(path.C_Str(), wPath);
						
						textures->push_back(wPath);
					}
				}
			}
		}
	}

	ModelMaterial::~ModelMaterial()
	{
		for (std::pair<TextureType, std::vector<std::wstring>*> textures : m_textures)
		{
			DELETE_OBJECT(textures.second);
		}
	}

	const std::map<ModelMaterial::TextureType, std::vector<std::wstring>*>& ModelMaterial::GetTextures() const
	{
		return m_textures;
	}

	void ModelMaterial::InitializeTextureTypeMappings()
	{
		if (g_textureTypeMappings.size() != ModelMaterial::TextureType::TextureTypeEnd)
		{
			g_textureTypeMappings[ModelMaterial::TextureType::TextureTypeDifffuse] = aiTextureType_DIFFUSE;
			g_textureTypeMappings[ModelMaterial::TextureType::TextureTypeSpecularMap] = aiTextureType_SPECULAR;
			g_textureTypeMappings[ModelMaterial::TextureType::TextureTypeAmbient] = aiTextureType_AMBIENT;
			g_textureTypeMappings[ModelMaterial::TextureType::TextureTypeHeightmap] = aiTextureType_HEIGHT;
			g_textureTypeMappings[ModelMaterial::TextureType::TextureTypeNormalMap] = aiTextureType_NORMALS;
			g_textureTypeMappings[ModelMaterial::TextureType::TextureTypeSpecularPowerMap] = aiTextureType_SHININESS;
			g_textureTypeMappings[ModelMaterial::TextureType::TextureTypeDisplacementMap] = aiTextureType_DISPLACEMENT;
			g_textureTypeMappings[ModelMaterial::TextureType::TextureTypeLightMap] = aiTextureType_LIGHTMAP;
		}
	}
}