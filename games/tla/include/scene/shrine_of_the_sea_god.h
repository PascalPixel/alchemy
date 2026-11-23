#ifndef ALCHEMY_SCENE_SHRINE_OF_THE_SEA_GOD_H
#define ALCHEMY_SCENE_SHRINE_OF_THE_SEA_GOD_H

/// @file
/// The Shrine of the Sea God: its footprints and its events.

#include "types.h"

struct Actor;

/// A footprint of the Shrine of the Sea God (gShrineOfTheSeaGod_Footprints):
/// where it lies in 16.16 pixels, its scale and its turn.
struct ShrineOfTheSeaGodSpot {
  s32 x;
  s32 z;
  s32 scaleX;
  s32 rotation;
};

void ShrineOfTheSeaGod_DjinniRunsWest(void);
void ShrineOfTheSeaGod_SetUpFloatingLog(s32 actor);

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

void ShrineOfTheSeaGod_BreakCrossroadsBridge(void);

void ShrineOfTheSeaGod_BreakFirstBridge(void);

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

void ShrineOfTheSeaGod_LandBrokenBridge(struct Actor *actor, s32 base);
void ShrineOfTheSeaGod_KeepFirstBridgeWithLeader(void);

void ShrineOfTheSeaGod_KeepSecondBridgeWithLeader(void);

void ShrineOfTheSeaGod_HideBlocks(void);
void ShrineOfTheSeaGod_TrackUpperLevel(void);
void ShrineOfTheSeaGod_LayerCrossroadsBridges(void);

void ShrineOfTheSeaGod_KeepFlameOnTorch(void);

void ShrineOfTheSeaGod_SetUpPuddle(s32 actorId);

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
