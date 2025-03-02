#include <Entity.h>
#include <World.h>

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

	TEST(HECSTest, CanAddComponents)
	{
		World& world = Ecs::GetInstance();

		Entity entt = world.CreateEntity();
		world.AddComponents(entt, Position(1.5f, 0.5f));
		world.AddComponents(entt, Velocity(1.0f, 1.0f));

		auto retrievedPosition = world.GetComponent<Position>(entt);
		auto retrievedVelocity = world.GetComponent<Velocity>(entt);

		EXPECT_EQ(retrievedPosition->x, 1.5f);
		EXPECT_EQ(retrievedPosition->y, 0.5f);

		EXPECT_EQ(retrievedVelocity->x, 1.0f);
		EXPECT_EQ(retrievedVelocity->y, 1.0f);
	}

	TEST(HECSTest, CanGetEntitiesByComponents)
	{
		World& world = Ecs::GetInstance();

		Entity entt = world.CreateEntity();
		world.AddComponents(entt, Position(1.5f, 0.5f));
		world.AddComponents(entt, Velocity(1.0f, 1.0f));

		auto retrievedEntities = world.GetEntitiesWith<Position>();

		EXPECT_EQ(retrievedEntities[1].GetID(), entt.GetID());
	}

	TEST(HECSTest, SingletonComponent)
	{
		World& world = Ecs::GetInstance();

		world.AddSingletonComponent(Position(1.5f, 0.5f));

		auto retrievedComponent = world.GetSingletonComponent<Position>();

		EXPECT_EQ(retrievedComponent->x, 1.5f);
	}

	TEST(HECSTest, RemovingEntities)
	{
		World& world = Ecs::GetInstance();

		auto entt = world.CreateEntity();
		world.AddComponents(entt, Position(1.5f, 0.5f));

		world.RemoveEntity(entt);

		EXPECT_EQ(0, 0);
	}

	TEST(EntityCloning, EntityClone)
	{
		World& world = Ecs::GetInstance();

		auto entt = world.CreateEntity();
		world.AddComponents(entt, Position(1.5f, 0.5f));

		auto clonedEntity = world.Clone(entt);

		EXPECT_EQ(world.GetComponent<Position>(entt)->x, world.GetComponent<Position>(clonedEntity)->x);
	}

	TEST(EntityViewing, EntityViewing)
	{
		World& world = Ecs::GetInstance();

		auto entt = world.CreateEntity();
		world.AddComponents(entt, Position(1.5f, 0.5f));
		entt = world.CreateEntity();
		world.AddComponents(entt, Position(2.0f, 0.5f));
		
		std::vector<Position*> positions;
		for (auto [entity, pos] : world.GetViewOfEntitiesWith<Position>())
		{
			positions.push_back(pos);
		}

		EXPECT_EQ(positions[0]->x, 1.5f);
	}

}

