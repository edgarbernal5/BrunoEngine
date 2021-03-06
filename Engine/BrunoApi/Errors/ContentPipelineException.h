#pragma once

#include "TrioApiRequisites.h"

#include <exception>
#include <Windows.h>
#include <string>

namespace BrunoEngine
{
	class ContentPipelineException : public std::exception
	{
	public:
		ContentPipelineException(const char* const& message);

		std::wstring whatw() const;
	};
}