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
extern struct SceneEntrance Suhalla_Scripts[];
extern u32 Suhalla_Messages[];
extern struct ScenePlacement Suhalla_Actors[];
extern struct ScenePlacement Suhalla_ActorsFlag96f[];
extern s16 Suhalla_CellAnimationOrigins[][2];
extern u16 Suhalla_CellSteps0[];
extern u16 Suhalla_CellSteps1[];
extern struct SceneEvent Suhalla_Extras[];
extern struct SceneEvent Suhalla_ExtrasFlag96f[];

#endif
