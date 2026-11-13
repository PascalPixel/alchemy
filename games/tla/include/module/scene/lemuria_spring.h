#ifndef ALCHEMY_MODULE_SCENE_LEMURIA_SPRING_H
#define ALCHEMY_MODULE_SCENE_LEMURIA_SPRING_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

s32 LemuriaSpring_RunSpringGame(void);

extern u16 gLemuriaSpring_CoinPrizes[];
extern u32 gLemuriaSpring_MedalPrizes[];

s32 LemuriaSpring_RunToss();

void LemuriaSpring_GivePrize(s32 a0);

s32 Func_02001400();

s32 ItemDisplay_ShowItem();

extern u8 gLemuriaSpring_FrontObstacleAnimations[];
extern u8 gLemuriaSpring_BackObstacleAnimations[];
extern u8 gLemuriaSpring_ObstacleStartX[];
extern u8 gLemuriaSpring_ObstacleStartZ[];

extern u16 gLemuriaSpring_ObstacleHeadings[];

void LemuriaSpring_SetSpriteStorageKind(s32 actor, s32 active);

s32 Party_RestoreHpPp();
s32 Class_GetSeries();

#endif
