#pragma once

#include "raylib.h"
#include "glm/glm.hpp"
#include <vector>

struct ImageGenerationParameters {
    int noiseSeed { 0 };
    float noiseScale { 4.0f };
    int resolution { 256 };
    float minHeightObject{ 0.25 };
    float maxHeightObject{0.9};

    //For Gaussian Mask
    float esperance = 0.1f;
    float ecarttype = 0.3f;
};

struct PointsGenerationParameters {
    // TODO(student): add parameters for points generation (ex: poisson disk radius, etc).
    // Poisson Disk Sampling
    int nbEssaie { 4 };
    float rayonMinimal { 0.04 }; 
    int nbPointMax { 500 }; 
};

struct NoiseGenerationParameters {
    // nombre de couches de bruits
    int nombreOctave{};
    // multiplicateur de fréquence à chaque octave
    float lacuranite{};
    // Le multiplicateur d'amplitude à chaque octave. Si le gain vaut 0.5, chaque octave successive aura deux fois moins d'impact visuel que la précédente. Les détails fins ne viennent pas gâcher la forme globale.
    float gain{};
    // zoom initial sur le bruit
    float scale{};
};

struct AppContext {
    Camera camera {};

    // Store the heightmap as a raylib Image, which is easy to sample from CPU side when generating object positions.
    Image heightmapImage {};

    // This is the image we use for texturing the terrain. It can be the same as heightmapImage, but it doesn't have to be (for example, you could use a color image with RGB channels representing different material types instead of height).
    Image image {};

    // The generated texture from the image, stored here so we can easily bind it when generating the model.
    Texture2D texture {};

    glm::vec3 terrainSize { 16.0f, 5.0f, 16.0f };

    // The generated terrain mesh and model.
    Mesh mesh {};
    Model model {};

    std::vector<glm::vec3> objectPositions {};

    // A simple cube mesh and material we use to draw objects on the terrain.
    Mesh cube {};
    Material cubeMaterial {};
    float cubeScale { 0.1f };

    // Parameters for object positions generation
    PointsGenerationParameters pointsGenerationParameters;

    // Parameters for noise generation
    NoiseGenerationParameters noiseGenerationParameters;

    // Parameters for island generation
    ImageGenerationParameters imageGenerationParameters;

};

Matrix getTerrainCenteringMatrix(AppContext const& context);
float sampleHeightmap(AppContext const& context, float u, float v);
void unload(AppContext& context);
void regenerateMeshFromImage(AppContext& context);