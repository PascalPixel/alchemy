#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_BASEMENT_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_BASEMENT_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "module/scene/scene.h"

struct EffectOptions;

void Func_02001d3c(s32 id);

void Func_02001d34();

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern struct LightOam Data_02002434[];

void Leader_CenterInCell(void);

void Func_02000074(void);

void Func_020000f8(void);

void Func_02000118(void);

extern u32 gFieldEffect_ShortLifeScript[];
extern u32 gFieldEffect_MediumLifeScript[];
extern u32 gFieldEffect_LongLifeScript[];

void Func_02001d1c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
extern s32 Data_020023ac;

#endif
