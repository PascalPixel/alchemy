#ifndef ALCHEMY_MODULE_SCENE_COLOSSEUM_ENTRANCE_H
#define ALCHEMY_MODULE_SCENE_COLOSSEUM_ENTRANCE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gColosseumEntrance_GatherIntoIsaacScript[];
extern s32 gColosseumEntrance_GatherIntoGaretScript[];
extern s32 gColosseumEntrance_RejoinPartyScript[];
extern s32 gColosseumEntrance_EnterGatesScript[];
extern s32 gColosseumEntrance_ApproachFinalsGroupScript[];
extern s32 gColosseumEntrance_JoinFinalsFromEastScript[];
extern s32 gColosseumEntrance_JoinFinalsFromSouthScript[];
extern s32 gColosseumEntrance_EnterFinalsScript[];
extern s32 gColosseumEntrance_PulseScaleScript[];
extern struct SceneEntrance gColosseumEntranceEntrancesOther[];
extern struct SceneEntrance gColosseumEntrances[];
extern struct SceneEntrance gColosseumEntranceEntrances[];
extern u32 gColosseumEntranceExits[];
extern struct ActorPlacement gColosseumEntranceActorPlacementsOther[];
extern struct ActorPlacement gColosseumStandsActorPlacements[];
extern struct ActorPlacement gColosseumTrialGroundsActorPlacements[];
extern struct ActorPlacement gColosseumActorPlacements[];
extern struct ActorPlacement gColosseumSoldiersActorPlacements[];
extern struct ActorPlacement
    gColosseumActorPlacementsFromColosseumAzartMatchWest[];
extern struct ActorPlacement
    gColosseumActorPlacementsFromColosseumNavampaMatch[];
extern struct ActorPlacement gColosseumEntranceActorPlacements[];
extern struct ActorPlacement gColosseumEntranceActorPlacementsFinalsDay[];
extern struct ActorPlacement gColosseumEntranceActorPlacementsAfterColosso[];
extern struct Trigger gColosseumEntranceTriggersOther[];
extern struct Trigger gColosseumTunnelTriggers[];
extern struct Trigger gColosseumTriggers[];
extern struct Trigger gColosseumEntranceTriggers[];
extern struct Trigger gColosseumTriggersFromColosseumAzartMatchWest[];

#endif
