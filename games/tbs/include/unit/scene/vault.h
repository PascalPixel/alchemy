#ifndef ALCHEMY_UNIT_SCENE_VAULT_H
#define ALCHEMY_UNIT_SCENE_VAULT_H

#include "types.h"
#include "field_scene.h"

void *SceneData_GetScriptTable(void);
int SceneData_ReturnZero(void);
void *SceneData_GetMessageTable(void);
void *SceneData_SelectActorTableByFlag855(void);
void *KuupuappuMura_SelectEvents(void);
s32 KuupuappuMura_RestoreEntryState(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 KuupuappuMura_Actor2[];
extern s32 KuupuappuMura_Actor0[];
extern s32 KuupuappuMura_Actor1[];
extern struct SceneEntrance KuupuappuMura_Scripts[];
extern u32 KuupuappuMura_Messages[];
extern struct ScenePlacement KuupuappuMura_Actors[];
extern struct ScenePlacement KuupuappuMura_ActorsFlag855[];
extern struct SceneEvent KuupuappuMura_Extras[];
extern struct SceneEvent KuupuappuMura_ExtrasFlag855[];
extern u16 KuupuappuMura_Scene5Cells[];
extern u16 KuupuappuMura_Scene6Cells[];
extern u16 KuupuappuMura_Scene7Cells[];
extern u16 KuupuappuMura_Scene8Cells[];
extern u16 KuupuappuMura_Scene9Cells[];
extern u16 KuupuappuMura_Scene10Cells[];
extern u16 KuupuappuMura_Scene12Cells[];
extern s32 KuupuappuMura_DriftScriptA[];
extern s32 KuupuappuMura_DriftScriptB[];

#endif
