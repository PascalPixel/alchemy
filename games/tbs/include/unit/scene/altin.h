#ifndef ALCHEMY_UNIT_SCENE_ALTIN_H
#define ALCHEMY_UNIT_SCENE_ALTIN_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance  *Scene_GetEntrances(void);
s32                          SceneData_ReturnZero(void);
u8                          *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          Scene_Initialize(void);
extern s32                   gAltinPlacements2_Data[];
extern struct SceneEntrance  gAltinEntrancesOther[];
extern struct SceneEntrance  gAltinEntrances1[];
extern struct SceneEntrance  gAltinEntrances2[];
extern u32                   gPlacement_Messages[];
extern struct ScenePlacement gAltinPlacementsOther[];
extern struct ScenePlacement gAltinPlacements1[];
extern struct ScenePlacement gAltinPlacements2[];
extern struct SceneEvent     gAltinEventsOther[];
extern struct SceneEvent     gAltinEvents1[];
extern struct SceneEvent     gAltinEvents2[];
extern u16                   gAltin_TriggerTable1[];
extern u16                   gAltin_TriggerTable0[];
extern u16                   gAltin_TriggerTable5[];
extern u16                   gAltin_TriggerTable6[];
extern u16                   gAltin_TriggerTable2[];
extern u16                   gAltin_TriggerTable3[];
extern u16                   gAltin_TriggerTable4[];
extern struct MapCellAnim    gAltin_TriggerTable[];

#endif
