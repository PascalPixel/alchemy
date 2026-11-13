#ifndef ALCHEMY_MODULE_SCENE_LEMURIA_INTERIORS_H
#define ALCHEMY_MODULE_SCENE_LEMURIA_INTERIORS_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

void LemuriaInteriors_PiersVisitsUncle(void);

extern s16 gSceneRegions[];
extern u32 gData_02002dec[];

void FieldCamera_WaitForArrival();

void LemuriaInteriors_AskFromBeyondSeaOfTime(s32 arg0);

void LemuriaInteriors_ScoffAtMindRead(s32 arg0, s32 arg1);

void LemuriaInteriors_LeavePiersToMourn(s32 a0, s32 a1, s32 a2);

#endif
