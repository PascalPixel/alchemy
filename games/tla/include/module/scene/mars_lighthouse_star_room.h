#ifndef ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_STAR_ROOM_H
#define ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_STAR_ROOM_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;

void Func_02002148(void);
void Func_02002ad0(struct Actor *object);

void Func_020014f4(void);

void Func_02001bf8(void);

s32 Func_0200422c(s32 terrain, s32 x, s32 z);

void Func_02001fcc(void);

void Func_0200274c(struct Actor *actor);

void Func_02002d90(void);
void Func_02002d04(struct Actor *object);

extern struct CellTarget gData_02004cf4;

void Func_02003bfc(struct CellTarget *table, struct CellTargetEntry *entry);

void Func_02000060(void);

void Func_02000074(void);

void Func_02000188(void);

void Func_020001e8(void);

void Func_020005b0(void);

void MarsLighthouseStarRoom_HoldWarmHand(void);

void MarsLighthouseStarRoom_TakeMarsStar(void);

void MarsLighthouseStarRoom_UseMarsStar(void);

void Func_0200007c(void);

extern u32 gData_0200485c[];
extern u32 gData_02004898[];
extern u32 gData_020048d4[];

#endif
