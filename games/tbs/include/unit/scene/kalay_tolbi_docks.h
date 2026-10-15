#ifndef ALCHEMY_UNIT_SCENE_KALAY_TOLBI_DOCKS_H
#define ALCHEMY_UNIT_SCENE_KALAY_TOLBI_DOCKS_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *KalayTolbiDocks_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern u16 KalayTolbiDocks_LeaveCells1[];
extern u16 KalayTolbiDocks_LeaveCells3[];
extern s32 KalayTolbiDocks_ActorSixteenScript[];
extern struct SceneEntrance gKalayTolbiDocksEntrancesOther[];
extern struct SceneEntrance gKalayTolbiDocksEntrances1[];
extern struct SceneEntrance gKalayTolbiDocksEntrances3[];
extern struct SceneEntrance gKalayTolbiDocksEntrances2[];
extern u32 KalayTolbiDocks_SceneTable[];
extern struct ScenePlacement gKalayTolbiDocksPlacementsOther[];
extern struct ScenePlacement gKalayTolbiDocksPlacements3[];
extern struct ScenePlacement gKalayTolbiDocksPlacements3Flag950[];
extern struct ScenePlacement gKalayTolbiDocksPlacements1[];
extern struct ScenePlacement gKalayTolbiDocksPlacements1Flag93e[];
extern struct ScenePlacement gKalayTolbiDocksPlacements2[];
extern struct ScenePlacement gKalayTolbiDocksPlacements2Flag93e[];
extern struct ScenePlacement gKalayTolbiDocksPlacements2Flag950[];
extern struct SceneEvent gKalayTolbiDocksEventsOther[];
extern struct SceneEvent gKalayTolbiDocksEvents1[];
extern struct SceneEvent gKalayTolbiDocksEvents1Flag93e[];
extern struct SceneEvent gKalayTolbiDocksEvents3[];
extern struct SceneEvent gKalayTolbiDocksEvents3Flag950[];
extern struct SceneEvent gKalayTolbiDocksEvents2[];
extern struct SceneEvent gKalayTolbiDocksEvents2Flag93e[];
extern struct SceneEvent gKalayTolbiDocksEvents2Flag950[];

#endif
