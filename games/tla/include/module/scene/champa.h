#ifndef ALCHEMY_MODULE_SCENE_CHAMPA_H
#define ALCHEMY_MODULE_SCENE_CHAMPA_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

struct Actor;

extern s16 gData_02002efc[];
extern s32 gData_020031fc[];

s32 Func_02002a5c();

extern struct Trigger gData_0200320c[];

extern struct Trigger gData_0200b410[];

void Actor_SelectAnimation(struct Actor *object, s32 mode);

extern s16 gData_020031e4[];
extern s16 gData_020031e6[];

s32 Func_02002bac();
s32 Window_CloseMatchingId();

#endif
