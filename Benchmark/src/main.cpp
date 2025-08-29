#include "HECS/Core/World.h"
#include <vector>
#include <benchmark/benchmark.h>
#include "glm/glm.hpp"

using namespace Hori;

struct Foo
{
	int x{}, y{};
};

struct Poo
{
    glm::mat4 m{1.f};
};

static void BM_Each(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        Hori::World world;

        constexpr int N = 1'000'0;
        for (int i = 0; i < N; ++i)
        {
            auto e = world.CreateEntity();
            world.AddComponents(e, Poo{3.f});
        }
        state.ResumeTiming();

        glm::mat4 x{5.f};
        world.Each<Poo>([&x](Hori::Entity, Poo& f)
        {
            benchmark::DoNotOptimize(f.m * x);
        });
    }
}

static void BM_ParallelEach(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        Hori::World world;

        constexpr int N = 1'000'0;
        for (int i = 0; i < N; ++i)
        {
            auto e = world.CreateEntity();
            world.AddComponents(e, Poo{3.f});
        }
        state.ResumeTiming();

        glm::mat4 x{5.f};
        world.ParallelEach<Poo>([&x](Hori::Entity, Poo& f)
        {
            benchmark::DoNotOptimize(f.m * x);
        });
    }
}

BENCHMARK(BM_Each);
BENCHMARK(BM_ParallelEach);
