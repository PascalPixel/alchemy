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
extern u32 VaultInteriors_Stops[];
extern s32 VaultInteriors_ActionTable[];
extern s32 VaultInteriors_Villager11ExitScript[];
extern s32 VaultInteriors_Villager12ExitScript[];
extern s32 VaultInteriors_VaultScriptA[];
extern s32 VaultInteriors_VaultScriptB[];
extern s32 VaultInteriors_VaultScriptC[];
extern s32 VaultInteriors_VaultScriptD[];
extern s32 VaultInteriors_VaultScriptE[];
extern s32 VaultInteriors_PairScriptA[];
extern s32 VaultInteriors_PairScriptB[];
extern s32 VaultInteriors_PairScriptC[];
extern s32 VaultInteriors_PairScriptD[];
extern s32 VaultInteriors_PairScriptE[];
extern s32 VaultInteriors_PairScriptF[];
extern s32 VaultInteriors_PairScriptG[];
extern s32 VaultInteriors_PairScriptH[];
extern s32 VaultInteriors_PairScriptI[];
extern s32 VaultInteriors_PairScriptJ[];
extern s32 VaultInteriors_PairScriptK[];
extern s32 VaultInteriors_PairScriptL[];
extern s32 VaultInteriors_PairScriptM[];
extern s32 VaultInteriors_PairScriptN[];
extern s32 VaultInteriors_PairScriptO[];
extern s32 VaultInteriors_PairScriptP[];
extern s32 VaultInteriors_PairScriptQ[];
extern s32 VaultInteriors_PairScriptR[];
extern struct SceneEntrance VaultInteriors_Scripts[];
extern s16 VaultInteriors_Regions[];
extern u32 VaultInteriors_Messages[];
extern struct ScenePlacement VaultInteriors_SceneTableA[];
extern struct ScenePlacement VaultInteriors_SceneTableB[];
extern struct SceneEvent gVaultInteriorsEvents[];
extern struct SceneEvent gVaultInteriorsEventsEntrances15To17[];
extern struct SceneEvent gVaultInteriorsEventsFlag855[];
extern s32 *VaultInteriors_StepActions[];
extern s32 *VaultInteriors_IdleActions[];
extern s32 *VaultInteriors_ResponseScripts[];

#endif
