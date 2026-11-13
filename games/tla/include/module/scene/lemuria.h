#ifndef ALCHEMY_MODULE_SCENE_LEMURIA_H
#define ALCHEMY_MODULE_SCENE_LEMURIA_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

void FieldCamera_WaitForArrival();

void Func_02001d34();

void Func_02001d3c();

void Lemuria_TakeLeaderLayer(void);
extern u32 gLemuria_FirstWalkerPaces[];
extern u32 gLemuria_SecondWalkerPaces[];
extern s16 gLemuria_DoorOpening[];
extern s16 gLemuria_WideDoorOpening[];

#endif
