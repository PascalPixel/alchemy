#ifndef ALCHEMY_MODULE_SCENE_TREASURE_ISLE_H
#define ALCHEMY_MODULE_SCENE_TREASURE_ISLE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "module/scene/scene.h"
#include "scene_tables.h"

struct TreasureIsleCell;

extern struct ActorPlacement gTreasureIsle_Placements[];

extern struct ActorPlacement gTreasureIsle_Room2Placements[];

extern struct ActorPlacement gTreasureIsle_Room3Placements[];
extern struct ActorPlacement gTreasureIsle_Room4Placements[];
extern struct ActorPlacement gTreasureIsle_Room5Placements[];
extern u32 gTreasureIsle_RockDust[];

// Import stubs: Event_Start and Event_End, and the pending
// actions' resolution.
void Event_Start(void);

extern struct TreasureIsleCell gTreasureIsle_Room2SpikyRocks[];
extern struct TreasureIsleCell gTreasureIsle_Room3SpikyRocks[];
extern struct Trigger gTreasureIsle_Room2Triggers[];
extern struct Trigger gTreasureIsle_Room3Triggers[];
extern s32 gTreasureIsle_Room4Triggers[];
extern struct Trigger gTreasureIsle_Room5Triggers[];

void TreasureIsle_SetSpikyRock(struct TreasureIsleCell *cells, s16 index);

void TreasureIsle_CycleWaterColors(void);

void Func_0200105c(void);

s32 Func_02001064(s32 a, s32 b, s32 c);

s32 Field_TryJumpForward();

void TreasureIsle_FightGale(void);

void TreasureIsle_VenusDjinniJoins(void);

void TreasureIsle_LiftFirstRock(void);

void TreasureIsle_LiftFourthRock(void);

void TreasureIsle_LiftFifthRock(void);

void TreasureIsle_LiftSixthRock(void);
void TreasureIsle_LiftSeventhRock(void);

void TreasureIsle_LiftEighthRock(void);

void TreasureIsle_MeetWaterGuardian(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];
extern u32 gTreasureIsle_PostStill[];
extern u32 gTreasureIsle_LiftRockScale[];

void TreasureIsle_PressRoom2Button(void);

void FieldCamera_WaitForArrival(void);

#endif
