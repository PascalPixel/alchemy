#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_8_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_8_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;
struct FieldEffect;

struct IsletsVector {
  s32 x;
  s32 y;
  s32 z;
};

s32 Teleporters_Step(void);

void Func_020022c0(s32 x, s32 z, s32 a, s32 b);

void Func_02002248(s32 id);

void Teleporter_PlaceArrival(s32 x, s32 z, s32 effect);

void Func_02000068(void);

extern u32 gData_020027a8[];
extern u32 gData_020027e4[];
extern u32 gData_02002820[];

struct Actor *PushTarget_FindAhead(s32 *direction, s32 *slot, s32 *kind);

extern u16 gPushKinds[];

extern s16 gLightSpriteCount;
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;

void Func_02002160(struct FieldEffect *effect, s32 mode);

void Leader_CenterInCell(void);

extern struct CellTarget gTeleporters;

void Teleporter_Uncover(struct CellTarget *table,
                        struct CellTargetEntry *entry);

void Func_020021e0(s32 x, s32 z, s32 a, s32 b);
extern u16 gData_02002ab0[];
extern u16 gData_02002ad4[];
extern s32 Data_02002aac;

#endif
