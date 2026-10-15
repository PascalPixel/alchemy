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
extern s32 gAltinPeak_ActorScript[];
extern s32 gAltinPeak_PuddleGrowScript[];
extern s32 gAltinPeak_LogRideScript[];
extern s32 gAltinPeak_LeaderRideScript[];
extern s32 gAltinPeak_SparkScript[];
extern s32 gAltinPeak_GaretScript[];
extern s32 gAltinPeak_IvanScript[];
extern s32 gAltinPeak_MiaScript[];
extern s32 gAltinPeak_CelebrateScript[];
extern s32 gAltinPeak_PartyScript[];
extern struct SceneEntrance gAltinPeakEntrancesOther[];
extern struct SceneEntrance gAltinPeakEntrances1[];
extern struct SceneEntrance gAltinPeakEntrances2[];
extern struct SceneEntrance gAltinPeakEntrances3[];
extern struct SceneEntrance gAltinPeakEntrances4[];
extern struct SceneEntrance gAltinPeakEntrances5[];
extern struct SceneEntrance gAltinPeakEntrances6[];
extern struct SceneEntrance gAltinPeakEntrances7[];
extern struct SceneEntrance gAltinPeakEntrances8[];
extern struct SceneEntrance gAltinPeakEntrances9[];
extern struct SceneEntrance gAltinPeakEntrances10[];
extern struct SceneEntrance gAltinPeakEntrances11[];
extern s16 gAltinPeakRegions9[];
extern s16 gAltinPeakRegions10[];
extern u32 gAltinMine_Exits[];
extern struct ScenePlacement gAltinPeakPlacementsOther[];
extern struct ScenePlacement gAltinPeakPlacements1[];
extern struct ScenePlacement gAltinPeakPlacements3[];
extern struct ScenePlacement gAltinPeakPlacements5[];
extern struct ScenePlacement gAltinPeakPlacements6[];
extern struct ScenePlacement gAltinPeakPlacements7[];
extern struct ScenePlacement gAltinPeakPlacements8[];
extern struct ScenePlacement gAltinPeakPlacements9[];
extern struct ScenePlacement gAltinPeakPlacements10[];
extern struct ScenePlacement gAltinPeakPlacements11[];
extern struct SceneEvent gAltinPeakEventsOther[];
extern struct SceneEvent gAltinPeakEvents1[];
extern struct SceneEvent gAltinPeakEvents1Drained[];
extern struct SceneEvent gAltinPeakEvents2[];
extern struct SceneEvent gAltinPeakEvents3[];
extern struct SceneEvent gAltinPeakEvents4[];
extern struct SceneEvent gAltinPeakEvents5[];
extern struct SceneEvent gAltinPeakEvents6[];
extern struct SceneEvent gAltinPeakEvents7[];
extern struct SceneEvent gAltinPeakEvents8[];
extern struct SceneEvent gAltinPeakEvents9[];
extern struct SceneEvent gAltinPeakEvents10[];
extern struct SceneEvent gAltinPeakEvents11[];
extern s32 gAltinPeak_CueTicks;
extern u32 gAltinPeak_RollRadii[];

#endif
