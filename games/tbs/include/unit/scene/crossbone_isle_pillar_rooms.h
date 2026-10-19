#ifndef ALCHEMY_UNIT_SCENE_CROSSBONE_ISLE_PILLAR_ROOMS_H
#define ALCHEMY_UNIT_SCENE_CROSSBONE_ISLE_PILLAR_ROOMS_H

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
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern const s32 *gEffectScripts[];
extern s32 gCrossboneIslePillarRooms_ActionTable[];
extern u16 gCrossboneIslePillarRooms_ShiftSteps1[];
extern u16 gCrossboneIslePillarRooms_ShiftSteps2[];
extern u16 gCrossboneIslePillarRooms_ShiftSteps3[];
extern u16 gCrossboneIslePillarRooms_ShiftSteps4[];
extern u16 gCrossboneIslePillarRooms_ShiftSteps5[];
extern struct SceneEntrance gCrossboneIslePillarRoomsEntrancesOther[];
extern struct SceneEntrance gCrossboneIslePillarRoomsEntrances1[];
extern struct SceneEntrance gCrossboneIslePillarRoomsEntrances2[];
extern struct SceneEntrance gCrossboneIslePillarRoomsEntrances3[];
extern struct SceneEntrance gCrossboneIslePillarRoomsEntrances4[];
extern struct SceneEntrance gCrossboneIslePillarRoomsEntrances5[];
extern u32 gCrossboneIslePillarRoomsExits[];
extern struct ScenePlacement gCrossboneIslePillarRoomsNoPlacements[];
extern struct ScenePlacement gCrossboneIslePillarRoomsPlacements1[];
extern struct ScenePlacement gCrossboneIslePillarRoomsPlacements2[];
extern struct ScenePlacement gCrossboneIslePillarRoomsPlacements3[];
extern struct ScenePlacement gCrossboneIslePillarRoomsPlacementsOther[];
extern struct ScenePlacement gCrossboneIslePillarRoomsPlacements5[];
extern struct SceneEvent gCrossboneIslePillarRoomsEventsOther[];
extern struct SceneEvent gCrossboneIslePillarRoomsEvents1[];
extern struct SceneEvent gCrossboneIslePillarRoomsEvents2[];
extern struct SceneEvent gCrossboneIslePillarRoomsEvents3[];
extern struct SceneEvent gCrossboneIslePillarRoomsEvents4[];
extern struct SceneEvent gCrossboneIslePillarRoomsEvents5[];

#endif
