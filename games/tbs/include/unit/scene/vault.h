#ifndef ALCHEMY_UNIT_SCENE_VAULT_H
#define ALCHEMY_UNIT_SCENE_VAULT_H

#include "types.h"
#include "field_scene.h"

void *SceneData_GetScriptTable(void);
int SceneData_ReturnZero(void);
void *SceneData_GetMessageTable(void);
void *SceneData_SelectActorTableByFlag855(void);
void *Vault_SelectEvents(void);
s32 Vault_RestoreEntryState(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 Vault_Actor2[];
extern s32 Vault_Actor0[];
extern s32 Vault_Actor1[];
extern struct SceneEntrance Vault_Scripts[];
extern u32 Vault_Messages[];
extern struct ScenePlacement Vault_Actors[];
extern struct ScenePlacement Vault_ActorsFlag855[];
extern struct SceneEvent Vault_Extras[];
extern struct SceneEvent Vault_ExtrasFlag855[];
extern u16 Vault_Scene5Cells[];
extern u16 Vault_Scene6Cells[];
extern u16 Vault_Scene7Cells[];
extern u16 Vault_Scene8Cells[];
extern u16 Vault_Scene9Cells[];
extern u16 Vault_Scene10Cells[];
extern u16 Vault_Scene12Cells[];
extern s32 Vault_DriftScriptA[];
extern s32 Vault_DriftScriptB[];

#endif
