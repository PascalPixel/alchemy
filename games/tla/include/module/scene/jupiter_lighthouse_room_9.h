#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_9_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_9_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;
struct LightOam;

void Teleporter_PlaceArrival(s32 x, s32 z, s32 effect);

void JupiterLighthouse_LoadPalettes(s32 mode);

extern s16 gLightSpriteTile;
extern struct Actor *gLightSpriteActors[];

s32 BattleEffect_GetObjectPriority(struct Actor *actor);
void Import_OamAddSpriteAtDepthClamped(struct LightOam *oam, s32 depth);

void Leader_CenterInCell(void);

extern struct CellTarget gTeleporters;

void Teleporter_Uncover(struct CellTarget *table,
                        struct CellTargetEntry *entry);

void JupiterLighthouse_StopHovering(void);

void Whirlwind_CarryLeader(void);

extern u32 gFieldEffect_ShortLifeScript[];
extern u32 gFieldEffect_MediumLifeScript[];

extern s32 gWhirlwindRiding;
extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
void Func_02001bd8();
void Func_02001c98();

#endif
