#pragma once

#include "TrioApiRequisites.h"

#include <string>

namespace TrioEngine
{
	class ContentItem;
	class ContentWriter;
	class ContentCompiler;

	class ContentTypeWriter
	{
	public:
		ContentTypeWriter() {}
		virtual ~ContentTypeWriter(){}

		virtual std::string GetWriterName() = 0;
		virtual std::string GetReaderName() = 0;

		virtual void Initialize(ContentCompiler *compiler) {}
		virtual void Write(ContentWriter *output, ContentItem* value) = 0;

		bool operator <(const ContentTypeWriter &other) const
		{
			return m_Name < other.m_Name;
		}
		bool operator <(const ContentTypeWriter *other) const
		{
			return m_Name < other->m_Name;
		}
	protected:
		std::string m_Name;
	};
}