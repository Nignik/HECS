#pragma once

#include "Component.h"

namespace Hori
{
	class System
	{
	public:
		virtual void Update(float deltaTime) = 0;
	};
}
