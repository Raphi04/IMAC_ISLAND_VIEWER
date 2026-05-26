#pragma once

#include "app.hpp"
#include "raylib.h"

void draw3DScene(AppContext& context);
void drawRaylibUI(AppContext& context);
void drawCubes(AppContext const& context, Matrix const& terrainCentering);
void drawSelectedModel(AppContext const& context, Vector3 const& terrainCentering);

void drawImGui(AppContext& context);