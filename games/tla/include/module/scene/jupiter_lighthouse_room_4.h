#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_4_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_4_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"

struct CellTarget;
struct CellTargetEntry;

void Actor_RaiseFromCell(s32 id);

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct LightOam gLightSpriteOam[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;

void Func_02001790(void);

extern struct CellTarget gTeleporters;

void Teleporter_Uncover(struct CellTarget *table,
                        struct CellTargetEntry *entry);

void JupiterLighthouse_HideThePassage(void);

void JupiterLighthouse_LeaveByFirstCyclone(void);

void JupiterLighthouse_LeaveBySecondCyclone(void);

void JupiterLighthouse_TryLockedDoor(void);

extern u32 gFieldEffect_LongLifeScript[];

void LightSprites_InitA(void);

extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
void Func_02001e28();
extern s32 gWhirlwindRiding;

#endif
