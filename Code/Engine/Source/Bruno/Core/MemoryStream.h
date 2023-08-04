#pragma once

#include "Stream.h"

namespace Bruno
{
	class MemoryStream : public Stream
	{
	public:
		MemoryStream();
		MemoryStream(uint32_t capacity);
		~MemoryStream();

		uint8_t* GetBuffer() { return m_buffer; }

		long GetLength() override;
		uint64_t GetPosition() override;
		bool IsStreamValid() const override;

		virtual bool Read(uint8_t* destination, size_t count) override;

		virtual void Write(const uint8_t* buffer, size_t count) override;

	private:
		uint8_t* m_buffer;
		uint64_t m_capacity;
		long m_length;
		uint64_t m_position;

		bool EnsureCapacity(uint64_t value);
	};
}


