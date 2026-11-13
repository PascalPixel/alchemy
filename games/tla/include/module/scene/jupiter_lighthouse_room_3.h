#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_3_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_3_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"

struct Actor;

void Data_02000130(void);

void Func_020001c0(void);

void Func_020003c4(void);

extern u32 gData_02002950[];
extern u32 gData_0200298c[];
extern u32 gData_020029c8[];

extern s32 gData_02002ce8;

struct Actor *Func_02000f34(s32 *direction, s32 *slot, s32 *kind);

s32 Func_02002338(struct Actor *object, s32 *position);

extern u16 gData_02002478[];

extern s16 gData_02002d2e;
extern struct Actor *gData_02002d30[];
extern struct LightOam Data_02002d70[];

extern s16 gData_02002e30;

extern s16 gData_02002e32;

s32 Func_02002448(struct Actor *actor);

/// The cells Func_020020e8 watches, and the cell it copies over the one the
/// leader steps on.
extern struct CellTarget gData_02002e34;

void Func_02001950(void);

#endif
