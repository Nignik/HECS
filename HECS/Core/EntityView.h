#pragma once

#include "World.h"

#include <tuple>

namespace Hori
{
	template<typename... Components>
	struct EntityView
	{
		using ViewTuple = std::tuple<Entity, Components*...>;
		std::vector<ViewTuple> data;

		auto begin() { return data.begin(); }
		auto end() { return data.end(); }
		auto begin() const { return data.begin(); }
		auto end()   const { return data.end(); }
	};
}