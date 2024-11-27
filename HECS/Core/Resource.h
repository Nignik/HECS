#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace Hori
{
	class Resource
	{
	public:
		virtual ~Resource() = default;
		virtual void Load(const fs::path filepath) = 0;
	};
}