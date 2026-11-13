#ifndef ALCHEMY_MODULE_SCENE_YAMPI_DESERT_H
#define ALCHEMY_MODULE_SCENE_YAMPI_DESERT_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

void YampiDesert_RippleFollowsScorpion(void);

extern u32 gYampiDesert_SandCloudCells[];

extern u32 gYampiDesert_SandCloudDrifts[];

void YampiDesert_ScorpionUsesScoop(void);
// Const: the cell helper only computes from its argument.
s32 YampiDesert_CellCenter(s32 cell) __attribute__((const));
extern u32 gYampiDesert_SlideDust[];
void YampiDesert_SetUpRoom1(void);
void YampiDesert_SetUpRoom2(void);
void YampiDesert_SetUpRoom3(void);
void YampiDesert_SetUpRoom4(void);
void YampiDesert_SetUpRoom5(void);

void YampiDesert_SetUpRoom6(void);

void YampiDesert_SetUpRoom8(void);
void YampiDesert_SetUpRoom9(void);
void YampiDesert_SetUpRoom10(void);
void YampiDesert_SetUpRoom11(void);
void YampiDesert_SetUpRoom12(void);

void YampiDesert_DropScoopGem(void);

void YampiDesert_TalkToElder(void);

void YampiDesert_ReadSignpost(void);

void YampiDesert_StepOffStakeTops(void);

void YampiDesert_CastSand(void);

void YampiDesert_EndSand(void);

void YampiDesert_WatchScorpionStop(void);

void YampiDesert_StirScorpion(void);

void YampiDesert_StirScorpionAgain(void);

void YampiDesert_CornerScorpion(void);

void YampiDesert_TrapKingScorpion(void);

void YampiDesert_RevealRockHint(void);

void YampiDesert_HideRockHint(void);

void YampiDesert_RevealMedalSpot(void);

void YampiDesert_ShowSlideActors(void);

void YampiDesert_ClimbRevealedLadder(void);
void YampiDesert_ClimbOutOfRoom11(void);

extern u32 gYampiDesert_TravelerLooksAround[];

extern u32 gYampiDesert_OtherTravelerLooksAround[];

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 Func_020041e8(s32 layer, s32 shape, s32 height);

void YampiDesert_StartRisingSand(s32 unused, s32 value);

void Icon_DrawItem(s32 item);
void FieldEffect_SetAttached(s32 actor, s32 a);
void FieldCamera_WaitForArrival();

#endif
