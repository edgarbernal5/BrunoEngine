#pragma once

#include "TrioApiRequisites.h"

namespace BrunoEngine
{
	// Defines how "GraphicsDevice.Present updates the game window.
	enum class PresentInterval
	{
		//Equivalent to "PresentInterval.One".
		Default = 0,

		//The driver waits for the vertical retrace period, before updating window client area. 
		//Present operations are not affected more frequently than the screen refresh rate.
		One = 1,

		//The driver waits for the vertical retrace period, before updating window client area.
		//Present operations are not affected more frequently than every second screen refresh.
		Two = 2,

		//The driver updates the window client area immediately. Present operations might be
		//affected immediately. There is no limit for framerate.
		Immediate = 3,
	};
}
