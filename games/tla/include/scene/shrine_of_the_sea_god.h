#ifndef ALCHEMY_SCENE_SHRINE_OF_THE_SEA_GOD_H
#define ALCHEMY_SCENE_SHRINE_OF_THE_SEA_GOD_H

/* @file */
/* The Shrine of the Sea God: its footprints and its events. */

#include "types.h"

struct ACTOR;

/* A footprint of the Shrine of the Sea God (ShrineOfTheSeaGod_Footprints): */
/* where it lies in 16.16 pixels, its scale and its turn. */
typedef struct SHRINE_OF_THE_SEA_GOD_SPOT {
	s32 x;
	s32 z;
	s32 scaleX;
	s32 rotation;
} SHRINE_OF_THE_SEA_GOD_SPOT;

void scShrineOfTheSeaGodDjinniRunsWest(void);
void scShrineOfTheSeaGodSetUpFloatingLog(s32 actor);

void scShrineOfTheSeaGodStartleRiki(void);

void scShrineOfTheSeaGodOpenFieldMenu(void);

void scShrineOfTheSeaGodUseSeaGodsTear(void);

void scShrineOfTheSeaGodSinkWithWater(void);

void scShrineOfTheSeaGodFreezeFirstPuddle(void);

void scShrineOfTheSeaGodFreezeSecondPuddle(void);

void scShrineOfTheSeaGodFreezeThirdPuddle(void);

void scShrineOfTheSeaGodLiftBlock(void);

void scShrineOfTheSeaGodSetDownTorch(void);

void scShrineOfTheSeaGodMovePillar(void);

void scShrineOfTheSeaGodDrawLeaderInFront(void);

void scShrineOfTheSeaGodDrawLeaderBehind(void);

void scShrineOfTheSeaGodBreakCrossroadsBridge(void);

void scShrineOfTheSeaGodBreakFirstBridge(void);

void scShrineOfTheSeaGodHideStone(void);
void scShrineOfTheSeaGodThrowRopeAgain(void);
void scShrineOfTheSeaGodLashRope(void);
void scShrineOfTheSeaGodDjinniFleesEast(void);

void scShrineOfTheSeaGodDjinniFleesSouth(void);

void scShrineOfTheSeaGodDjinniRunsNorth(void);

void scShrineOfTheSeaGodDjinniHopsOntoLedge(void);
void scShrineOfTheSeaGodChaseDjinni(void);
void scShrineOfTheSeaGodCastSand(void);
void scShrineOfTheSeaGodEndSand(void);

extern u32 FieldEffect_MediumScript[];

void scShrineOfTheSeaGodLandBrokenBridge(struct ACTOR *actor, s32 base);
void scShrineOfTheSeaGodKeepFirstBridgeWithLeader(void);

void scShrineOfTheSeaGodKeepSecondBridgeWithLeader(void);

void scShrineOfTheSeaGodHideBlocks(void);
void scShrineOfTheSeaGodTrackUpperLevel(void);
void scShrineOfTheSeaGodLayerCrossroadsBridges(void);

void scShrineOfTheSeaGodKeepFlameOnTorch(void);

void scShrineOfTheSeaGodSetUpPuddle(s32 actorId);

void scShrineOfTheSeaGodInitRope(void);
void scShrineOfTheSeaGodInitPlankBridge(void);

void scShrineOfTheSeaGodInitCrossroads(void);

void scShrineOfTheSeaGodInitFootprints(void);
void scShrineOfTheSeaGodInitBreath(void);

void scShrineOfTheSeaGodInitPillarHole(void);

void scShrineOfTheSeaGodInitPillars(void);
void scShrineOfTheSeaGodInitSeaGodsTear(void);

void scShrineOfTheSeaGodInitStatue(void);

void scShrineOfTheSeaGodInitRightProng(void);

void scShrineOfTheSeaGodLowerWater(void);

void scShrineOfTheSeaGodRiseOnWater(void);

#endif
