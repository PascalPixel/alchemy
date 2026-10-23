#ifndef ALCHEMY_UNIT_MAP_VALE_STORMY_NIGHT_H
#define ALCHEMY_UNIT_MAP_VALE_STORMY_NIGHT_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *ValeStormyNight_GetWarpDests(void);
const struct MapWarpRegion *ValeStormyNight_GetRegions(void);
const u32 *ValeStormyNight_GetWarps(void);
const struct ObjectEventTemplate *ValeStormyNight_GetObjectEventTemplates(void);
const struct MapEvent *ValeStormyNight_GetEvents(void);
s32 ValeStormyNight_Initialize(void);
extern u8 gValeStormyNight_FrameModes[];
extern s32 gValeStormyNight_ShovingManScript[];
extern s32 gValeStormyNight_UnusedGaretRunsUpScript[];
extern s32 gValeStormyNight_UnusedGaretPantingScript[];
extern s32 gValeStormyNight_BoulderWaits[];
extern s32 gValeStormyNight_BoulderFallLeaderPath[];
extern s32 gValeStormyNight_BoulderFallGaretPath[];
extern s32 gValeStormyNight_DoraRunsIntoRiversideHouse[];
extern s32 gValeStormyNight_JennaRunsIntoRiversideHouse[];
extern s32 gValeStormyNight_DoraComesOutOfRiversideHouse[];
extern s32 gValeStormyNight_JennaComesOutOfRiversideHouse[];
extern s32 gValeStormyNight_DoraHeadsNorth[];
extern s32 gValeStormyNight_JennaHeadsForPlaza[];
extern s32 gValeStormyNight_DoraLeavesForHelp[];
extern s32 gValeStormyNight_DoraRunsToRiver[];
extern s32 gValeStormyNight_ToRiverbankPath[];
extern s32 gValeStormyNight_ExhaustedStrongmanGatherPath[];
extern s32 gValeStormyNight_JennaGatherPath[];
extern s32 gValeStormyNight_LeaderGatherPath[];
extern s32 gValeStormyNight_GaretGatherPath[];
extern s32 gValeStormyNight_LookAroundScript[];
extern s32 gValeStormyNight_SquashAndStretchScript[];
extern s32 gValeStormyNight_BoulderRollsDownriver[];
extern struct MapWarpDest gValeStormyNight_WarpDests[];
extern u32 gValeStormyNight_Warps[];
extern struct ObjectEventTemplate gValeStormyNight_ObjectEventTemplates[];
extern struct ObjectEventTemplate gValeStormyNight_BoulderWatchers[];
extern struct MapEvent gValeStormyNight_Events[];
extern u16 gValeStormyNight_SanctumDoorOpens[];
extern u16 gValeStormyNight_IsaacsDoorOpens[];
extern u16 gValeStormyNight_GrandparentsDoorOpens[];
extern u16 gValeStormyNight_WestHouseDoorOpens[];
extern u16 gValeStormyNight_IsaacsDoorCloses[];
extern u16 gValeStormyNight_GrandparentsDoorCloses[];

#endif
