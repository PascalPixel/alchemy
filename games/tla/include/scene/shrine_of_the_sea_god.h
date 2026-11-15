#ifndef ALCHEMY_SCENE_SHRINE_OF_THE_SEA_GOD_H
#define ALCHEMY_SCENE_SHRINE_OF_THE_SEA_GOD_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;

/// A word of the object written through a one-member union (see the
/// FAKEMATCH(signature) tag in ShrineOfTheSeaGod_LashRope).
union ObjectWord {
  s32 value;
};

struct ShrineOfTheSeaGodSpot {
  s32 x;
  s32 z;
  s32 scaleX;
  s32 rotation;
};

void ShrineOfTheSeaGod_DjinniRunsWest(void);
void ShrineOfTheSeaGod_SetUpFlatProp(s32 actor);

void ShrineOfTheSeaGod_StartleRiki(void);

void ShrineOfTheSeaGod_OpenFieldMenu(void);

void ShrineOfTheSeaGod_UseSeaGodsTear(void);

void ShrineOfTheSeaGod_SinkWithWater(void);

void ShrineOfTheSeaGod_FreezeFirstPuddle(void);

void ShrineOfTheSeaGod_FreezeSecondPuddle(void);

void ShrineOfTheSeaGod_FreezeThirdPuddle(void);

void ShrineOfTheSeaGod_LiftBlock(void);

void ShrineOfTheSeaGod_SetDownTorch(void);

void ShrineOfTheSeaGod_MovePillar(void);

void ShrineOfTheSeaGod_DrawLeaderInFront(void);

void ShrineOfTheSeaGod_DrawLeaderBehind(void);

void ShrineOfTheSeaGod_TopplePillarIntoWater(void);

void ShrineOfTheSeaGod_ToppleFirstPillar(void);

void ShrineOfTheSeaGod_HideStone(void);
void ShrineOfTheSeaGod_ThrowRopeAgain(void);
void ShrineOfTheSeaGod_LashRope(void);
void ShrineOfTheSeaGod_DjinniFleesEast(void);

void ShrineOfTheSeaGod_DjinniFleesSouth(void);

void ShrineOfTheSeaGod_DjinniRunsNorth(void);

void ShrineOfTheSeaGod_DjinniHopsOntoLedge(void);
void ShrineOfTheSeaGod_ChaseDjinni(void);
void ShrineOfTheSeaGod_CastSand(void);
void ShrineOfTheSeaGod_EndSand(void);

extern u32 gFieldEffect_MediumScript[];

void ShrineOfTheSeaGod_LandPillarAsLogs(struct Actor *object, s32 base);
void ShrineOfTheSeaGod_KeepFirstPillarWithLeader(void);

void ShrineOfTheSeaGod_KeepSecondPillarWithLeader(void);

void ShrineOfTheSeaGod_HideSlideActors(void);
void ShrineOfTheSeaGod_TrackUpperLevel(void);
void ShrineOfTheSeaGod_LayerCrossroadsProps(void);

void ShrineOfTheSeaGod_KeepFlameOnTorch(void);

void ShrineOfTheSeaGod_SetUpPuddle(s32 actor);

void ShrineOfTheSeaGod_InitializeRope(void);
void ShrineOfTheSeaGod_InitializePlankBridge(void);

void ShrineOfTheSeaGod_InitializeCrossroads(void);

void ShrineOfTheSeaGod_InitializeFootprints(void);
void ShrineOfTheSeaGod_InitializeBreath(void);

void ShrineOfTheSeaGod_InitializePillarHole(void);

void ShrineOfTheSeaGod_InitializePillars(void);
void ShrineOfTheSeaGod_InitializeSeaGodsTear(void);

void ShrineOfTheSeaGod_InitializeStatue(void);

void ShrineOfTheSeaGod_InitializeRightProng(void);

void ShrineOfTheSeaGod_LowerWater(void);

void ShrineOfTheSeaGod_RiseOnWater(void);

#endif
