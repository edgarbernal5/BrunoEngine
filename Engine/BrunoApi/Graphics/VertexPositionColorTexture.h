#pragma once

#include "TrioApiRequisites.h"

#include "../Math/MathVector.h"
#include "VertexElement.h"

namespace BrunoEngine
{
	class VertexDeclaration;

	// Vertex struct holding position, color, and texture mapping information.
	struct BRUNO_API_EXPORT VertexPositionColorTexture
	{
		VertexPositionColorTexture() = default;

		VertexPositionColorTexture(const VertexPositionColorTexture&) = default;
		VertexPositionColorTexture& operator=(const VertexPositionColorTexture&) = default;

		VertexPositionColorTexture(VertexPositionColorTexture&&) = default;
		VertexPositionColorTexture& operator=(VertexPositionColorTexture&&) = default;

		VertexPositionColorTexture(Vector3 const& position, ColorRGBA8 const& color, Vector2 const& textureCoordinate)
			: m_position(position),
			m_color(color),
			m_textureCoordinate(textureCoordinate)
		{ }

#ifdef BRUNO_DIRECTX
		VertexPositionColorTexture(DirectX::FXMVECTOR position, DirectX::FXMVECTOR color, DirectX::CXMVECTOR textureCoordinate)
		{
			XMStoreFloat3(&this->m_position, position);
			XMStoreColor(&this->m_color, color);
			XMStoreFloat2(&this->m_textureCoordinate, textureCoordinate);
		}
#endif
		const Vector3& GetPosition() { return m_position; }
		const ColorRGBA8& GetColor() { return m_color; }
		const Vector2& GetTexCoord() { return m_textureCoordinate; }

		static const int InputElementCount = 3;
#ifdef BRUNO_DIRECTX
		static const D3D11_INPUT_ELEMENT_DESC InputElementsD3D11[InputElementCount];
#endif
		static const VertexElement InputElements[InputElementCount];

		static VertexDeclaration* GetVertexDeclaration();

	private:
		Vector3 m_position;
		ColorRGBA8 m_color;
		Vector2 m_textureCoordinate;

		static VertexDeclaration* g_vertexDeclaration;
	};
}
