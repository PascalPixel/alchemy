#ifndef ALCHEMY_SCENE_MAGMA_ROCK_H
#define ALCHEMY_SCENE_MAGMA_ROCK_H

/// @file
/// What Magma Rock's outside (magma_rock.c) and inside (magma_rock_inside.c)
/// share: where a statue's fire stops, the flag set while its Burst plays, and
/// the outside's tables and events.

#include "types.h"

struct Actor;

/// The collision codes a statue's fire stops at as it flies
/// (MagmaRock_FireFromStatue, MagmaRockInside_FireFromStatue): the edge of its
/// way, and a lava fall, which it floods the room from
/// (MagmaRockInside_RaiseLava).
enum MagmaRockFireStopCell {
  CELL_BEHAVIOR_MAGMA_ROCK_FIRE_EDGE = 212,
  CELL_BEHAVIOR_MAGMA_ROCK_LAVA_FALL = 222
};

/// Set while a statue's fire bursts where it lands, so that Burst there does
/// not target the statue again (MagmaRock_FireFromStatue,
/// MagmaRockInside_FireFromStatue).
#define FLAG_FIRE_BURST_PLAYING FLAG_SCENE_TEMP_F0

/// A crumbling pillar of Magma Rock (MagmaRock_CrumblePillar): its actor and
/// its flag.
struct MagmaRockCrumblingPillar {
  u16 id;
  u16 flag;
};

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);

extern s16 gMagmaRock_SlidePillars[];

extern s16 gMagmaRock_SalamanderTailHighWestPillars[];

extern s16 gMagmaRock_SalamanderTailHighEastPillars[];
extern s16 gMagmaRock_SalamanderTailLowWestPillars[];
extern s16 gMagmaRock_SalamanderTailLowEastPillars[];

void MagmaRock_MoveFireball(struct Actor *actor);

extern u32 gMagmaRock_EmberScript[];

void MagmaRock_BurnStatue(struct Actor *actor);

extern s16 gMagmaRock_SlideStatues[];
extern s16 gMagmaRock_SalamanderTailHighWestStatues[];
extern s16 gMagmaRock_SalamanderTailHighEastStatues[];
extern s16 gMagmaRock_SalamanderTailLowWestStatues[];
extern s16 gMagmaRock_SalamanderTailLowEastStatues[];
extern s16 gMagmaRock_StatueStatues[];

void MagmaRock_FlashStatue(struct Actor *actor);

void MagmaRock_DropRocks(s32 x, s32 y, s32 z, s32 count);
void MagmaRock_StartFallingRocks(void);
void MagmaRock_EndFallingRocks(void);

void MagmaRock_BurstStatue(s32 mode, s32 cue);

void Actor_SetCellShape(s32 level, s32 layer, s32 id);

struct Actor *Actor_FindInCell(s32 *position, struct Actor *self);

extern s16 gMagmaRock_CrumblingPillars[];

void MagmaRock_RestoreCrumblingPillars(const u16 *table);

void MagmaRock_PlaceTightrope();
s32 Field_TryJumpForward(void);

void Dust_Scatter(s32 spread);
void MagmaRock_SetLeaderCellShape(s32 heightOffset, s32 layer);
extern u32 gDust_ScatterScript[];

extern u32 gMagmaRock_DustScript[];

void MagmaRock_FireStatue(void);

void MagmaRock_IgniteStatue(void);
void MagmaRock_ResetCaveDoor(void);

void MagmaRock_EnterCave(void);

void MagmaRock_ResolveBeforeBurst(void);
void MagmaRock_CheckCrumblingPillars(void);

void MagmaRock_ReadyHighWestStatue(void);

void MagmaRock_ResetHighWestStatue(void);
void MagmaRock_KnockLeaderOffWall(void);
void MagmaRock_PushPillarEast(void);
void MagmaRock_PushPillarWest(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b);

#endif
