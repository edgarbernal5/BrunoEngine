#pragma once

#include "TrioApiRequisites.h"

#include "IServiceBase.h"

namespace BrunoEngine
{
	class BRUNO_API_EXPORT IGraphicsDeviceManager : public IServiceBase
	{
	public:
		virtual ~IGraphicsDeviceManager() {}
		virtual bool BeginDraw() = 0;
		virtual void CreateDevice() = 0;
		virtual void EndDraw() = 0;
	};

}
