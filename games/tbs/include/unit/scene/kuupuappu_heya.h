#ifndef ALCHEMY_UNIT_SCENE_KUUPUAPPU_HEYA_H
#define ALCHEMY_UNIT_SCENE_KUUPUAPPU_HEYA_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

u8 *SceneData_GetScriptTable(void);
u8 *SceneData_GetRegionTable(void);
u8 *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 KuupuappuHeya_ApplyEntryState(void);
extern u32 KuupuappuHeya_Stops[];
extern s32 KuupuappuHeya_ActionTable[];
extern s32 KuupuappuHeya_Villager11ExitScript[];
extern s32 KuupuappuHeya_Villager12ExitScript[];
extern s32 KuupuappuHeya_VaultScriptA[];
extern s32 KuupuappuHeya_VaultScriptB[];
extern s32 KuupuappuHeya_VaultScriptC[];
extern s32 KuupuappuHeya_VaultScriptD[];
extern s32 KuupuappuHeya_VaultScriptE[];
extern s32 KuupuappuHeya_PairScriptA[];
extern s32 KuupuappuHeya_PairScriptB[];
extern s32 KuupuappuHeya_PairScriptC[];
extern s32 KuupuappuHeya_PairScriptD[];
extern s32 KuupuappuHeya_PairScriptE[];
extern s32 KuupuappuHeya_PairScriptF[];
extern s32 KuupuappuHeya_PairScriptG[];
extern s32 KuupuappuHeya_PairScriptH[];
extern s32 KuupuappuHeya_PairScriptI[];
extern s32 KuupuappuHeya_PairScriptJ[];
extern s32 KuupuappuHeya_PairScriptK[];
extern s32 KuupuappuHeya_PairScriptL[];
extern s32 KuupuappuHeya_PairScriptM[];
extern s32 KuupuappuHeya_PairScriptN[];
extern s32 KuupuappuHeya_PairScriptO[];
extern s32 KuupuappuHeya_PairScriptP[];
extern s32 KuupuappuHeya_PairScriptQ[];
extern s32 KuupuappuHeya_PairScriptR[];
extern struct SceneEntrance KuupuappuHeya_Scripts[];
extern s16 KuupuappuHeya_Regions[];
extern u32 KuupuappuHeya_Messages[];
extern struct ScenePlacement KuupuappuHeya_SceneTableA[];
extern struct ScenePlacement KuupuappuHeya_SceneTableB[];
extern struct SceneEvent gKuupuappuHeyaEvents[];
extern struct SceneEvent gKuupuappuHeyaEventsEntrances15To17[];
extern struct SceneEvent gKuupuappuHeyaEventsFlag855[];
extern s32 *KuupuappuHeya_StepActions[];
extern s32 *KuupuappuHeya_IdleActions[];
extern s32 *KuupuappuHeya_ResponseScripts[];

#endif
