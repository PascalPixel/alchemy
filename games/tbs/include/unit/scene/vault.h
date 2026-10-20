#ifndef ALCHEMY_UNIT_SCENE_VAULT_H
#define ALCHEMY_UNIT_SCENE_VAULT_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gVault_Actor2[];
extern s32 gVault_Actor0[];
extern s32 gVault_Actor1[];
extern struct SceneEntrance gVault_Entrances[];
extern u32 gVault_Exits[];
extern struct ScenePlacement gVault_Placements[];
extern struct ScenePlacement gVault_PlacementsAfterThieves[];
extern struct SceneEvent gVault_Events[];
extern struct SceneEvent gVault_EventsAfterThieves[];
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
