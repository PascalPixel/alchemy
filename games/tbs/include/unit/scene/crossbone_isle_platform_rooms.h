#ifndef ALCHEMY_UNIT_SCENE_CROSSBONE_ISLE_PLATFORM_ROOMS_H
#define ALCHEMY_UNIT_SCENE_CROSSBONE_ISLE_PLATFORM_ROOMS_H

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
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gCrossboneIslePlatformRooms_BaneSquashScript[];
extern s32 gCrossboneIslePlatformRooms_BaneWalkTargets[];
extern struct SceneEntrance gCrossboneIslePlatformRoomsEntrancesOther[];
extern struct SceneEntrance gCrossboneIslePlatformRoomsEntrances1[];
extern struct SceneEntrance gCrossboneIslePlatformRoomsEntrances2[];
extern struct SceneEntrance gCrossboneIslePlatformRoomsEntrances3[];
extern u32 gCrossboneIslePlatformRoomsExits[];
extern struct ScenePlacement gCrossboneIslePlatformRoomsPlacementsOther[];
extern struct ScenePlacement gCrossboneIslePlatformRoomsPlacements1[];
extern struct ScenePlacement gCrossboneIslePlatformRoomsPlacements2[];
extern struct ScenePlacement gCrossboneIslePlatformRoomsPlacements3[];
extern struct SceneEvent gCrossboneIslePlatformRoomsEventsOther[];
extern struct SceneEvent gCrossboneIslePlatformRoomsEvents1[];
extern struct SceneEvent gCrossboneIslePlatformRoomsEvents2[];
extern struct SceneEvent gCrossboneIslePlatformRoomsEvents3[];
extern u32 gCrossboneIslePlatformRooms_SlotColumns[];
extern s32 gCrossboneIslePlatformRooms_IconTimer;

#endif
