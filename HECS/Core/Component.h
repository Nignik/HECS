#pragma once

#include "Resource.h"

namespace Hori
{
	class Component
	{
		virtual void Create(Resource* resource) = 0;
		virtual ~Component() = default;
	};
}
