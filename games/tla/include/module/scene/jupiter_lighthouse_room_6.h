#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_6_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_6_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "module/scene/scene.h"

void JupiterLighthouse_OpenDoor(s32 value);

void JupiterLighthouse_CheckBlockOnCell(void);

void Teleporter_PlaceArrival(s32 x, s32 z, s32 effect);

extern s16 gLightSpriteTile;
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritesLarge;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

void Leader_CenterInCell(void);

void Scene_RunIfFlagCleared(void);

extern u32 gFieldEffect_ShortLifeScript[];
extern u32 gFieldEffect_MediumLifeScript[];
extern u32 gFieldEffect_LongLifeScript[];

void Func_020001d0(void);

extern s16 gLightSpriteCount;
extern u16 gLightSpritePriority;
extern u16 gPaletteCycleColors[];
void Func_02001be4();
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
extern s32 Data_020023e4;

#endif
