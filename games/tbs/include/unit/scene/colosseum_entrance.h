#ifndef ALCHEMY_UNIT_SCENE_COLOSSEUM_ENTRANCE_H
#define ALCHEMY_UNIT_SCENE_COLOSSEUM_ENTRANCE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance  *Scene_GetEntrances(void);
const struct SceneRegion    *Scene_GetRegions(void);
const u32                   *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          Scene_Initialize(void);
extern s32                   gColosseumEntrance_ActionTable1[];
extern s32                   gColosseumEntrance_ActionTable2[];
extern s32                   gColosseumEntrance_ActionTable3[];
extern s32                   gColosseumEntrance_ActionTable4[];
extern s32                   gColosseumEntrance_ActionTable5[];
extern s32                   gColosseumEntrance_ActionTable6[];
extern s32                   gColosseumEntrance_ActionTable7[];
extern s32                   gColosseumEntrance_ActionTable8[];
extern s32                   gColosseumEntrance_ActionTable9[];
extern struct SceneEntrance  gColosseumEntranceEntrancesOther[];
extern struct SceneEntrance  gColosseumEntranceEntrances1[];
extern struct SceneEntrance  gColosseumEntranceEntrances3[];
extern u32                   gColosseumEntranceExits[];
extern struct ScenePlacement gColosseumEntrancePlacementsOther[];
extern struct ScenePlacement gColosseumEntrancePlacements1Entrance5[];
extern struct ScenePlacement gColosseumEntrancePlacements1Entrance7[];
extern struct ScenePlacement gColosseumEntrancePlacements1[];
extern struct ScenePlacement gColosseumEntrancePlacements1Entrance8[];
extern struct ScenePlacement gColosseumEntrancePlacements1Entrance12[];
extern struct ScenePlacement gColosseumEntrancePlacements1Entrance66[];
extern struct ScenePlacement gColosseumEntrancePlacements3[];
extern struct ScenePlacement gColosseumEntrancePlacements3Flag962[];
extern struct ScenePlacement gColosseumEntrancePlacements3Flag950[];
extern struct SceneEvent     gColosseumEntranceEventsOther[];
extern struct SceneEvent     gColosseumEntranceEvents2[];
extern struct SceneEvent     gColosseumEntranceEvents1[];
extern struct SceneEvent     gColosseumEntranceEvents3[];
extern struct SceneEvent     gColosseumEntranceEvents1Entrance12[];

#endif
