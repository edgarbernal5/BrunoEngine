#pragma once

#include "TrioApiRequisites.h"

#include "Content/Pipeline/ContentItem.h"

namespace BrunoEngine
{
	class GeometryContent;
	class VertexChannel;
	class VertexChannelCollection;
	class IndirectPositionCollection;
	class VertexBufferContent;
	
	class BRUNO_API_EXPORT VertexContent
	{
	public:
		VertexContent(GeometryContent* geometry);
		~VertexContent();

		int Add(int positionIndex);
		void AddRange(const int* positionIndexCollection, uint32_t size);
		VertexBufferContent* CreateVertexBuffer();

		inline VertexChannelCollection* GetChannels()
		{
			return m_channels;
		}

		inline IndirectPositionCollection* GetPositions()
		{
			return m_positions;
		}

		int GetVertexCount();
	private:
		VertexChannelCollection* m_channels;
		VertexChannel* m_positionIndices;
		IndirectPositionCollection* m_positions;
	};
}