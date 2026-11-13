#ifndef ALCHEMY_MODULE_SCENE_MAGMA_ROCK_INSIDE_H
#define ALCHEMY_MODULE_SCENE_MAGMA_ROCK_INSIDE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct FieldSprite;
struct Particle;
struct ParticleSprite;
struct RockSprite__s4b18;

struct MagmaRockInsideSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

void MagmaRockInside_PausePlatform();

extern s16 gMagmaRockInside_Room2Pillars[];

extern s16 gMagmaRockInside_Room3Pillars[];

extern s16 gMagmaRockInside_Room4Pillars[];

extern s16 gMagmaRockInside_Room7Pillars[];

void MagmaRockInside_MarkBlockCells(u16 *ids);

void MagmaRockInside_WaitForLanding(struct Actor *object);

s32 MagmaRockInside_GetCell(u32 layer, s32 x, s32 y);

s32 MagmaRockInside_FindStatue(u16 *table);

s32 MagmaRockInside_FlatDistance(s32 *from, s32 *to);

void MagmaRockInside_TrailEmbers(struct Actor *object);
void MagmaRockInside_DrainLava(void);

void MagmaRockInside_DropRocks(s32 x, s32 y, s32 z, s32 count);

void MagmaRockInside_StartFallingRocks(void);

void MagmaRockInside_PressGateSwitch1(void);
void MagmaRockInside_PressGateSwitch2(void);
void MagmaRockInside_PressGateSwitch3(void);

extern s16 gMagmaRockInside_PlatformPath6[];
extern s16 gMagmaRockInside_PlatformPath7[];
extern s16 gMagmaRockInside_PlatformPath8[];
extern s16 gMagmaRockInside_PlatformPath9[];

void FieldEffect_StartBlankOverlay(s32 value);

void MagmaRockInside_RaiseActorCells(u16 *list);

void HangingVine_Place(s32 id, s32 value);
s32 Func_02004ed0(struct Actor *object);
s32 Field_TryJumpForward(void);
struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

void MagmaRockInside_SpawnRubble(s32 spread);
void MagmaRockInside_SetLeaderCellShape(s32 a, s32 b);
void MagmaRockInside_SpawnDustRing(void);
extern u32 gMagmaRockInside_RubbleScript[];

u32 Func_02004e98(struct FieldSprite *sprite, u32 frame);

extern u32 gMagmaRockInside_DustScript[];

void Func_02004f18(struct ParticleSprite *sprite, struct Particle *particle);
void Func_02004c58(void *entry, void *destination);
void Func_02004f10(struct RockSprite__s4b18 *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);

void MagmaRockInside_UpdateFallingRocks(void);

void MagmaRockInside_FightFury(void);

void MagmaRockInside_UnblockMovedPillar(void);
void MagmaRockInside_BlockMovedPillar(void);
void MagmaRockInside_PushPillarIntoLava(void);

void MagmaRockInside_FloodWithLava(void);

void MagmaRockInside_ReleaseRocks(void);
void MagmaRockInside_TakeFlame(void);

void MagmaRockInside_GrowFlame(void);

void MagmaRockInside_PutOutFlames(void);

void MagmaRockInside_ExamineEmberStone(void);

void MagmaRockInside_StampStepCell(void);

void MagmaRockInside_StampOtherStepCell(void);

void MagmaRockInside_StepOnMarker(void);

void MagmaRockInside_LightTabletTorch(void);
void MagmaRockInside_GrowTabletFlame(void);
void MagmaRockInside_PutOutTabletFlame(void);
void MagmaRockInside_ReadBlazeTablet(void);
void MagmaRockInside_ReadBlazeTabletFromWest(void);
void MagmaRockInside_ReadBlazeTabletFromEast(void);
void MagmaRockInside_ReadBlazeTabletFromSouth(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

void MagmaRockInside_BurstStatue(s32 mode, s32 cue);

#endif
