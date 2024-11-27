#include <Core/Entity.h>
#include <Core/Component.h>
#include <Core/World.h>

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

	TEST(HECSTest, CanAddComponents)
	{
		World& world = World::GetInstance();

		Entity entt = world.CreateEntity();
		world.AddComponent(entt, Position(1.5f, 0.5f));
		world.AddComponent(entt, Velocity(1.0f, 1.0f));

		auto retrievedPosition = world.GetComponent<Position>(entt);
		auto retrievedVelocity = world.GetComponent<Velocity>(entt);

		EXPECT_EQ(retrievedPosition.x, 1.5f);
		EXPECT_EQ(retrievedPosition.y, 0.5f);

		EXPECT_EQ(retrievedVelocity.x, 1.0f);
		EXPECT_EQ(retrievedVelocity.y, 1.0f);
	}

	TEST(HECSTest, CanGetEntitiesByComponents)
	{
		World& world = World::GetInstance();

		Entity entt = world.CreateEntity();
		world.AddComponent(entt, Position(1.5f, 0.5f));
		world.AddComponent(entt, Velocity(1.0f, 1.0f));

		auto retrievedEntities = world.GetEntitiesWithComponents<Position>();

		EXPECT_EQ(retrievedEntities[1].GetID(), entt.GetID());

	}

}

