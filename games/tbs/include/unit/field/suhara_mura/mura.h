#ifndef ALCHEMY_UNIT_FIELD_SUHARA_MURA_MURA_H
#define ALCHEMY_UNIT_FIELD_SUHARA_MURA_MURA_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
s32 SceneData_SelectActorTableByFlag96f(void);
s32 SceneData_SelectExtraTableByFlag96f(void);
s32 SceneState_InitEntryWorkspaceAndFlag96f(void);
extern struct SceneEntrance SuharaMura_Scripts[];
extern u32 SuharaMura_Messages[];
extern struct ScenePlacement SuharaMura_Actors[];
extern struct ScenePlacement SuharaMura_ActorsFlag96f[];
extern s16 SuharaMura_CellAnimationOrigins[][2];
extern u16 SuharaMura_CellSteps0[];
extern u16 SuharaMura_CellSteps1[];
extern struct SceneEvent SuharaMura_Extras[];
extern struct SceneEvent SuharaMura_ExtrasFlag96f[];

#endif
