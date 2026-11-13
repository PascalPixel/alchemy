#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_3_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_3_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"

void Data_02000130(void);

void Func_020001c0(void);

void Func_020003c4(void);

extern u32 gFieldEffect_ShortLifeScript[];
extern u32 gFieldEffect_MediumLifeScript[];
extern u32 gFieldEffect_LongLifeScript[];

extern s32 gWhirlwindRiding;

struct Actor *PushTarget_FindAhead(s32 *direction, s32 *slot, s32 *kind);

s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);

extern u16 gPushKinds[];

extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern struct LightOam gLightSpriteOams[];

extern s16 gLightSpritesLarge;

extern s16 gLightSpritePriority;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

/// The cells Teleporters_Step watches, and the cell it copies over the one the
/// leader steps on.
extern struct CellTarget gTeleporters;

void Teleporters_Update(void);

void PaletteCycle_StepAt(void);

#endif
