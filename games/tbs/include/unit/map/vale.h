#ifndef ALCHEMY_UNIT_SCENE_VALE_H
#define ALCHEMY_UNIT_SCENE_VALE_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *Vale_GetEntrances(void);
const struct SceneRegion *Vale_GetRegions(void);
const u32 *Vale_GetExits(void);
const struct ScenePlacement *Vale_GetPlacements(void);
const struct SceneEvent *Vale_GetEvents(void);
s32 Vale_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gVale_AlephGuardPatrol[];
extern s32 gVale_LeaderHammers[];
extern s32 gVale_GaretWalksToTheHouse[];
extern s32 gVale_UnusedWaterGrows[];
extern s32 gVale_SparkleScript[];
extern struct SceneEntrance gVale_Entrances[];
extern u32 gVale_Exits[];
extern struct ScenePlacement gVale_Placements[];
extern struct ScenePlacement gVale_PlacementsAfterLeaving[];
extern struct ScenePlacement gVale_PlacementsAfterKalay[];
extern struct ScenePlacement gVale_RepairPlacements[];
extern struct SceneEvent gVale_Events[];
extern struct SceneEvent gVale_EventsAfterLeaving[];
extern u16 gVale_SanctumDoorOpens[];
extern u16 gVale_IsaacsDoorOpens[];
extern u16 gVale_GrandparentsDoorOpens[];
extern u16 gVale_WestHouseDoorOpens[];
extern s32 gVale_FirstWarningLeaderPath[];
extern s32 gVale_FirstWarningGuardPath[];
extern s32 gVale_LeaderFromWestPath[];
extern s32 gVale_GuardFromFirstPausePath[];
extern s32 gVale_LeaderPushedBackPath[];
extern s32 gVale_GuardFromSecondPausePath[];

#endif
