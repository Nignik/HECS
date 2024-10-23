#include <Core/Entity.h>
#include <Core/Component.h>
#include <Core/World.h>

#include <gtest/gtest.h>

using namespace Hori;

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
	World world;
	Entity entt = world.CreateEntity();
	world.AddComponent(entt, std::make_shared<Position>(1.5f, 0.5f));
	world.AddComponent(entt, std::make_shared<Velocity>(1.0f, 1.0f));

	auto retrievedPosition = world.GetComponent<Position>(entt);
	auto retrievedVelocity = world.GetComponent<Velocity>(entt);

	EXPECT_NE(retrievedPosition, nullptr);
	EXPECT_NE(retrievedVelocity, nullptr);

	EXPECT_EQ(retrievedPosition->x, 1.5f);
	EXPECT_EQ(retrievedPosition->y, 0.5f);

	EXPECT_EQ(retrievedVelocity->x, 1.0f);
	EXPECT_EQ(retrievedVelocity->y, 1.0f);
}
