#ifndef ALCHEMY_SCENE_AIRS_ROCK_H
#define ALCHEMY_SCENE_AIRS_ROCK_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/scene_tables.h"

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 y);

extern s16 gAirsRock_StormBrandChestRegions[];
extern s16 gAirsRock_EntranceRegions[];
extern struct ActorPlacement gAirsRock_StormBrandChestPlacements[];
extern struct ActorPlacement gAirsRock_EntrancePlacements[];
extern struct ActorPlacement gAirsRock_FujinShieldChestPlacements[];
extern struct Trigger gAirsRock_StormBrandChestTriggers[];
extern struct Trigger gAirsRock_EntranceTriggers[];
extern struct Trigger gAirsRock_FujinShieldChestTriggers[];

void AirsRock_DriftFog(void);
void AirsRock_LiftFog(void);

void AirsRock_ScrollWave(void);

extern s16 gAirsRock_EntrancePillars[];
extern s16 gAirsRock_FrozenPuddle[];

extern s8 gAirsRock_FogAlpha;
extern s8 gAirsRock_FogAlphaStep;
extern s8 gAirsRock_LiftingFogAlpha;
extern s8 gAirsRock_LiftingFogAlphaStep;

void AirsRock_ExamineWindStone(void);

void AirsRock_EnterTheRock(void);
void AirsRock_MoveFirstPillar(void);

void AirsRock_MoveThirdPillar(void);

void AirsRock_ClimbLedge(void);
void AirsRock_StartWhirlwind(void);
void AirsRock_TakeOverWhirlwind(void);

void AirsRock_BlowGust(void);

void AirsRock_WhirlwindClearsWall(void);
void AirsRock_RideWhirlwind(void);
void AirsRock_LiftPillar(void);
void AirsRock_PlacePillar(void);
void AirsRock_FreezePuddle(void);
void AirsRock_WhirlwindOpensTheTop(void);
void AirsRock_WhirlwindClearsFog(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];
extern s32 gAirsRock_WindStoneScript[];

void AirsRock_SetLedgeCellsByLevel(void);

struct Actor;

#endif
