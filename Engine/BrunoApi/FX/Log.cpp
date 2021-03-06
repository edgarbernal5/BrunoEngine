#include "stdafx.h"
#include "Log.h"
#include "../Utils/StringUtility.h"

#include <iostream>

namespace BrunoFX
{
	void Log_Error(const char* format, ...)
	{
		va_list args;
		va_start(args, format);

		char buffer[1024];
		int count = TrioUtils::StringUtility::Printf(buffer, sizeof(buffer), format, args);

		va_end(args);

		std::cerr << "ERROR: " << buffer << '\n';
	}
}