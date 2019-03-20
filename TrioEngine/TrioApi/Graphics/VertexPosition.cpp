#include "stdafx.h"
#include "VertexPosition.h"

#include "VertexDeclarationHelper.h"

namespace Vago
{
	VertexDeclaration* VertexPosition::g_pVertexDeclaration = nullptr;

	//--------------------------------------------------------------------------------------
	// Vertex struct holding position information.
#ifdef TRIO_DIRECTX
	const D3D11_INPUT_ELEMENT_DESC VertexPosition::InputElementsD3D11[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
#endif
	const VertexElement VertexPosition::InputElements[] =
	{
		{ 0, VertexElementFormat::Vector3, VertexElementUsage::Position, 0 }
	};

	VertexDeclaration* VertexPosition::GetVertexDeclaration()
	{
		if (g_pVertexDeclaration == nullptr)
		{
#ifdef TRIO_DIRECTX
			g_pVertexDeclaration = VertexDeclarationHelper::GetVertexDeclaration(InputElementCount, InputElementsD3D11, InputElements);
#else
			g_pVertexDeclaration = VertexDeclarationHelper::GetVertexDeclaration(InputElementCount, InputElements);
#endif
			VertexDeclarationHelper::AddVertexDeclaration(typeid(VertexPosition).name(), g_pVertexDeclaration);
		}

		return g_pVertexDeclaration;
	}
}
