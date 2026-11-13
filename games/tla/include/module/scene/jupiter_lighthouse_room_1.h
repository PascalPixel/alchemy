#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_1_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_1_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct LightOam;

void Func_02001bcc(s32 owner);

extern s32 gWhirlwindRiding;

s32 Func_02001bbc(s32 x, s32 z, s32 layer);

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;

void Import_OamAddSpriteAtDepthClamped(struct LightOam *oam, s32 depth);

void Leader_CenterInCell(void);

void Scene_RunOnce(void);

void Scene_RunIfFlagCleared(void);

extern u32 gFieldEffect_ShortLifeScript[];
extern u32 gFieldEffect_MediumLifeScript[];
extern u32 gFieldEffect_LongLifeScript[];

extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];

#endif
