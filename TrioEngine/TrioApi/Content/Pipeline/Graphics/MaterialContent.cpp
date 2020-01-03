#include "stdafx.h"
#include "MaterialContent.h"

namespace TrioEngine
{
	MaterialContent::MaterialContent()
	{
		m_processorName = "MaterialWriter";
	}

	ExternalReference& MaterialContent::GetTexture(std::string key)
	{
		ExternalReference reference;
		std::map<std::string, ExternalReference>::iterator itm = m_textures.find(key);
		if (itm != m_textures.end())
			return (*itm).second;

		return reference;
	}
}