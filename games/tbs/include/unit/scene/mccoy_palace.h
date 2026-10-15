#ifndef ALCHEMY_UNIT_SCENE_MCCOY_PALACE_H
#define ALCHEMY_UNIT_SCENE_MCCOY_PALACE_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

u8                          *SceneData_GetScriptTable(void);
s32                          SceneData_ReturnZero(void);
u8                          *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          McCoyPalace_ApplyEntryState(s32 a0, s32 a1);
extern s32                   gPalace_FacingActions[];
extern s32                   gPalace_PacingActions[];
extern struct SceneEntrance  gPlacement_Scripts[];
extern u32                   gPlacement_Messages[];
extern struct ScenePlacement gMcCoyPalacePlacementsOther[];
extern struct ScenePlacement gMcCoyPalacePlacements[];
extern struct SceneEvent     gMcCoyPalaceEventsOther[];
extern struct SceneEvent     gMcCoyPalaceEvents[];

#endif
