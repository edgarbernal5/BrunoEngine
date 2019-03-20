#pragma once

#include <stdint.h>

namespace Vago
{
	int ComputeHash(const char * data, int size);
	int ComputeHash(uint8_t * data, int size);

	int ComputeHash(uint8_t * data, size_t size);
}
