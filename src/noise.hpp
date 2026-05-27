#pragma once

#include <glm/glm.hpp>
#include <functional>
#include <app.hpp>

float perlinNoise(glm::vec2 const& position);
float perlinNoiseSeeded(glm::vec2 const& position, int seed);

float octaveNoise(glm::vec2 const& position, int const& seed, std::function<float(glm::vec2 const&, int const&)> noiseFunction, NoiseGenerationParameters const& noiseGenerationParameters);