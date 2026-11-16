#ifndef ALCHEMY_SCENE_MAGMA_ROCK_H
#define ALCHEMY_SCENE_MAGMA_ROCK_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;

/// The collision codes a statue's fire stops at as it flies
/// (MagmaRock_FireFromStatue, MagmaRockInside_FireFromStatue): the edge of its
/// way, and a lava fall, which it floods the room from
/// (MagmaRockInside_RaiseLava).
enum MagmaRockFireStopCell {
  MAGMA_ROCK_CELL_FIRE_EDGE = 212,
  MAGMA_ROCK_CELL_LAVA_FALL = 222
};

/// A crumbling pillar of Magma Rock (MagmaRock_CrumblePillar): its actor and
/// its flag.
struct MagmaRockCrumblingPillar {
  u16 id;
  u16 flag;
};

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);

extern s16 gMagmaRock_SlidePillars[];

extern s16 gMagmaRock_SalamanderTailLowerPillars[];

extern s16 gMagmaRock_SalamanderTailMiddlePillars[];
extern s16 gMagmaRock_SalamanderTailUpperPillars[];
extern s16 gMagmaRock_SalamanderTailTopPillars[];

void MagmaRock_RollMagmaRock(struct Actor *object);

extern u32 gMagmaRock_EmberScript[];

void MagmaRock_BurnStatue(struct Actor *object);

extern s16 gMagmaRock_SlideStatues[];
extern s16 gMagmaRock_SalamanderTailLowerStatues[];
extern s16 gMagmaRock_SalamanderTailMiddleStatues[];
extern s16 gMagmaRock_SalamanderTailUpperStatues[];
extern s16 gMagmaRock_SalamanderTailTopStatues[];
extern s16 gMagmaRock_StatueStatues[];

void MagmaRock_FlashStatue(struct Actor *object);

void MagmaRock_DropRocks(s32 x, s32 y, s32 z, s32 count);
void MagmaRock_StartFallingRocks(void);
void MagmaRock_EndFallingRocks(void);

void MagmaRock_BurstStatue(s32 mode, s32 cue);

void MagmaRock_SetActorCellShape(s32 heightOffset, s32 layer, s32 id);

struct Actor *Actor_FindInCell(s32 *position, struct Actor *self);

extern s16 gMagmaRock_CrumblingPillars[];

void MagmaRock_RestoreCrumblingPillars(const u16 *table);

void MagmaRock_PlaceTightrope();
s32 Field_TryJumpForward(void);

void Rubble_Scatter(s32 spread);
void MagmaRock_SetLeaderCellShape(s32 heightOffset, s32 layer);
extern u32 gRubble_ScatterScript[];

extern u32 gMagmaRock_DustScript[];

void MagmaRock_FireStatue(void);

void MagmaRock_IgniteStatue(void);
void MagmaRock_ClearSlideFlag(void);

void MagmaRock_SlideDown(void);

void MagmaRock_ResolveBeforeBurst(void);
void MagmaRock_CheckCrumblingPillars(void);

void MagmaRock_SetLedgeBehavior(void);

void MagmaRock_ClearLedgeBehavior(void);
void MagmaRock_AmplifyBurst(void);
void MagmaRock_PushPillarEast(void);
void MagmaRock_PushPillarWest(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b);

#endif
