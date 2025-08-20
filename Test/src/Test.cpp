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

	TEST(Systems, ForEach)
	{
		World world;

		constexpr int N = 10;
		std::vector<Entity> entities;
		for (int i = 0; i < N; ++i)
		{
			auto e = world.CreateEntity();
			world.AddComponents(e, Position{.x = 2, .y = 1});
			entities.push_back(e);
		}

		world.Each<Position>([](Entity, Position& p) {
				p.x = 3.f;
		});

		auto pos = world.GetComponent<Position>(entities[0]);
		EXPECT_EQ(pos->x, 3.f);
	}

	TEST(Systems, ParallelForEach)
	{
		World world;

		constexpr int N = 10;
		std::vector<Entity> entities;
		for (int i = 0; i < N; ++i)
		{
			auto e = world.CreateEntity();
			world.AddComponents(e, Position{.x = 2, .y = 1}, Velocity{.x = 4, .y = 5});
			entities.push_back(e);
		}

		world.ParallelEach<Position, Velocity>([](Entity, Position& p, Velocity& v) {
				p.x = 3.f;
				v.x = 10.f;
		});

		auto pos = world.GetComponent<Position>(entities[0]);
		auto vel = world.GetComponent<Velocity>(entities[0]);
		EXPECT_EQ(pos->x, 3.f);
		EXPECT_EQ(vel->x, 10.f);
	}

	TEST(Components, SingletonComponent)
	{
		World world;
		world.AddSingletonComponent(Position{1.2f, 0.3f});

		auto pos = world.GetSingletonComponent<Position>();
		EXPECT_EQ(pos->x, 1.2f);
		EXPECT_EQ(pos->y, 0.3f);
	}

	TEST(Components, RemoveComponent)
	{
		World world;
		Entity e;
		world.AddComponents(e, Position{1.f, 1.f});
		world.RemoveComponents<Position>(e);
		EXPECT_EQ(world.HasComponents<Position>(e), false);
	}
}

