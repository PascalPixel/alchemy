#ifndef ALCHEMY_UNIT_MAP_ALTIN_PEAK_H
#define ALCHEMY_UNIT_MAP_ALTIN_PEAK_H

#include "types.h"
#include "map_tables.h"
#include "scene_header.h"

struct SceneEntrance;
struct MapEvent;
struct ObjectEventTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
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
extern struct SceneEntrance gAltinPeak_EntrancesOther[];
extern struct SceneEntrance gAltinPeak_FirstWaterBeastEntrances[];
extern struct SceneEntrance gAltinPeak_RailTunnelEntrances[];
extern struct SceneEntrance gAltinPeak_SecondWaterBeastEntrances[];
extern struct SceneEntrance gAltinPeak_SouthEntranceEntrances[];
extern struct SceneEntrance gAltinPeak_ThirdWaterBeastEntrances[];
extern struct SceneEntrance gAltinPeak_PoolEntrances[];
extern struct SceneEntrance gAltinPeak_SpritzEntrances[];
extern struct SceneEntrance gAltinPeak_JunctionEntrances[];
extern struct SceneEntrance gAltinPeak_FallingRocksEntrances[];
extern struct SceneEntrance gAltinPeak_HydrosStatueEntrances[];
extern struct SceneEntrance gAltinPeak_WestExitEntrances[];
extern struct SceneExitRegion gAltinPeak_FallingRocksRegions[];
extern struct SceneExitRegion gAltinPeak_HydrosStatueRegions[];
extern u32 gAltinPeak_Exits[];
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
