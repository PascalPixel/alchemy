#ifndef ALCHEMY_UNIT_SCENE_SUHALLA_H
#define ALCHEMY_UNIT_SCENE_SUHALLA_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
s32 SceneData_SelectActorTableByFlag96f(void);
s32 SceneData_SelectExtraTableByFlag96f(void);
s32 SceneState_InitEntryWorkspaceAndFlag96f(void);
extern struct SceneEntrance gSuhalla_Scripts[];
extern u32 gSuhalla_Messages[];
extern struct ScenePlacement gSuhalla_Actors[];
extern struct ScenePlacement gSuhalla_ActorsFlag96f[];
extern s16 gSuhalla_CellAnimationOrigins[][2];
extern u16 gSuhalla_CellSteps0[];
extern u16 gSuhalla_CellSteps1[];
extern struct SceneEvent gSuhalla_Extras[];
extern struct SceneEvent gSuhalla_ExtrasFlag96f[];

#endif
