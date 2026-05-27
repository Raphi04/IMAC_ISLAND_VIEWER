#include "sound.hpp"

Sound ado;
Sound teto;
Sound capy;
Sound enderman;

void loadSounds() {
    ado = LoadSound("../../resources/gyaru.mp3");
    teto = LoadSound("../../resources/teto.mp3");
    capy = LoadSound("../../resources/capy.mp3");
    enderman = LoadSound("../../resources/enderman.mp3");
}