#ifndef ALCHEMY_UNIT_SCENE_VAULT_REVISIT_H
#define ALCHEMY_UNIT_SCENE_VAULT_REVISIT_H

#include "types.h"
#include "field_scene.h"

const void *SceneData_GetScriptTable(void);
int SceneData_ReturnZero(void);
const void *SceneData_GetMessageTable(void);
const void *SceneData_GetActorTable(void);
const u8 *SceneData_GetExtraTable(void);
s32 SceneSetup_InitializeActorsAndFlags(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 VaultRevisit_Actor0[];
extern s32 VaultRevisit_Actor1[];
extern struct SceneEntrance VaultRevisit_Scripts[];
extern u32 VaultRevisit_Messages[];
extern struct ScenePlacement VaultRevisit_Actors[];
extern u16 VaultRevisit_Scene5Cells[];
extern u16 VaultRevisit_Scene6Cells[];
extern u16 VaultRevisit_Scene7Cells[];
extern u16 VaultRevisit_Scene8Cells[];
extern u16 VaultRevisit_Scene9Cells[];
extern u16 VaultRevisit_Scene10Cells[];
extern u16 VaultRevisit_Scene12Cells[];
extern struct SceneEvent VaultRevisit_Extras[];

#endif
