#include "generation.hpp"

#include "noise.hpp"
#include "raylib.h"

#include "utils/raylibUtils.hpp"
#include <algorithm> // for std::clamp


std::vector<glm::vec2> generate2DPositions([[maybe_unused]] PointsGenerationParameters const& params) {
    std::vector<glm::vec2> positions {};

    // positions.reserve(params.nbPointMax);
    // // Naive random generation
    // for (int i {0}; i < params.nbPointMax; ++i)
    // {
    //     positions.emplace_back(
    //         static_cast<float>(GetRandomValue(0, INT_MAX)) / static_cast<float>(INT_MAX),
    //         static_cast<float>(GetRandomValue(0, INT_MAX)) / static_cast<float>(INT_MAX)
    //     );
    // }

    // TODO(student): implement Poisson disk sampling to replace the above naive random generation
    // points output should be in [0..1] range, where (0,0) is one corner of the terrain and (1,1) is the opposite corner, so they can be easily scaled to terrain size and sampled from heightmap.
    std::vector<glm::vec2> activePoints {};

    // Ajout d'un point aléatoire dans activePoint
    activePoints.emplace_back(
        static_cast<float>(GetRandomValue(0, INT_MAX)) / static_cast<float>(INT_MAX),
        static_cast<float>(GetRandomValue(0, INT_MAX)) / static_cast<float>(INT_MAX)
    );

    while (activePoints.size() > 0 && activePoints.size() < params.nbPointMax) {
       std::size_t randomIndex { static_cast<size_t>(GetRandomValue(0, activePoints.size() - 1)) };
        
        glm::vec2 selectedActivePoint { activePoints[randomIndex] };

        bool isFree { false };

        for(int i { 0 }; i < params.nbEssaie; i++) {
            float angle { static_cast<float>(GetRandomValue(0, INT_MAX) / static_cast<float>(INT_MAX) * M_PI * 2) };
            float rayon { static_cast<float>((1 + GetRandomValue(0, INT_MAX) / static_cast<float>(INT_MAX)) * params.rayonMinimal) };

            glm::vec2 direction { cos(angle) * rayon, sin(angle) * rayon};
            glm::vec2 candidate { selectedActivePoint + direction };

            bool isValid { true };

            if(candidate.x >= 0 && candidate.x <= 1 && candidate.y >= 0 && candidate.y <= 1) {

                for(int i { 0 }; i < positions.size(); i++) {
                    glm::vec2 currentPoint { positions[i] };

                    if(glm::distance(candidate, currentPoint) < params.rayonMinimal) {
                        isValid = false;
                        break;
                    }
                }

                if(isValid) {
                    positions.push_back(candidate);
                    activePoints.push_back(candidate);
                    isFree = true;
                    break;
                }

            } else {
                continue;
            }
        }
        
        if(!isFree) {
            activePoints.erase(activePoints.begin() + randomIndex);
        }
    }

    return positions;
}

void generateObjectsPositions(AppContext& context) {
    std::vector<glm::vec2> const positions {generate2DPositions(context.pointsGenerationParameters)};

    context.objectPositions.clear();
    context.objectPositions.reserve(positions.size());
    for (glm::vec2 const& p : positions)
    {
        context.objectPositions.emplace_back(
            p.x, // x
            p.y, // y
            // sample height from heightmap for each point (asuming positions are normalized in [0..1] range)
            sampleHeightmap(context, p.x, p.y)
        );
    }
    // TODO(student): extension - filter positions by sampled height range.
}

float sampleHeightmap(AppContext const& context, float u, float v)
{
    if (!context.heightmapImage.data || context.heightmapImage.width <= 0 || context.heightmapImage.height <= 0) return 0.0f;

    int const px = std::clamp(static_cast<int>(u * static_cast<float>(context.heightmapImage.width - 1)), 0, context.heightmapImage.width - 1);
    int const py = std::clamp(static_cast<int>(v * static_cast<float>(context.heightmapImage.height - 1)), 0, context.heightmapImage.height - 1);

    // If the heightmap is in R32 format, we can directly read the height value as a float. 
    if (context.heightmapImage.format == PIXELFORMAT_UNCOMPRESSED_R32)
    {
        float const* heightData = static_cast<float const*>(context.heightmapImage.data);
        int const idx = py * context.heightmapImage.width + px;
        return std::clamp(heightData[idx], 0.0f, 1.0f);
    }

    // Otherwise, we assume it's in a color format and we read the red channel as height (with normalization from [0..255] to [0..1]).
    Color const c = GetImageColor(context.heightmapImage, px, py);
    return static_cast<float>(c.r)/255.0f;
}

void generateHeightmap(AppContext& context) {

    if (context.texture.id > 0) {
        UnloadTexture(context.texture);
        context.texture = {};
    }

    if(context.image.data) {
        UnloadImage(context.image);
        context.image = {};
    }

    if (context.heightmapImage.data) {
        UnloadImage(context.heightmapImage);
        context.heightmapImage = {};
    }

    int const resolution = std::max(1, context.imageGenerationParameters.resolution);

    context.heightmapImage = GenImageFromNoiseFunction<float>(resolution, resolution, PIXELFORMAT_UNCOMPRESSED_R32,
        [&](glm::vec2 const& p)->float {
            // TODO(student): implement stack based noise and island mask

            return (perlinNoiseSeeded(p * context.imageGenerationParameters.noiseScale, context.imageGenerationParameters.noiseSeed) * 0.5f + 0.5f);
        });

    // exemple conversion from heightmap to color image
    context.image = TransformImage<float, Color>(context.heightmapImage, [&](float const& v, int const, int const) {
        if (v < 0.3f)
        {
            return color_from({ 70, 130, 180 }); // water
        }
        else if (v < 0.5f)
        {
            return color_from({ 238, 214, 175 }); // sand
        }
        else
        {
            return color_from({ 34, 139, 34 }); // grass
        }
        
    }, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    context.texture = LoadTextureFromImage(context.image);
    if (context.model.meshCount > 0) {
        context.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = context.texture;
    }
}