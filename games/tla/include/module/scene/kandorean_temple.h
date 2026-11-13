#ifndef ALCHEMY_MODULE_SCENE_KANDOREAN_TEMPLE_H
#define ALCHEMY_MODULE_SCENE_KANDOREAN_TEMPLE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

struct MapWork17 {
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 *script;
  u16 flags;
  u8 wideTiles;
  u8 bits0 : 2;
  u8 mode : 2;
};

extern u32 gKandoreanTemple_Room5Regions[];
extern struct ActorPlacement gKandoreanTemple_GatePlacements[];
extern struct ActorPlacement gKandoreanTemple_PoisHallPlacements[];
extern s16 gKandoreanTemple_Room2Events[];
void KandoreanTemple_ArriveOnEastLift(void);
void KandoreanTemple_LowerTheLadder(void);

void KandoreanTemple_StartLeftVent(void);

void KandoreanTemple_TestTheStudents(void);
void KandoreanTemple_PraiseTheLevitator(void);
void KandoreanTemple_FindTheRopeOnTheFloor(void);
void KandoreanTemple_MeetPoi(void);
void KandoreanTemple_CreateLashPebble(void);
extern u32 gKandoreanTemple_PoiWalksOutScript[];
extern u32 gKandoreanTemple_StudentWalksAsideScript[];
extern u32 gKandoreanTemple_StudentWalksAwayScript[];

void KandoreanTemple_CastLash(void);
void KandoreanTemple_KradenSuspectsTheWall(void);

void KandoreanTemple_PushThroughTheSteam(void);

void KandoreanTemple_ReadPoisSign(void);

void KandoreanTemple_TryTheLockedDoor(void);

void KandoreanTemple_AskWhoPassedTheTest(void);

void KandoreanTemple_ReadTheGateStudent(void);

void KandoreanTemple_ReadTheGuard(void);
void KandoreanTemple_ClimbDownTheWell(void);

void KandoreanTemple_DropThroughTheTrapdoor(void);

void KandoreanTemple_SlipIntoTheCrack(void);
void KandoreanTemple_StartMovingBlock(void);
void KandoreanTemple_FinishMovingBlock(void);
void KandoreanTemple_FinishMovingPillar(void);
void KandoreanTemple_StartMovingPot(void);
void KandoreanTemple_FinishMovingPot(void);

void KandoreanTemple_TouchTheHotFloor(void);

void KandoreanTemple_OpenTheWellPassage(void);
void KandoreanTemple_FindTheHiddenPassage(void);
void KandoreanTemple_StepBehindThePole(void);
void KandoreanTemple_StepOutFromThePole(void);
void KandoreanTemple_FightChestbeaters(void);
void KandoreanTemple_CycloneFirstPatch(void);
void KandoreanTemple_CycloneSecondPatch(void);
void KandoreanTemple_CycloneThirdPatch(void);
void KandoreanTemple_CycloneFourthPatch(void);

void KandoreanTemple_CycloneFifthPatch(void);

extern u32 gFieldEffect_SmallSpawnScript[];

extern u32 gFieldEffect_MediumSpawnScript[];

extern s32 gKandoreanTemple_LevitateHighScript[];
extern s32 gKandoreanTemple_LevitateLowScript[];
extern s32 gKandoreanTemple_StandStillScript[];

void KandoreanTemple_ArriveOnWestLift(void);

extern s32 gKandoreanTemple_LeftTimer;
extern u16 gKandoreanTemple_LeftBobPhase;
extern u16 gKandoreanTemple_RightBobPhase;
void Func_02005398(void);
void Func_020053b8(const void *data);

#endif
