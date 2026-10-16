#ifndef ALCHEMY_UNIT_SCENE_CROSSBONE_ISLE_PLATFORM_ROOMS_H
#define ALCHEMY_UNIT_SCENE_CROSSBONE_ISLE_PLATFORM_ROOMS_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u32 *SceneData_GetSceneTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gCrossboneIslePlatformRooms_ActorFifteenMotionScript[];
extern s32 gCrossboneIslePlatformRooms_ActorFifteenWalkTargets[];
extern struct SceneEntrance gCrossboneIslePlatformRoomsEntrancesOther[];
extern struct SceneEntrance gCrossboneIslePlatformRoomsEntrances1[];
extern struct SceneEntrance gCrossboneIslePlatformRoomsEntrances2[];
extern struct SceneEntrance gCrossboneIslePlatformRoomsEntrances3[];
extern u32 gCrossboneIslePlatformRooms_SceneTable[];
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
