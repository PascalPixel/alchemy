#ifndef ALCHEMY_UNIT_MAP_VALE_H
#define ALCHEMY_UNIT_MAP_VALE_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *Vale_GetEntrances(void);
const struct SceneExitRegion *Vale_GetRegions(void);
const u32 *Vale_GetExits(void);
const struct ObjectEventTemplate *Vale_GetObjectEventTemplates(void);
const struct MapEvent *Vale_GetEvents(void);
s32 Vale_Initialize(void);
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern s32 gVale_AlephGuardPatrol[];
extern s32 gVale_LeaderHammers[];
extern s32 gVale_GaretWalksToTheHouse[];
extern s32 gVale_UnusedWaterGrows[];
extern s32 gVale_SparkleScript[];
extern struct SceneEntrance gVale_Entrances[];
extern u32 gVale_Exits[];
extern struct ObjectEventTemplate gVale_ObjectEventTemplates[];
extern struct ObjectEventTemplate gVale_ObjectEventTemplatesAfterLeaving[];
extern struct ObjectEventTemplate gVale_ObjectEventTemplatesAfterKalay[];
extern struct ObjectEventTemplate gVale_RepairObjectEventTemplates[];
extern struct MapEvent gVale_Events[];
extern struct MapEvent gVale_EventsAfterLeaving[];
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
