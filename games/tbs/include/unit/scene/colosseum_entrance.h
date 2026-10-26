#ifndef ALCHEMY_UNIT_MAP_COLOSSEUM_ENTRANCE_H
#define ALCHEMY_UNIT_MAP_COLOSSEUM_ENTRANCE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *ColosseumEntrance_GetEntrances(void);
const struct SceneExitRegion *ColosseumEntrance_GetRegions(void);
const u32 *ColosseumEntrance_GetExits(void);
const struct ActorTemplate *ColosseumEntrance_GetActorTemplates(void);
const struct Trigger *ColosseumEntrance_GetTriggers(void);
s32 ColosseumEntrance_Initialize(void);
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
extern struct ActorTemplate gColosseumEntranceActorTemplatesOther[];
extern struct ActorTemplate gColosseumStandsActorTemplates[];
extern struct ActorTemplate gColosseumTrialGroundsActorTemplates[];
extern struct ActorTemplate gColosseumActorTemplates[];
extern struct ActorTemplate gColosseumSoldiersActorTemplates[];
extern struct ActorTemplate
    gColosseumActorTemplatesFromColosseumAzartMatchWest[];
extern struct ActorTemplate gColosseumActorTemplatesFromColosseumNavampaMatch[];
extern struct ActorTemplate gColosseumEntranceActorTemplates[];
extern struct ActorTemplate gColosseumEntranceActorTemplatesFinalsDay[];
extern struct ActorTemplate gColosseumEntranceActorTemplatesAfterColosso[];
extern struct Trigger gColosseumEntranceTriggersOther[];
extern struct Trigger gColosseumTunnelTriggers[];
extern struct Trigger gColosseumTriggers[];
extern struct Trigger gColosseumEntranceTriggers[];
extern struct Trigger gColosseumTriggersFromColosseumAzartMatchWest[];

#endif
