#ifndef ALCHEMY_UNIT_SCENE_ALTIN_PEAK_H
#define ALCHEMY_UNIT_SCENE_ALTIN_PEAK_H

#include "types.h"
#include "field_scene.h"

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
extern struct SceneEntrance gAltinPeak_Entrances1[];
extern struct SceneEntrance gAltinPeak_Entrances2[];
extern struct SceneEntrance gAltinPeak_Entrances3[];
extern struct SceneEntrance gAltinPeak_Entrances4[];
extern struct SceneEntrance gAltinPeak_Entrances5[];
extern struct SceneEntrance gAltinPeak_Entrances6[];
extern struct SceneEntrance gAltinPeak_Entrances7[];
extern struct SceneEntrance gAltinPeak_Entrances8[];
extern struct SceneEntrance gAltinPeak_Entrances9[];
extern struct SceneEntrance gAltinPeak_Entrances10[];
extern struct SceneEntrance gAltinPeak_Entrances11[];
extern s16 gAltinPeak_Regions9[];
extern s16 gAltinPeak_Regions10[];
extern u32 gAltinPeak_Exits[];
extern struct ScenePlacement gAltinPeak_PlacementsOther[];
extern struct ScenePlacement gAltinPeak_Placements1[];
extern struct ScenePlacement gAltinPeak_Placements3[];
extern struct ScenePlacement gAltinPeak_Placements5[];
extern struct ScenePlacement gAltinPeak_Placements6[];
extern struct ScenePlacement gAltinPeak_Placements7[];
extern struct ScenePlacement gAltinPeak_Placements8[];
extern struct ScenePlacement gAltinPeak_Placements9[];
extern struct ScenePlacement gAltinPeak_Placements10[];
extern struct ScenePlacement gAltinPeak_Placements11[];
extern struct SceneEvent gAltinPeak_EventsOther[];
extern struct SceneEvent gAltinPeak_Events1[];
extern struct SceneEvent gAltinPeak_Events1Drained[];
extern struct SceneEvent gAltinPeak_Events2[];
extern struct SceneEvent gAltinPeak_Events3[];
extern struct SceneEvent gAltinPeak_Events4[];
extern struct SceneEvent gAltinPeak_Events5[];
extern struct SceneEvent gAltinPeak_Events6[];
extern struct SceneEvent gAltinPeak_Events7[];
extern struct SceneEvent gAltinPeak_Events8[];
extern struct SceneEvent gAltinPeak_Events9[];
extern struct SceneEvent gAltinPeak_Events10[];
extern struct SceneEvent gAltinPeak_Events11[];
extern s32 gAltinPeak_RumbleTicks;
extern u32 gAltinPeak_RollRadii[];

#endif
