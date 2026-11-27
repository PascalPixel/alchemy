#ifndef ALCHEMY_SCENE_SHRINE_OF_THE_SEA_GOD_H
#define ALCHEMY_SCENE_SHRINE_OF_THE_SEA_GOD_H

/* The Shrine of the Sea God: its footprints and its scripts. */

#include "types.h"

struct ACTOR;

/* A footprint of the Shrine of the Sea God (ShrineOfTheSeaGodFootprintsTable):
   where it lies in 16.16 pixels, its scale and its turn. */
typedef struct SHRINE_OF_THE_SEA_GOD_SPOT {
	Sint32 x;
	Sint32 z;
	Sint32 scaleX;
	Sint32 rotation;
} SHRINE_OF_THE_SEA_GOD_SPOT;

void scShrineOfTheSeaGodDjinniRunsWest(void);
void scShrineOfTheSeaGodSetUpFloatingLog(Sint32 actor);

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

extern Uint32 FieldEffectMediumScript[];

void scShrineOfTheSeaGodLandBrokenBridge(struct ACTOR *actor, Sint32 base);
void scShrineOfTheSeaGodKeepFirstBridgeWithLeader(void);

void scShrineOfTheSeaGodKeepSecondBridgeWithLeader(void);

void scShrineOfTheSeaGodHideBlocks(void);
void scShrineOfTheSeaGodTrackUpperLevel(void);
void scShrineOfTheSeaGodLayerCrossroadsBridges(void);

void scShrineOfTheSeaGodKeepFlameOnTorch(void);

void scShrineOfTheSeaGodSetUpPuddle(Sint32 actorId);

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
