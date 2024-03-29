#pragma once

#include <unordered_map>
#include <string>

namespace Bruno
{
	class Shader;
	enum class RenderShaderId : uint8_t
	{
		UnlitColored,
		OpaqueForward,

		Count
	};

	class ShaderCache
	{
	public:
		void Initialize();
		std::shared_ptr<Shader> Get(const std::wstring& filename);

	private:
		std::string GetShaderNameById(RenderShaderId shaderId);
		std::shared_ptr<Shader> Load(const std::wstring& filename);

		std::unordered_map<std::wstring, std::shared_ptr<Shader>> m_cache;
	};
}