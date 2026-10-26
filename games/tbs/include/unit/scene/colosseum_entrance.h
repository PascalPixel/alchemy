#ifndef ALCHEMY_UNIT_MAP_COLOSSEUM_ENTRANCE_H
#define ALCHEMY_UNIT_MAP_COLOSSEUM_ENTRANCE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ObjectEventTemplate;
struct SceneExitRegion;

const struct SceneEntrance *ColosseumEntrance_GetEntrances(void);
const struct SceneExitRegion *ColosseumEntrance_GetRegions(void);
const u32 *ColosseumEntrance_GetExits(void);
const struct ObjectEventTemplate *
ColosseumEntrance_GetObjectEventTemplates(void);
const struct MapEvent *ColosseumEntrance_GetEvents(void);
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
extern struct ObjectEventTemplate gColosseumEntranceObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gColosseumStandsObjectEventTemplates[];
extern struct ObjectEventTemplate gColosseumTrialGroundsObjectEventTemplates[];
extern struct ObjectEventTemplate gColosseumObjectEventTemplates[];
extern struct ObjectEventTemplate gColosseumSoldiersObjectEventTemplates[];
extern struct ObjectEventTemplate
    gColosseumObjectEventTemplatesFromColosseumAzartMatchWest[];
extern struct ObjectEventTemplate
    gColosseumObjectEventTemplatesFromColosseumNavampaMatch[];
extern struct ObjectEventTemplate gColosseumEntranceObjectEventTemplates[];
extern struct ObjectEventTemplate
    gColosseumEntranceObjectEventTemplatesFinalsDay[];
extern struct ObjectEventTemplate
    gColosseumEntranceObjectEventTemplatesAfterColosso[];
extern struct MapEvent gColosseumEntranceEventsOther[];
extern struct MapEvent gColosseumTunnelEvents[];
extern struct MapEvent gColosseumEvents[];
extern struct MapEvent gColosseumEntranceEvents[];
extern struct MapEvent gColosseumEventsFromColosseumAzartMatchWest[];

#endif
