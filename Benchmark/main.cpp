#include "../HECS/Core/World.h"
#include <vector>
#include <benchmark/benchmark.h>

using namespace Hori;

struct Foo
{
	int x{}, y{};
};

static void BM_DenseEach(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        Hori::World world;

        constexpr int N = 1'000;
        for (int i = 0; i < N; ++i)
        {
            auto e = world.CreateEntity();
            world.AddComponents(e, Foo{.x = 2, .y = 1});
        }
        state.ResumeTiming();

        world.Each<Foo>([](Hori::Entity, Foo& f)
        {
            benchmark::DoNotOptimize(f.x += 1);
        });
    }
}
BENCHMARK(BM_DenseEach);
