#include "stdafx.h"
#include "main.h"

#include <Graphics/SimpleMath.h>

#include <Graphics/GraphicsDevice.h>
#include <Graphics/GraphicsDeviceManager.h>
#include <Graphics/Effect.h>


#include <Graphics\IndexBuffer.h>
#include <Graphics\VertexBuffer.h>
#include <Graphics\VertexPositionColor.h>
#include <Graphics\VertexPosition.h>

#include <memory>
#include <iostream>

#pragma warning(disable : 4996)

DemoGame::DemoGame() :
	Game(), m_pIndexBuffer(nullptr)
{

	GraphicsDeviceManager* graphicsManager = new GraphicsDeviceManager(this);
}

void DemoGame::Initialize()
{
	m_pEffect = new Effect(GetGraphicsDevice(), "LineEffect.fx");

	m_pIndexBuffer = new IndexBuffer(GetGraphicsDevice(), IndexElementSize::ThirtyTwoBits, 36, ResourceUsage::Immutable);

	uint32_t indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	m_pIndexBuffer->SetData<uint32_t>(indices, 36);

	DirectX::SimpleMath::Color whiteColor(1, 1, 1);
	DirectX::SimpleMath::Color blackColor(0, 0, 0);
	DirectX::SimpleMath::Color redColor(1, 0, 0);
	DirectX::SimpleMath::Color greenColor(0, 1, 0);
	DirectX::SimpleMath::Color blueColor(0, 0, 1);
	DirectX::SimpleMath::Color yellowColor(1, 0, 1);
	DirectX::SimpleMath::Color cyanColor(0, 1, 1);
	DirectX::SimpleMath::Color magentaColor(0, 0.5f, 1);

	VertexPositionColor vertexs[] =
	{
		{ DirectX::SimpleMath::Vector3(-1.0f, -1.0f, -1.0f), whiteColor },
		{ DirectX::SimpleMath::Vector3(-1.0f, +1.0f, -1.0f), blackColor },
		{ DirectX::SimpleMath::Vector3(+1.0f, +1.0f, -1.0f), redColor },
		{ DirectX::SimpleMath::Vector3(+1.0f, -1.0f, -1.0f), greenColor },
		{ DirectX::SimpleMath::Vector3(-1.0f, -1.0f, +1.0f), blueColor },
		{ DirectX::SimpleMath::Vector3(-1.0f, +1.0f, +1.0f), yellowColor },
		{ DirectX::SimpleMath::Vector3(+1.0f, +1.0f, +1.0f), cyanColor },
		{ DirectX::SimpleMath::Vector3(+1.0f, -1.0f, +1.0f), magentaColor }
	};

	int sizeVPC = sizeof(VertexPosition);
	m_pVertexBuffer = new VertexBuffer(GetGraphicsDevice(), VertexPositionColor::GetVertexDeclaration(), 8);
	m_pVertexBuffer->SetData<VertexPositionColor>(vertexs, 8);
}

void DemoGame::Draw(DX::StepTimer const & timer)
{
	DirectX::SimpleMath::Color black(0.0, 1.0f, 1.0f);
	GetGraphicsDevice()->Clear(black);
}

void DemoGame::Update(DX::StepTimer const& timer)
{
	std::cout << "Update: dt = " << timer.GetElapsedSeconds() << "\t Total seconds: " << timer.GetTotalSeconds() << endl;
}


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	freopen("ouput.txt", "w", stdout);

	std::unique_ptr<DemoGame> game(new DemoGame());


	game->Run();

	return 0;
}