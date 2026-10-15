#ifndef ALCHEMY_UNIT_SCENE_SUHALLA_GATE_H
#define ALCHEMY_UNIT_SCENE_SUHALLA_GATE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetSceneTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 SuhallaGate_EnterScene(void);
extern s32 SuhallaGate_PatrolStartup[];
extern s32 gSuhallaGatePlacements1Flagged1[];
extern s32 SuhallaGate_FirstGuardStartup[];
extern s32 gSuhallaGatePlacements1Flagged0[];
extern struct SceneEntrance gSuhallaGateEntrancesOther[];
extern struct SceneEntrance gSuhallaGateEntrances2[];
extern struct SceneEntrance gSuhallaGateEntrances3[];
extern u32 SuhallaGate_SceneTable[];
extern struct ScenePlacement gSuhallaGatePlacementsOther[];
extern struct ScenePlacement gSuhallaGatePlacements2[];
extern struct ScenePlacement gSuhallaGatePlacements1[];
extern struct ScenePlacement gSuhallaGatePlacements1Flagged[];
extern struct SceneEvent gSuhallaGateEventsOther[];
extern struct SceneEvent gSuhallaGateEvents2[];
extern struct SceneEvent gSuhallaGateEvents3[];

#endif
