#ifndef ALCHEMY_UNIT_FIELD_KUUPUAPPU_MURA_SAI_SAI_H
#define ALCHEMY_UNIT_FIELD_KUUPUAPPU_MURA_SAI_SAI_H

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
extern s32 KuupuappuMuraSai_Actor0[];
extern s32 KuupuappuMuraSai_Actor1[];
extern struct SceneEntrance KuupuappuMuraSai_Scripts[];
extern u32 KuupuappuMuraSai_Messages[];
extern struct ScenePlacement KuupuappuMuraSai_Actors[];
extern u16 KuupuappuMuraSai_Scene5Cells[];
extern u16 KuupuappuMuraSai_Scene6Cells[];
extern u16 KuupuappuMuraSai_Scene7Cells[];
extern u16 KuupuappuMuraSai_Scene8Cells[];
extern u16 KuupuappuMuraSai_Scene9Cells[];
extern u16 KuupuappuMuraSai_Scene10Cells[];
extern u16 KuupuappuMuraSai_Scene12Cells[];
extern struct SceneEvent KuupuappuMuraSai_Extras[];

#endif
