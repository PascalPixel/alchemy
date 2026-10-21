#ifndef ALCHEMY_UNIT_SCENE_WORLD_MAP_H
#define ALCHEMY_UNIT_SCENE_WORLD_MAP_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *WorldMap_GetEntrances(void);
const struct SceneRegion *WorldMap_GetRegions(void);
const u32 *WorldMap_GetExits(void);
const struct ScenePlacement *WorldMap_GetPlacements(void);
const struct SceneEvent *WorldMap_GetEvents(void);
s32 WorldMap_Initialize(void);
extern s32 gWorldMapTriggerActor;
extern u16 gWorldMapBlend;
extern s32 gCaravanFirstWagonArrives[];
extern s32 gCaravanFirstWagonDeparts[];
extern s32 gCaravanFirstWagonReturns[];
extern s32 gCaravanWagonHeadsNorth[];
extern s32 gCaravanSecondWagonArrives[];
extern s32 gCaravanSecondWagonDeparts[];
extern s32 gCaravanSecondWagonReturns[];
extern s32 gCaravanThirdWagonArrives[];
extern s32 gCaravanThirdWagonDeparts[];
extern s32 gCaravanThirdWagonReturns[];
extern s32 gCaravanFourthWagonArrives[];
extern s32 gCaravanFourthWagonDeparts[];
extern s32 gCaravanFourthWagonReturns[];
extern s32 gCaravanRockGuideScript[];
extern s32 gBlackOrbLeaderScript[];
extern s32 gEndingAlexWalk[];
extern s32 gEndingKradenWalk[];
extern s32 gEndingJennaWalk[];
extern s32 gTransitionSparkScript[];
extern s32 gTransferLeaderIdle[];
extern s32 gTransferLeaderTurn[];
extern s32 gOpeningLeaderRise[];
extern s32 gWorldMapPlacement0[];
extern struct SceneEntrance gWorldMapEntrances[];
extern u32 gWorldMapExits[];
extern s32 gWorldMapPlacement2[];
extern s32 gWorldMapPlacement1[];
extern s32 gWorldMapPlacement4[];
extern s32 gWorldMapPlacement3[];
extern struct ScenePlacement gWorldMapPlacements[];
extern struct ScenePlacement gWorldMapPlacements64[];
extern struct ScenePlacement gWorldMapPlacementsFromWestLunpaCave[];
extern struct ScenePlacement gWorldMapPlacements65[];
extern struct ScenePlacement gWorldMapPlacements66[];
extern struct ScenePlacement gWorldMapPlacementsFromKalayTunnelEntranceSouth[];
extern struct ScenePlacement gWorldMapPlacements80[];
extern struct ScenePlacement gWorldMapPlacementsFromClear[];
extern struct ScenePlacement gWorldMapPlacements73[];
extern struct SceneEvent gWorldMapEvents[];
extern u32 gWorldMapPassengerRowValues[];
extern s32 gIslandWakeScript[];

#endif
