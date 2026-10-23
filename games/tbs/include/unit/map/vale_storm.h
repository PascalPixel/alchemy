#ifndef ALCHEMY_UNIT_SCENE_VALE_STORM_H
#define ALCHEMY_UNIT_SCENE_VALE_STORM_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *ValeStorm_GetEntrances(void);
const struct SceneRegion *ValeStorm_GetRegions(void);
const u32 *ValeStorm_GetExits(void);
const struct ScenePlacement *ValeStorm_GetPlacements(void);
const struct SceneEvent *ValeStorm_GetEvents(void);
s32 ValeStorm_Initialize(void);
extern u8 gValeStorm_FrameModes[];
extern s32 gValeStorm_ShovingManScript[];
extern s32 gValeStorm_UnusedGaretRunsUpScript[];
extern s32 gValeStorm_UnusedGaretPantingScript[];
extern s32 gValeStorm_BoulderWaits[];
extern s32 gValeStorm_BoulderFallLeaderPath[];
extern s32 gValeStorm_BoulderFallGaretPath[];
extern s32 gValeStorm_DoraRunsIntoRiversideHouse[];
extern s32 gValeStorm_JennaRunsIntoRiversideHouse[];
extern s32 gValeStorm_DoraComesOutOfRiversideHouse[];
extern s32 gValeStorm_JennaComesOutOfRiversideHouse[];
extern s32 gValeStorm_DoraHeadsNorth[];
extern s32 gValeStorm_JennaHeadsForPlaza[];
extern s32 gValeStorm_DoraLeavesForHelp[];
extern s32 gValeStorm_DoraRunsToRiver[];
extern s32 gValeStorm_ToRiverbankPath[];
extern s32 gValeStorm_ExhaustedStrongmanGatherPath[];
extern s32 gValeStorm_JennaGatherPath[];
extern s32 gValeStorm_LeaderGatherPath[];
extern s32 gValeStorm_GaretGatherPath[];
extern s32 gValeStorm_LookAroundScript[];
extern s32 gValeStorm_SquashAndStretchScript[];
extern s32 gValeStorm_BoulderRollsDownriver[];
extern struct SceneEntrance gValeStorm_Entrances[];
extern u32 gValeStorm_Exits[];
extern struct ScenePlacement gValeStorm_Placements[];
extern struct ScenePlacement gValeStorm_BoulderWatchers[];
extern struct SceneEvent gValeStorm_Events[];
extern u16 gValeStorm_SanctumDoorOpens[];
extern u16 gValeStorm_IsaacsDoorOpens[];
extern u16 gValeStorm_GrandparentsDoorOpens[];
extern u16 gValeStorm_WestHouseDoorOpens[];
extern u16 gValeStorm_IsaacsDoorCloses[];
extern u16 gValeStorm_GrandparentsDoorCloses[];

#endif
