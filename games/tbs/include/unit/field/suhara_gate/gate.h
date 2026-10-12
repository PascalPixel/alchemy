#ifndef ALCHEMY_UNIT_FIELD_SUHARA_GATE_GATE_H
#define ALCHEMY_UNIT_FIELD_SUHARA_GATE_GATE_H

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
s32 SuharaGate_EnterScene(void);
extern s32 SuharaGate_PatrolStartup[];
extern s32 gSuharaGatePlacements1Flagged1[];
extern s32 SuharaGate_FirstGuardStartup[];
extern s32 gSuharaGatePlacements1Flagged0[];
extern struct SceneEntrance gSuharaGateEntrancesOther[];
extern struct SceneEntrance gSuharaGateEntrances2[];
extern struct SceneEntrance gSuharaGateEntrances3[];
extern u32 SuharaGate_SceneTable[];
extern struct ScenePlacement gSuharaGatePlacementsOther[];
extern struct ScenePlacement gSuharaGatePlacements2[];
extern struct ScenePlacement gSuharaGatePlacements1[];
extern struct ScenePlacement gSuharaGatePlacements1Flagged[];
extern struct SceneEvent gSuharaGateEventsOther[];
extern struct SceneEvent gSuharaGateEvents2[];
extern struct SceneEvent gSuharaGateEvents3[];

#endif
