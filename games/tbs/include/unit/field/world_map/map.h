#ifndef ALCHEMY_UNIT_FIELD_WORLD_MAP_MAP_H
#define ALCHEMY_UNIT_FIELD_WORLD_MAP_MAP_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

u8 *WorldMap_GetEntrances(void);
s32 WorldMap_GetRegions(void);
u8 *WorldMap_GetExits(void);
struct ScenePlacement *StoryScene_SelectPlacementTable(void);
struct SceneEvent *WorldMap_GetEvents(void);
s32 WorldMap_EnterScene(void);
extern s32 gWorldMapTriggerActor;
extern u16 gWorldMapBlend;
extern s32 gTransferArrive10[];
extern s32 gTransferDepart10[];
extern s32 gTransferReturn10[];
extern s32 gTransferGather[];
extern s32 gTransferArrive11[];
extern s32 gTransferDepart11[];
extern s32 gTransferReturn11[];
extern s32 gTransferArrive12[];
extern s32 gTransferDepart12[];
extern s32 gTransferReturn12[];
extern s32 gTransferArrive13[];
extern s32 gTransferDepart13[];
extern s32 gTransferReturn13[];
extern s32 gTransferGuide14[];
extern s32 gBlackOrbLeaderScript[];
extern s32 gPresentGuide9[];
extern s32 gPresentGuide8[];
extern s32 gPresentGuide5[];
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
extern struct ScenePlacement gWorldMapPlacements49[];
extern struct ScenePlacement gWorldMapPlacements65[];
extern struct ScenePlacement gWorldMapPlacements66[];
extern struct ScenePlacement gWorldMapPlacements71[];
extern struct ScenePlacement gWorldMapPlacements80[];
extern struct ScenePlacement gWorldMapPlacements72[];
extern struct ScenePlacement gWorldMapPlacements73[];
extern struct SceneEvent gWorldMapEvents[];
extern u32 gWorldMapRewards[];
extern s32 gActorEightPuffScript[];

#endif
