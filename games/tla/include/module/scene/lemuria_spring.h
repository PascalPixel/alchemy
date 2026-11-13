#ifndef ALCHEMY_MODULE_SCENE_LEMURIA_SPRING_H
#define ALCHEMY_MODULE_SCENE_LEMURIA_SPRING_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

s32 LemuriaSpring_RunSpringGame(void);

extern u16 gData_0200173c[];
extern u32 gData_02001700[];

s32 LemuriaSpring_RunToss();

void Func_0200049c(s32 a0);

s32 Func_02001400();

s32 ItemDisplay_ShowItem();

extern u8 gData_02001746[];
extern u8 gData_02001749[];
extern u8 gData_0200174c[];
extern u8 gData_02001750[];

extern u16 gData_02001754[];

void LemuriaSpring_SetSpriteStorageKind(s32 actor, s32 active);

s32 Party_RestoreHpPp();
s32 Class_GetSeries();

#endif
