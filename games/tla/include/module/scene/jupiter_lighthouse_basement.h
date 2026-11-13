#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_BASEMENT_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_BASEMENT_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "module/scene/scene.h"

struct EffectOptions;

void Func_02001d3c(s32 id);

void Func_02001d34();

void Func_02001094(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gData_020023f4[];
extern struct LightOam Data_02002434[];

void Leader_CenterInCell(void);

void Func_02000074(void);

void Func_020000f8(void);

void Func_02000118(void);

extern u32 gData_020021c8[];
extern u32 gData_02002204[];
extern u32 gData_02002240[];

void Func_02001d1c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
extern u16 gData_020023b0[];
extern u16 gData_020023d0[];
extern u16 gData_020023d4[];
extern s32 Data_020023ac;

#endif
