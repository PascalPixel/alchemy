#ifndef ALCHEMY_UNIT_SCENE_VAULT_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_VAULT_INTERIORS_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

u8 *SceneData_GetScriptTable(void);
u8 *SceneData_GetRegionTable(void);
u8 *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 VaultInteriors_ApplyEntryState(void);
extern u32 gVaultInteriors_Stops[];
extern s32 gVaultInteriors_ActionTable[];
extern s32 gVaultInteriors_Villager11ExitScript[];
extern s32 gVaultInteriors_Villager12ExitScript[];
extern s32 gVaultInteriors_VaultScriptA[];
extern s32 gVaultInteriors_VaultScriptB[];
extern s32 gVaultInteriors_VaultScriptC[];
extern s32 gVaultInteriors_VaultScriptD[];
extern s32 gVaultInteriors_VaultScriptE[];
extern s32 gVaultInteriors_PairScriptA[];
extern s32 gVaultInteriors_PairScriptB[];
extern s32 gVaultInteriors_PairScriptC[];
extern s32 gVaultInteriors_PairScriptD[];
extern s32 gVaultInteriors_PairScriptE[];
extern s32 gVaultInteriors_PairScriptF[];
extern s32 gVaultInteriors_PairScriptG[];
extern s32 gVaultInteriors_PairScriptH[];
extern s32 gVaultInteriors_PairScriptI[];
extern s32 gVaultInteriors_PairScriptJ[];
extern s32 gVaultInteriors_PairScriptK[];
extern s32 gVaultInteriors_PairScriptL[];
extern s32 gVaultInteriors_PairScriptM[];
extern s32 gVaultInteriors_PairScriptN[];
extern s32 gVaultInteriors_PairScriptO[];
extern s32 gVaultInteriors_PairScriptP[];
extern s32 gVaultInteriors_PairScriptQ[];
extern s32 gVaultInteriors_PairScriptR[];
extern struct SceneEntrance gVaultInteriors_Scripts[];
extern s16 gVaultInteriors_Regions[];
extern u32 gVaultInteriors_Messages[];
extern struct ScenePlacement gVaultInteriors_SceneTableA[];
extern struct ScenePlacement gVaultInteriors_SceneTableB[];
extern struct SceneEvent gVaultInteriorsEvents[];
extern struct SceneEvent gVaultInteriorsEventsEntrances15To17[];
extern struct SceneEvent gVaultInteriorsEventsFlag855[];
extern s32 *gVaultInteriors_StepActions[];
extern s32 *gVaultInteriors_IdleActions[];
extern s32 *gVaultInteriors_ResponseScripts[];

#endif
