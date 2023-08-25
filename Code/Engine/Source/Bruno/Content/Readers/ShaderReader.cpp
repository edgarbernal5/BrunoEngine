#include "brpch.h"
#include "ShaderReader.h"

#include "Bruno/Content/ContentReader.h"

namespace Bruno
{
	BR_RTTI_DEFINITIONS(ShaderReader);

	ShaderReader::ShaderReader() :
		ContentTypeReader(Shader::GetTypeIdClass())
	{
	}

	std::shared_ptr<Shader> ShaderReader::ReadInternal(ContentReader& input)
	{
		std::vector<std::vector<uint8_t>> programsData;
		programsData.resize(Graphics::Core::SHADER_PROGRAMS_COUNT);
		for (size_t i = 0; i < Graphics::Core::SHADER_PROGRAMS_COUNT; i++)
		{
			std::vector<uint8_t> data;
			input.ReadBytes(data);

			programsData[i] = std::move(data);
		}
	
		return std::make_shared<Shader>(std::move(programsData));
	}
}