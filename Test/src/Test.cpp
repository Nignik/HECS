#include "../../HECS/Core/World.h"

#include <gtest/gtest.h>

namespace Hori
{
	struct Position
	{
		float x, y;
	};

	struct Velocity
	{
		float x, y;
	};

	class Ecs
	{
	public:
		Ecs(const Ecs&) = delete;
		Ecs& operator=(const Ecs&) = delete;

		static World& GetInstance()
		{
			static Ecs instance;
			return instance.m_world;
		}

	private:
		Ecs() = default;
		~Ecs() = default;

		World m_world{};
	};

	
	TEST(Systems, ForEach)
	{
		Hori::World world;

		constexpr int N = 10;
		std::vector<Entity> entities;
		for (int i = 0; i < N; ++i)
		{
			auto e = world.CreateEntity();
			world.AddComponents(e, Position{.x = 2, .y = 1});
			entities.push_back(e);
		}

		world.Each<Position>([](Hori::Entity, Position& p)
		{
				p.x = 0;
		});

		auto pos = world.GetComponent<Position>(entities[0]);
		EXPECT_EQ(pos->x, 0.f);
	}


}

