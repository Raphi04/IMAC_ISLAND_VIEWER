#include "draw.hpp"

#include "app.hpp"

#include "generation.hpp"
#include "third_party/random.hpp"
#include "imgui.h"
#include "raylib.h"
#include "raymath.h"

#include "biome.hpp"

void draw3DScene(AppContext& context) {
    ClearBackground(RAYWHITE);
    
    BeginMode3D(context.camera);

    Matrix const terrainCentering { getTerrainCenteringMatrix(context) };
    Vector3 const terrainCenterOffset { terrainCentering.m12, terrainCentering.m13, terrainCentering.m14 };

    DrawModel(context.model, terrainCenterOffset, 1.0f, WHITE);
    //drawCubes(context, terrainCentering);
    drawSelectedModel(context, terrainCenterOffset);
    DrawGrid(20, 1.0f);

    EndMode3D();
}

void drawSelectedModel(AppContext const& context,  Vector3 const& terrainCenterOffset) {
    for (glm::vec3 const& pos : context.objectPositions) {
        
        Vector3 position { 
            pos.x * context.terrainSize.x + terrainCenterOffset.x,
            pos.z * context.terrainSize.y + 0.5f * context.cubeScale + terrainCenterOffset.y,
            pos.y * context.terrainSize.z + terrainCenterOffset.z
        };
        
        DrawModel(context.selectedModel, position, context.cubeScale, WHITE);
    }
}

void drawCubes(AppContext const& context, Matrix const& terrainCentering)
{
    if (context.objectPositions.empty()) {
        return;
    }

    float const cubeHalfHeight { 0.5f * context.cubeScale };

    for (glm::vec3 const& pos : context.objectPositions) {
        Matrix const objectTranslation { MatrixTranslate(
            pos.x * context.terrainSize.x,
            pos.z * context.terrainSize.y + cubeHalfHeight,
            pos.y * context.terrainSize.z
        )};
        Matrix const centeredTranslation { MatrixMultiply(objectTranslation, terrainCentering) };
        Matrix const scale { MatrixScale(context.cubeScale, context.cubeScale, context.cubeScale) };
        Matrix const transform { MatrixMultiply(scale, centeredTranslation) };
        DrawMesh(context.cube, context.cubeMaterial, transform);
    }
}

void RegenerateMap(AppContext& context)
{
        generateHeightmap(context);
        regenerateMeshFromImage(context);
        generateObjectsPositions(context);
}

void drawImGui(AppContext& context) {
    if (ImGui::CollapsingHeader("Randomizer", ImGuiTreeNodeFlags_DefaultOpen)) {

        if (ImGui::Button("Random positions")) {
            generateObjectsPositions(context);
        }
        if (ImGui::Button("Random map seed")) {
            context.imageGenerationParameters.noiseSeed = p6::random::integer(0, 1000);
            RegenerateMap(context);
        }

        if (ImGui::Button("Random map layout")) {
            context.imageGenerationParameters.esperance = p6::random::number(0.01f, 1.0f);
            context.imageGenerationParameters.ecarttype = p6::random::number(0.01f, 1.0f);
            RegenerateMap(context);
        }

        if (ImGui::Button("Random biome")) {
            selectBiome = p6::random::integer(0, biomes.size());
            RegenerateMap(context);
        }

        if (ImGui::Button("Random height object")) {
            context.imageGenerationParameters.minHeightObject = p6::random::number(0.f, 0.5f);
            context.imageGenerationParameters.maxHeightObject = p6::random::number(0.5f, 1.f);
            RegenerateMap(context);
        }
    }

    
    if (ImGui::CollapsingHeader("Biomes", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::Button("Plaine")) {
            selectBiome = 0;
            RegenerateMap(context);
        }

        if (ImGui::Button("Arctique")) 
        {
            selectBiome = 1;
            RegenerateMap(context);
        }
        if (ImGui::Button("Desert")) 
        {
            selectBiome = 2;
            RegenerateMap(context);
        }
        if (ImGui::Button("Magma")) 
        {
            selectBiome = 3;
            RegenerateMap(context);
        }
        if (ImGui::Button("The End")) 
        {
            selectBiome = 4;
            RegenerateMap(context);
        }
        if (ImGui::Button("Mesa")) 
        {
            selectBiome = 5;
            RegenerateMap(context);
        }
    }

    if (ImGui::CollapsingHeader("Selection Modele 3D", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::SliderInt("Modele 3D", &context.selectedModelIndex, 1, 4)) {
            switch (context.selectedModelIndex) {
                case 1:
                    context.selectedModel = LoadModel("../../resources/ado.glb");
                    break;
                
                case 2 :
                    context.selectedModel = LoadModel("../../resources/teto.glb");
                    break;

                case 3 :
                    context.selectedModel = LoadModel("../../resources/capy.glb");
                    break;

                case 4 :
                    context.selectedModel = LoadModel("../../resources/enderman.glb");
                    break;

                default:
                    context.selectedModel = LoadModel("../../resources/ado.glb");
                    break;
            }
            RegenerateMap(context);
        }
    }

    if (ImGui::CollapsingHeader("Poisson Disk Sampling", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::SliderInt("Nombre d'essaie avant rejet", &context.pointsGenerationParameters.nbEssaie, 1.f, 15.f)) {
            RegenerateMap(context);
        }

        if (ImGui::SliderInt("Seed", &context.imageGenerationParameters.noiseSeed, 0, 1000)) {
            RegenerateMap(context);
        }

        if (ImGui::SliderFloat("Rayon minimal", &context.pointsGenerationParameters.rayonMinimal, 0.01f, 0.05f)) {
            RegenerateMap(context);
        }

        if (ImGui::SliderInt("Nombre de point maximum", &context.pointsGenerationParameters.nbPointMax, 1.f, 1000.0f)) {
            RegenerateMap(context);
        }
    }

    if (ImGui::CollapsingHeader("Objects", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::SliderFloat("Cube Scale", &context.cubeScale, 0.01f, 1.0f)) {
            RegenerateMap(context);
        }

        if (ImGui::SliderFloat("Esperance", &context.imageGenerationParameters.esperance, 0.01f, 1.0f)) {
            RegenerateMap(context);
        }
        if (ImGui::SliderFloat("Ecart type", &context.imageGenerationParameters.ecarttype, 0.01f, 1.0f)) {
            RegenerateMap(context);
        }
        if (ImGui::SliderFloat("Hauteur minimale", &context.imageGenerationParameters.minHeightObject, 0.f, 0.5f)) {
            RegenerateMap(context);
        }
        if (ImGui::SliderFloat("Hauteur maximale", &context.imageGenerationParameters.maxHeightObject, 0.5f, 1.f)) {
            RegenerateMap(context);
        }
    }

    if (ImGui::CollapsingHeader("noise", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SliderInt("Octave Number", &context.noiseGenerationParameters.nombreOctave, -10, 10);
        ImGui::SliderFloat("Amplitude", &context.noiseGenerationParameters.gain, 0., 1.);
        ImGui::SliderFloat("Lacuranite", &context.noiseGenerationParameters.lacuranite, -10., 10.);
        ImGui::SliderFloat("Scale", &context.noiseGenerationParameters.scale, -10., 10.);
        ImGui::SliderFloat("Seed", &context.noiseGenerationParameters.seed, -10., 10.);
    }

    if(ImGui::Button("Generate random seeds")) {
        // generateRandomNoise(context);
    }

}


void drawRaylibUI(AppContext& context) {
    int screenWidth { GetScreenWidth() };
    
    float wanted_size { 400.f };
    float scale_factor { wanted_size / std::max(context.texture.width, context.texture.height) };
    float const preview_x { screenWidth - wanted_size - 20.f };
    float const preview_y { 20.f };
    float const preview_w { context.texture.width * scale_factor };
    float const preview_h { context.texture.height * scale_factor };
    // DrawTexture(context.texture, screenWidth - context.texture.width - 20, 20, WHITE);
    DrawTextureEx(context.texture, { preview_x, preview_y }, 0.0f, scale_factor, WHITE);
    DrawRectangleLines(screenWidth - wanted_size - 20, 20, wanted_size, wanted_size, GREEN);

    //draw positions on top of the heightmap
    for (auto const& pos : context.objectPositions)
    {
        // Remap normalized coordinates [0..1] to the preview image in screen space.
        float const px { preview_x + Clamp(pos.x, 0.0f, 1.0f) * preview_w };
        float const py { preview_y + Clamp(pos.y, 0.0f, 1.0f) * preview_h };

        DrawCircleV({ px, py }, 2.0f, RED);
    }

    DrawFPS(10, 10);
}

