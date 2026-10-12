#ifndef ALCHEMY_UNIT_SCENE_RARIBERO_MACHI_H
#define ALCHEMY_UNIT_SCENE_RARIBERO_MACHI_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
s32 SceneData_SelectTableByFlag9a7(void);
s32 SceneData_SelectSecondaryTableByFlag9a7(void);
s32 Scene_Initialize(void);
extern s32 RariberoMachi_LamentActions[];
extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement Placement_Actors[];
extern struct ScenePlacement Placement_Actors9a7[];
extern s16 RariberoMachi_DoorCells[][2];
extern u16 RariberoMachi_DoorOpenSteps[];
extern u16 RariberoMachi_GateOpenSteps[];
extern struct SceneEvent Placement_Effects[];
extern struct SceneEvent Placement_Effects9a7[];

#endif
