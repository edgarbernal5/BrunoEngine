#pragma once

#include <zlib.h>
#include "Stream.h"

namespace Bruno
{
	class MemoryStream;

	class Compressor
	{
	public:
		Compressor(Stream& outputStream);
		~Compressor();

		void Compress(uint8_t* inputBytes, unsigned long sizeInBytes);
		void FlushOutput();

	private:
		Stream& m_outputStream;
		uint8_t* m_buffer;
		uint32_t m_capacity;
		uint32_t m_length;
	};

	class Decompressor
	{
	public:
		Decompressor(MemoryStream* outputStream);

		void Decompress(Stream* inputStream, int compressedLength, int decompressedLength);

	private:
		MemoryStream* m_outputStream;
	};
}

