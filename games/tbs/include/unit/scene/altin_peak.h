#ifndef ALCHEMY_UNIT_SCENE_ALTIN_PEAK_H
#define ALCHEMY_UNIT_SCENE_ALTIN_PEAK_H

#include "types.h"
#include "field_scene.h"
#include "field_runtime.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
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
extern struct SceneRegion gAltinPeak_FallingRocksRegions[];
extern struct SceneRegion gAltinPeak_HydrosStatueRegions[];
extern u32 gAltinPeak_Exits[];
extern struct ScenePlacement gAltinPeak_PlacementsOther[];
extern struct ScenePlacement gAltinPeak_FirstWaterBeastPlacements[];
extern struct ScenePlacement gAltinPeak_SecondWaterBeastPlacements[];
extern struct ScenePlacement gAltinPeak_ThirdWaterBeastPlacements[];
extern struct ScenePlacement gAltinPeak_PoolPlacements[];
extern struct ScenePlacement gAltinPeak_SpritzPlacements[];
extern struct ScenePlacement gAltinPeak_JunctionPlacements[];
extern struct ScenePlacement gAltinPeak_FallingRocksPlacements[];
extern struct ScenePlacement gAltinPeak_HydrosStatuePlacements[];
extern struct ScenePlacement gAltinPeak_WestExitPlacements[];
extern struct SceneEvent gAltinPeak_EventsOther[];
extern struct SceneEvent gAltinPeak_FirstWaterBeastEvents[];
extern struct SceneEvent gAltinPeak_FirstWaterBeastEventsDrained[];
extern struct SceneEvent gAltinPeak_RailTunnelEvents[];
extern struct SceneEvent gAltinPeak_SecondWaterBeastEvents[];
extern struct SceneEvent gAltinPeak_SouthEntranceEvents[];
extern struct SceneEvent gAltinPeak_ThirdWaterBeastEvents[];
extern struct SceneEvent gAltinPeak_PoolEvents[];
extern struct SceneEvent gAltinPeak_SpritzEvents[];
extern struct SceneEvent gAltinPeak_JunctionEvents[];
extern struct SceneEvent gAltinPeak_FallingRocksEvents[];
extern struct SceneEvent gAltinPeak_HydrosStatueEvents[];
extern struct SceneEvent gAltinPeak_WestExitEvents[];
extern s32 gAltinPeak_RumbleTicks;
extern u32 gAltinPeak_RollRadii[];

#endif
