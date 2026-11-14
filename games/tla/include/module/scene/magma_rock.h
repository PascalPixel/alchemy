#ifndef ALCHEMY_MODULE_SCENE_MAGMA_ROCK_H
#define ALCHEMY_MODULE_SCENE_MAGMA_ROCK_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct ResourceEntry;

struct MagmaRockEffectOptions {
  s32 kind;
  s32 palette;
  s32 scaleX;
  s32 scaleY;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  s32 unk20;
  void (*update)();
};

struct MagmaRockCrumblingPillar {
  u16 id;
  u16 flag;
};

struct Entry {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 unk18;
};

struct EntryWork {
  u8 unk0[0x500];
  struct Entry entries[64];
  u8 unkc00[0xa00];
  s16 unk1600;
};

struct MagmaRockSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);

extern s16 gMagmaRock_Room1Pillars[];

extern s16 gMagmaRock_Room2LowerPillars[];

extern s16 gMagmaRock_Room2MiddlePillars[];
extern s16 gMagmaRock_Room2UpperPillars[];
extern s16 gMagmaRock_Room2TopPillars[];

void MagmaRock_RollMagmaRock(struct Actor *object);

extern u32 gMagmaRock_EmberScript[];

void MagmaRock_BurnStatue(struct Actor *object);

extern s16 gMagmaRock_Room1Statues[];
extern s16 gMagmaRock_Room2LowerStatues[];
extern s16 gMagmaRock_Room2MiddleStatues[];
extern s16 gMagmaRock_Room2UpperStatues[];
extern s16 gMagmaRock_Room2TopStatues[];
extern s16 gMagmaRock_Room3Statues[];

void MagmaRock_FlashStatue(struct Actor *object);

void Actor_SelectAnimation(struct Actor *object, s32 a);

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
