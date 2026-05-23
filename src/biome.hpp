#pragma once

#include "raylib.h"

// Variable qui va permettre de choisir un biome, de base est fixé à 0
inline int selectBiome { 0 };

// Structure pour mieux stocker les valeurs de hauteur et de couleurs + palettes
struct ColorHeight
{
    float h; // Hauteur
    Color c; // Couleurs
};

std::vector<ColorHeight> const plaine {
    {
        { 0.0f, {0, 0, 120, 255} },     // Mer sombre
        { 0.2f, {70, 130, 180, 255} },  // Mer clair
        { 0.3f, {240, 220, 130, 255} }, // Plage
        { 0.4f, {80, 160, 50, 255} },   // Plaine
        { 0.6f, {60, 80, 40, 255} },    // Forêt
        { 0.7f, {120, 100, 80, 255} },  // Montagne
        { 0.9f, {255, 255, 255, 255} }, // Neige
    }
};

std::vector<ColorHeight> const arctique {
    {
        { 0.0f, {0, 20, 80, 255} },      // Mer sombre
        { 0.1f, {30, 60, 120, 255} },    // Mer clair
        { 0.15f, {80, 90, 91, 255} },    // Arctique
        { 0.30f, {240, 245, 255, 255} }, // Glacier
        { 0.9f, {250, 250, 250, 255} },  // Sommet
    }
};

std::vector<ColorHeight> const desert {
    {
        { 0.0f, {30, 80, 150, 255} },   // Mer
        { 0.2f, {70, 130, 180, 255} },  // Mer cotiere
        { 0.3f, {210, 190, 120, 255} }, // Plage
        { 0.5f, {200, 160, 70, 255} },  // Désert
        { 0.7f, {170, 120, 50, 255} },  // Dunes
        { 0.8f, {130, 90, 40, 255} },   // Roche
        { 0.9f, {100, 70, 30, 255} },   // Falaise
    }
};

std::vector<ColorHeight> const magma {
    {
        { 0.0f, {10, 5, 5, 255} },      // Roche noire
        { 0.2f, {40, 10, 5, 255} },     // Roche sombre
        { 0.4f, {120, 20, 0, 255} },    // Roche chaude
        { 0.6f, {200, 60, 0, 255} },    // Lave refroidie
        { 0.75f, {240, 120, 0, 255} },  // Lave vive
        { 0.85f, {255, 200, 0, 255} },  // Lave incandescente
        { 0.9f, {255, 255, 180, 255} }, // Cratère
    }
};

std::vector<ColorHeight> const theEnd {
    {
        { 0.0f, {0, 0, 0, 255} },          // Void
        { 0.05f, {231, 240, 175, 255} },   // End Stone 1
        { 0.1f, {220, 226, 162, 255} },    // End Stone 2
        { 0.2f, {200, 196, 142, 255} },    // End Stone 3
        { 0.3f, {159, 110, 159, 255} },    // Light Purple
        { 0.45f, {128, 91, 128, 255} },    // Purple
        { 0.6f, {49, 28, 74, 255} },       // Dark purple
        { 0.85f, {31, 19, 53, 255} },      // End Top
        { 0.95f, {0, 0, 0, 255} },         // End sky
    }
};

std::vector<ColorHeight> const mesa {
    {
        { 0.0f, {207, 124, 24, 255} },   // Bottom mesa1
        { 0.05f, {127, 74, 40, 255} },   // Bottom mesa2
        { 0.1f, {196, 152, 55, 255} },   // Bottom mesa3
        { 0.2f, {161, 101, 79, 255} },   // Middle mesa1
        { 0.3f, {90, 68, 79, 255} },     // Middle mesa2
        { 0.45f, {172, 84, 91, 255} },   // Middle mesa3
        { 0.6f, {180, 121, 101, 255} },  // Top mesa1
        { 0.7f, {204, 182, 169, 255} },  // End Top
    }
};

inline std::array<std::vector<ColorHeight>, 6> const biomes {
    {
        plaine,
        arctique,
        desert,
        magma,
        theEnd,
        mesa,
    }
};