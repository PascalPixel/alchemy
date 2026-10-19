#ifndef ALCHEMY_UNIT_SCENE_LUNPA_FORTRESS_H
#define ALCHEMY_UNIT_SCENE_LUNPA_FORTRESS_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneRegion;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gLunpaFortressRandomPick;
extern s32 gSecondPatrolRoute[];
extern s32 gFirstPatrolRoute[];
extern s32 gDonpaSleeps[];
extern s32 gOuterPatrolRoute[];
extern s32 gHallPatrolRoute[];
extern struct SceneEntrance gLunpaFortressEntrances1[];
extern struct SceneEntrance gLunpaFortressEntrances2[];
extern struct SceneEntrance gLunpaFortressEntrances3[];
extern struct SceneEntrance gLunpaFortressEntrancesOther[];
extern u32 gLunpaFortressExitsOther[];
extern u32 gLunpaFortressExits2[];
extern u32 gLunpaFortressExits3And4[];
extern struct ScenePlacement gLunpaFortressPlacementsOther[];
extern struct ScenePlacement gLunpaFortressPlacementsWestLunpaCave[];
extern struct ScenePlacement gLunpaFortressPlacements1[];
extern struct ScenePlacement gLunpaFortressPlacements2[];
extern struct ScenePlacement gLunpaFortressPlacements3[];
extern struct ScenePlacement gLunpaFortressPlacements4[];
extern struct SceneEvent gLunpaFortressNoEvents[];
extern struct SceneEvent gLunpaFortressEvents1[];
extern struct SceneEvent gLunpaFortressEvents2[];
extern struct SceneEvent gLunpaFortressEvents3[];
extern struct SceneEvent gLunpaFortressEventsOther[];
extern u32 gRescueRoomDoorCells[];
extern u32 gColumnRoomDoorCells[];
extern u32 gGuardRoomDoorCells[];
extern u32 gOuterDoorCells[];

#endif
