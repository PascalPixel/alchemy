#ifndef ALCHEMY_SCENE_MAGMA_ROCK_H
#define ALCHEMY_SCENE_MAGMA_ROCK_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct ResourceEntry;

struct MagmaRockCrumblingPillar {
  u16 id;
  u16 flag;
};

struct MagmaRockSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
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
s32 Func_02003c04(struct Actor *object);
s32 Field_TryJumpForward(void);

void MagmaRock_SpawnRubble(s32 spread);
void MagmaRock_SetLeaderCellShape(s32 a, s32 b);
extern u32 gMagmaRock_RubbleScript[];

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
void MagmaRock_GrowVine1(void);
void MagmaRock_GrowVine2(void);
void MagmaRock_GrowVine3(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b);

#endif
