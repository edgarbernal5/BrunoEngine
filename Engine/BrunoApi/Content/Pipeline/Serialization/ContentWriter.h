#pragma once

#include "TrioApiRequisites.h"

#include "Content/Pipeline/ExternalReference.h"
#include "IO/BinaryWriter.h"
#include <map>
#include <string>
#include <vector>
#include <queue>

namespace TrioIO
{
	class Stream;
	class MemoryStream;
}

namespace BrunoEngine
{
	class ContentCompiler;
	class ContentItem;
	class ContentTypeWriter;

	class ContentWriter : public TrioIO::BinaryWriter
	{
	public:
		ContentWriter(ContentCompiler* compiler, TrioIO::Stream* _stream, bool compressContent, std::string rootDirectory, std::string referenceRelocationPath);
		~ContentWriter();

		void Write(const Matrix& value);
		void Write(const Vector2& value);
		void Write(const Vector3& value);
		void Write(const Vector4& value);

		//template <ContentItem* T>
		void WriteObject(ContentItem* value);
		void WriteSharedResource(ContentItem* resource);

		void WriteExternalReference(ExternalReference& const reference);

		void BeginWrite();
		void FlushOutput();
	private:
		const int HeaderSize = 6;
		const char* FileExtension = "bruno";

		std::string m_rootDirectory;
		std::string m_referenceRelocationPath;

		ContentCompiler *m_compiler;

		std::map<std::string, int> m_typeTable;
		std::vector<ContentTypeWriter *> m_typeWriters;

		ContentTypeWriter* GetTypeWriter(std::string name, int &typeIndex);

		void WriteFinalOutput();
		void WriteCompressedOutput();
		void WriteUncompressedOutput();
		void WriteHeader();
		void WriteSharedResources();

		TrioIO::MemoryStream* m_headerData;
		TrioIO::MemoryStream* m_contentData;

		TrioIO::Stream* m_finalOutput;

		//std::map<int, int> m_SharedResourceNames;
		std::map<ContentItem*, int> m_sharedResourceNames;
		std::queue<ContentItem*> m_sharedResources;

		std::map<ContentItem*, bool> m_recurseDetector;
	};
}