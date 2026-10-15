#ifndef ALCHEMY_UNIT_SCENE_MCCOY_PALACE_GARDEN_H
#define ALCHEMY_UNIT_SCENE_MCCOY_PALACE_GARDEN_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

u8                          *SceneData_GetScriptTable(void);
s32                          SceneData_ReturnZero(void);
u8                          *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          Scene_Initialize(void);
extern u16                   gGarden_GateCells[];
extern s32                   gMcCoyPalaceGarden_GuardScript[];
extern struct SceneEntrance  gPlacement_Scripts[];
extern u32                   gPlacement_Messages[];
extern struct ScenePlacement gMcCoyPalaceGardenPlacementsOther[];
extern struct ScenePlacement gMcCoyPalaceGardenPlacements[];
extern struct SceneEvent     gMcCoyPalaceGardenEventsOther[];
extern struct SceneEvent     gMcCoyPalaceGardenEvents[];

#endif
