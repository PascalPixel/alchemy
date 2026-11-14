#ifndef ALCHEMY_MODULE_SCENE_AIRS_ROCK_H
#define ALCHEMY_MODULE_SCENE_AIRS_ROCK_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

struct MarkedBlock {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x63 - 0x14];
  u8 unk63;
};

struct AirsRockMapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct AirsRockMapWork {
  u8 unk000[0x108];
  struct AirsRockMapLayer layers[3];
};

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 y);

extern s16 gAirsRock_Room1Regions[];
extern s16 gAirsRock_Room2Regions[];
extern struct ActorPlacement gAirsRock_Room1Placements[];
extern struct ActorPlacement gAirsRock_Room2Placements[];
extern struct ActorPlacement gAirsRock_Placements[];
extern struct Trigger gAirsRock_Room1Triggers[];
extern struct Trigger gAirsRock_Room2Triggers[];
extern struct Trigger gAirsRock_Triggers[];

void AirsRock_DriftFog(void);
void AirsRock_LiftFog(void);

void AirsRock_ScrollWave(void);

extern s16 gAirsRock_Room2Pillars[];
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

extern char SceneId_AirsRock1;
extern char SceneId_AirsRock2;
extern char SceneId_AirsRock3;

#endif
