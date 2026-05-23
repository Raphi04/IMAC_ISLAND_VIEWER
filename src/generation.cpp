#include "generation.hpp"

#include "noise.hpp"
#include "raylib.h"

#include "utils/raylibUtils.hpp"
#include <algorithm> // for std::clamp


std::vector<glm::vec2> generate2DPositions([[maybe_unused]] PointsGenerationParameters const& params) {
    // TODO(student): implement Poisson disk sampling to replace the above naive random generation
    // points output should be in [0..1] range, where (0,0) is one corner of the terrain and (1,1) is the opposite corner, so they can be easily scaled to terrain size and sampled from heightmap.
    
    // Calcul de la taille d'une cellule
    float cellSize = params.rayonMinimal/sqrt(2);
    
    // Points finaux
    std::vector<glm::vec2> positions {};

    // Point actifs
    std::vector<glm::vec2> activePoints {};

    // Grille de points
    std::vector<std::vector<int>> grid(
        ceil(1 / cellSize),
        std::vector<int>(ceil(1 / cellSize))
    );

    // Ajout d'un point au centre de l'ile dans activePoint
    activePoints.emplace_back(0.5f, 0.5f);

    // Tant qu'il y a des points actifs et qu'on a pas atteint un nombre de points maximum
    while (activePoints.size() > 0 && static_cast<int>(positions.size()) < params.nbPointMax) {

        // On prend un point aléatoire dans les points actifs
        std::size_t randomIndex { static_cast<size_t>(GetRandomValue(0, activePoints.size() - 1)) };
        glm::vec2 selectedActivePoint { activePoints[randomIndex] };

        // Booléan qui détermine si le points actifs à encore de la place autour de lui
        bool isFree { false };

        // On essaie n fois de trouver un nouveau point qui fonctionne
        for(int i { 0 }; i < params.nbEssaie; i++) {

            // On prend un angle aléatoire autour du point actif 
            float angle { static_cast<float>(GetRandomValue(0, INT_MAX) / static_cast<float>(INT_MAX) * M_PI * 2) };

            // On prend un rayon aléatoire entre 1 et 2 fois le rayon minimum
            float rayon { static_cast<float>((1 + GetRandomValue(0, INT_MAX) / static_cast<float>(INT_MAX)) * params.rayonMinimal) };

            // On détermine la position du point candidat
            glm::vec2 direction { cos(angle) * rayon, sin(angle) * rayon};
            glm::vec2 candidate { selectedActivePoint + direction };

            // Booléan permettant de savoir si le point est valide
            bool isValid { true };

            // On vérifie que le point est comprit dans la nos limites
            if(candidate.x >= 0 && candidate.x <= 1 && candidate.y >= 0 && candidate.y <= 1) {

                // Point candidat dans la grille
                int cellX { static_cast<int>(candidate.x / cellSize) };
                int cellY { static_cast<int>(candidate.y / cellSize) };

                // Variable pour la recherche dans la grille
                int searchStartX = std::max(0, cellX - 2);
                int searchEndX = std::min(cellX + 2, static_cast<int>(grid.size() - 1));
                int searchStartY = std::max(0, cellY - 2);
                int searchEndY = std::min(cellY + 2, static_cast<int>(grid.size() - 1));

                // Recherche autour du point candidat
                for(int x { searchStartX }; x <= searchEndX; x++) {
                    for(int y { searchStartY }; y <= searchEndY; y++) {
                        int pointGrille { grid[x][y] - 1 };
                        if(pointGrille != -1) {
                            if(glm::distance(candidate, positions[pointGrille]) < params.rayonMinimal) {
                                isValid = false;
                                break;
                            }
                        }

                    }
                }

                // Si le point candidat est valide on l'ajoute aux points finaux et actifs
                if(isValid) {
                    positions.push_back(candidate);
                    activePoints.push_back(candidate);
                    grid[static_cast<int>(candidate.x / cellSize)][static_cast<int>(candidate.y / cellSize)] = positions.size();
                    isFree = true;
                    break;
                }

            } else {
                continue;
            }
        }
        
        // Si le point n'est pas de place autour de lui, on le retire des points actifs
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