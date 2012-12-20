// Graphics converted using PAGfx by Mollusk.

#pragma once

#include <PA_BgStruct.h>

#ifdef __cplusplus
extern "C"{
#endif

// Sprites:
extern const unsigned char PenguinSprite_Sprite[4096] _GFX_ALIGN; // Palette: PenguinSprite_Pal
extern const unsigned char obstacle_Sprite[4096] _GFX_ALIGN; // Palette: obstacle_Pal

// Backgrounds:
extern const PA_BgStruct curveScreen1;
extern const PA_BgStruct curveScreen2;
extern const PA_BgStruct curveScreen3;
extern const PA_BgStruct curveScreen4;
extern const PA_BgStruct map;
extern const PA_BgStruct snowScreen1;
extern const PA_BgStruct snowScreen2;
extern const PA_BgStruct start;
extern const PA_BgStruct gameOver;

// Palettes:
extern const unsigned short PenguinSprite_Pal[256] _GFX_ALIGN;
extern const unsigned short obstacle_Pal[256] _GFX_ALIGN;

#ifdef __cplusplus
}
#endif
