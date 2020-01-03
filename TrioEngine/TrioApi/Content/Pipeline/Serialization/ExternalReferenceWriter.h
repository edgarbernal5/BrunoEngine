#pragma once

#include "TrioApiRequisites.h"

#include "ContentTypeWriter.h"
#include <string>

namespace TrioEngine
{
	class ExternalReferenceWriter : public ContentTypeWriter
	{
	public:
		ExternalReferenceWriter();
		~ExternalReferenceWriter();

		std::string GetWriterName() { m_Name = "ExternalReferenceWriter"; return m_Name; }
		std::string GetReaderName() { return "ExternalReferenceReader"; }
		void Write(ContentWriter *output, ContentItem* value);
	};
}