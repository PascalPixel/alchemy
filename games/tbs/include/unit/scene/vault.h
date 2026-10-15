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
extern s32 gVault_Actor2[];
extern s32 gVault_Actor0[];
extern s32 gVault_Actor1[];
extern struct SceneEntrance gVault_Scripts[];
extern u32 gVault_Messages[];
extern struct ScenePlacement gVault_Actors[];
extern struct ScenePlacement gVault_ActorsFlag855[];
extern struct SceneEvent gVault_Extras[];
extern struct SceneEvent gVault_ExtrasFlag855[];
extern u16 gVault_Scene5Cells[];
extern u16 gVault_Scene6Cells[];
extern u16 gVault_Scene7Cells[];
extern u16 gVault_Scene8Cells[];
extern u16 gVault_Scene9Cells[];
extern u16 gVault_Scene10Cells[];
extern u16 gVault_Scene12Cells[];
extern s32 gVault_DriftScriptA[];
extern s32 gVault_DriftScriptB[];

#endif
