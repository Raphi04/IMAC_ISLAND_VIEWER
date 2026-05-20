#include "random.hpp"
#include <random>
#include <stdexcept>
// Jules a dit que je pouvais copier :)
namespace p6::random {

static auto& generator()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    return gen;
}

float number()
{
    return number(0.f, 1.f);
}

float number(float max)
{
    return number(0.f, max);
}

float number(float min, float max)
{
    if (min > max)
        throw std::invalid_argument{"`min` must be smaller than `max`"};

    auto distribution = std::uniform_real_distribution<float>{min, max};
    return distribution(generator());
}

int integer(int max)
{
    return integer(0, max);
}

int integer(int min, int max)
{
    if (min >= max)
        throw std::invalid_argument{"`min` must be strictly smaller than `max`"};

    auto distribution = std::uniform_int_distribution<int>{min, max - 1};
    return distribution(generator());
}

size_t size_type(size_t max)
{
    return size_type(0u, max);
}

size_t size_type(size_t min, size_t max)
{
    if (min >= max)
        throw std::invalid_argument{"`min` must be strictly smaller than `max`"};

    auto distribution = std::uniform_int_distribution<size_t>{min, max - 1};
    return distribution(generator());
}

} // namespace p6::random