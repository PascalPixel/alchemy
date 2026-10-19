#ifndef ALCHEMY_UNIT_SCENE_SUHALLA_GATE_H
#define ALCHEMY_UNIT_SCENE_SUHALLA_GATE_H

#include "types.h"
#include "field_scene.h"

struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gSuhallaGateWoundedNoCollision[];
extern s32 gSuhallaGateWounded[];
extern s32 gSuhallaGateWoundedFlippedNoCollision[];
extern s32 gSuhallaGateWoundedFlipped[];
extern struct SceneEntrance gSuhallaGateEntrancesOther[];
extern struct SceneEntrance gSuhallaGateEntrances2[];
extern struct SceneEntrance gSuhallaGateEntrances3[];
extern u32 gSuhallaGateExits[];
extern struct ScenePlacement gSuhallaGatePlacementsOther[];
extern struct ScenePlacement gSuhallaGatePlacements2[];
extern struct ScenePlacement gSuhallaGatePlacements1[];
extern struct ScenePlacement gSuhallaGatePlacements1AfterAudience[];
extern struct SceneEvent gSuhallaGateEventsOther[];
extern struct SceneEvent gSuhallaGateEvents2[];
extern struct SceneEvent gSuhallaGateEvents3[];

#endif
