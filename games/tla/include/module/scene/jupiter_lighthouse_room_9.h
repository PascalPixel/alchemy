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
extern struct Actor *gData_02002660[];

s32 Func_02001c80(struct Actor *actor);
void Import_OamAddSpriteAtDepth(struct LightOam *oam, s32 depth);

void Leader_CenterInCell(void);

extern struct CellTarget gTeleporters;

void Teleporter_Uncover(struct CellTarget *table,
                        struct CellTargetEntry *entry);

void Func_0200007c(void);

void Whirlwind_CarryLeader(void);

extern u32 gData_0200211c[];
extern u32 gData_02002158[];

#endif
