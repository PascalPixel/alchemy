#ifndef ALCHEMY_MODULE_SCENE_YAMPI_DESERT_CAVE_H
#define ALCHEMY_MODULE_SCENE_YAMPI_DESERT_CAVE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct Actor;

extern u16 gYampiDesertCave_LogSprites[];

void YampiDesertCave_StopLeaderAtLog(struct Actor *object);

void Import_ActorSelectAnimation(struct Actor *object, s32 mode);

void YampiDesertCave_ArriveByTeleport(void);

extern u16 gYampiDesertCave_MarkCycle[];

void Func_02001928(s32 value);

void YampiDesertCave_FightCrystal(void);

void YampiDesertCave_WonderAtRipple(void);

void YampiDesertCave_TryPushLog(void);

void YampiDesertCave_TeleportFromFirstMark(void);

void YampiDesertCave_TeleportFromSecondMark(void);

void YampiDesertCave_CastSand(void);
void YampiDesertCave_EndSand(void);

extern u32 gYampiDesertCave_Settle[];

void Func_02001950(void);

#endif
