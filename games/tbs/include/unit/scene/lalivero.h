#ifndef ALCHEMY_UNIT_SCENE_LALIVERO_H
#define ALCHEMY_UNIT_SCENE_LALIVERO_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
s32 SceneData_SelectTableByFlag9a7(void);
s32 SceneData_SelectSecondaryTableByFlag9a7(void);
s32 Scene_Initialize(void);
extern s32 gLalivero_LamentActions[];
extern struct SceneEntrance gPlacement_Scripts[];
extern u32 gPlacement_Messages[];
extern struct ScenePlacement gPlacement_Actors[];
extern struct ScenePlacement gPlacement_Actors9a7[];
extern s16 gLalivero_DoorCells[][2];
extern u16 gLalivero_DoorOpenSteps[];
extern u16 gLalivero_GateOpenSteps[];
extern struct SceneEvent gPlacement_Effects[];
extern struct SceneEvent gPlacement_Effects9a7[];

#endif
