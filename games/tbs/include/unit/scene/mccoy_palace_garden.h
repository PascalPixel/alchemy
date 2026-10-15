#ifndef ALCHEMY_UNIT_SCENE_MCCOY_PALACE_GARDEN_H
#define ALCHEMY_UNIT_SCENE_MCCOY_PALACE_GARDEN_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern u16 Niwa_GateCells[];
extern s32 BiribinoNiwa_GuardScript[];
extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement gBiribinoNiwaPlacementsOther[];
extern struct ScenePlacement gBiribinoNiwaPlacements[];
extern struct SceneEvent gBiribinoNiwaEventsOther[];
extern struct SceneEvent gBiribinoNiwaEvents[];

#endif
