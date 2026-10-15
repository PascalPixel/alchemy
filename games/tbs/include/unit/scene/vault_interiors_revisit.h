#ifndef ALCHEMY_UNIT_SCENE_VAULT_INTERIORS_REVISIT_H
#define ALCHEMY_UNIT_SCENE_VAULT_INTERIORS_REVISIT_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetPreparedActors(void);
u8 *SceneData_GetEffectTable(void);
s32 FieldScene_InitSceneStateByStep(void);
extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement Placement_Actors[];
extern struct SceneEvent Placement_Effects[];

#endif
