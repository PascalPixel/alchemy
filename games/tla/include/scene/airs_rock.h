#ifndef ALCHEMY_SCENE_AIRS_ROCK_H
#define ALCHEMY_SCENE_AIRS_ROCK_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/scene_tables.h"

/// Set while the leader rides a statue's whirlwind (AirsRock_RideWhirlwind,
/// AirsRockInside_RideWhirlwind): the statues breathe theirs without a flare.
#define FLAG_RIDING_WHIRLWIND FLAG_SCENE_TEMP_50

/// How near across the ground a statue's whirlwind comes to pick up the
/// leader, in pixels; on Air's Rock's outside, also how near it comes to
/// another actor to end on it.
#define WHIRLWIND_REACH 8

/// A statue breathes a whirlwind every this many frames.
#define WHIRLWIND_BREATH_FRAMES 240

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 y);

extern struct SceneRegion gAirsRock_StormBrandChestRegions[];
extern struct SceneRegion gAirsRock_EntranceRegions[];
extern struct ActorPlacement gAirsRock_StormBrandChestPlacements[];
extern struct ActorPlacement gAirsRock_EntrancePlacements[];
extern struct ActorPlacement gAirsRock_FujinShieldChestPlacements[];
extern struct Trigger gAirsRock_StormBrandChestTriggers[];
extern struct Trigger gAirsRock_EntranceTriggers[];
extern struct Trigger gAirsRock_FujinShieldChestTriggers[];

void AirsRock_DriftFog(void);
void AirsRock_LiftFog(void);

/// The great whirlwind's sway: BG0's line scroll, by HBlank DMA.
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

void AirsRock_DropOffLedge(void);
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
