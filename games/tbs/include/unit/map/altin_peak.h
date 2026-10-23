#ifndef ALCHEMY_UNIT_MAP_ALTIN_PEAK_H
#define ALCHEMY_UNIT_MAP_ALTIN_PEAK_H

#include "types.h"
#include "map_tables.h"
#include "field_runtime.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern s8 gAltinPeak_PaletteScript[];
extern s32 gAltinPeak_FaceLeaderScript[];
extern s32 gAltinPeak_PuddleGrowScript[];
extern s32 gAltinPeak_BoulderPath[];
extern s32 gAltinPeak_LeaderFleePath[];
extern s32 gAltinPeak_DebrisScript[];
extern s32 gAltinPeak_GaretScript[];
extern s32 gAltinPeak_IvanScript[];
extern s32 gAltinPeak_MiaScript[];
extern s32 gAltinPeak_StatueSinkScript[];
extern s32 gAltinPeak_RejoinScript[];
extern struct MapWarpDest gAltinPeak_WarpDestsOther[];
extern struct MapWarpDest gAltinPeak_FirstWaterBeastWarpDests[];
extern struct MapWarpDest gAltinPeak_RailTunnelWarpDests[];
extern struct MapWarpDest gAltinPeak_SecondWaterBeastWarpDests[];
extern struct MapWarpDest gAltinPeak_SouthEntranceWarpDests[];
extern struct MapWarpDest gAltinPeak_ThirdWaterBeastWarpDests[];
extern struct MapWarpDest gAltinPeak_PoolWarpDests[];
extern struct MapWarpDest gAltinPeak_SpritzWarpDests[];
extern struct MapWarpDest gAltinPeak_JunctionWarpDests[];
extern struct MapWarpDest gAltinPeak_FallingRocksWarpDests[];
extern struct MapWarpDest gAltinPeak_HydrosStatueWarpDests[];
extern struct MapWarpDest gAltinPeak_WestExitWarpDests[];
extern struct MapWarpRegion gAltinPeak_FallingRocksRegions[];
extern struct MapWarpRegion gAltinPeak_HydrosStatueRegions[];
extern u32 gAltinPeak_Warps[];
extern struct ObjectEventTemplate gAltinPeak_ObjectEventTemplatesOther[];
extern struct ObjectEventTemplate
    gAltinPeak_FirstWaterBeastObjectEventTemplates[];
extern struct ObjectEventTemplate
    gAltinPeak_SecondWaterBeastObjectEventTemplates[];
extern struct ObjectEventTemplate
    gAltinPeak_ThirdWaterBeastObjectEventTemplates[];
extern struct ObjectEventTemplate gAltinPeak_PoolObjectEventTemplates[];
extern struct ObjectEventTemplate gAltinPeak_SpritzObjectEventTemplates[];
extern struct ObjectEventTemplate gAltinPeak_JunctionObjectEventTemplates[];
extern struct ObjectEventTemplate gAltinPeak_FallingRocksObjectEventTemplates[];
extern struct ObjectEventTemplate gAltinPeak_HydrosStatueObjectEventTemplates[];
extern struct ObjectEventTemplate gAltinPeak_WestExitObjectEventTemplates[];
extern struct MapEvent gAltinPeak_EventsOther[];
extern struct MapEvent gAltinPeak_FirstWaterBeastEvents[];
extern struct MapEvent gAltinPeak_FirstWaterBeastEventsDrained[];
extern struct MapEvent gAltinPeak_RailTunnelEvents[];
extern struct MapEvent gAltinPeak_SecondWaterBeastEvents[];
extern struct MapEvent gAltinPeak_SouthEntranceEvents[];
extern struct MapEvent gAltinPeak_ThirdWaterBeastEvents[];
extern struct MapEvent gAltinPeak_PoolEvents[];
extern struct MapEvent gAltinPeak_SpritzEvents[];
extern struct MapEvent gAltinPeak_JunctionEvents[];
extern struct MapEvent gAltinPeak_FallingRocksEvents[];
extern struct MapEvent gAltinPeak_HydrosStatueEvents[];
extern struct MapEvent gAltinPeak_WestExitEvents[];
extern s32 gAltinPeak_RumbleTicks;
extern u32 gAltinPeak_RollRadii[];

#endif
