#ifndef ALCHEMY_UNIT_MAP_VALE_H
#define ALCHEMY_UNIT_MAP_VALE_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *Vale_GetEntrances(void);
const struct SceneExitRegion *Vale_GetRegions(void);
const u32 *Vale_GetExits(void);
const struct ActorTemplate *Vale_GetActorTemplates(void);
const struct Trigger *Vale_GetTriggers(void);
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
extern struct ActorTemplate gVale_ActorTemplates[];
extern struct ActorTemplate gVale_ActorTemplatesAfterLeaving[];
extern struct ActorTemplate gVale_ActorTemplatesAfterKalay[];
extern struct ActorTemplate gVale_RepairActorTemplates[];
extern struct Trigger gVale_Triggers[];
extern struct Trigger gVale_TriggersAfterLeaving[];
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
