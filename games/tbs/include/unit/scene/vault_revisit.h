#ifndef ALCHEMY_UNIT_SCENE_VAULT_REVISIT_H
#define ALCHEMY_UNIT_SCENE_VAULT_REVISIT_H

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
extern s32 gVaultRevisit_Actor0[];
extern s32 gVaultRevisit_Actor1[];
extern struct SceneEntrance gVaultRevisit_Entrances[];
extern u32 gVaultRevisit_Exits[];
extern struct ScenePlacement gVaultRevisit_Placements[];
extern u16 gVaultRevisit_Scene5Cells[];
extern u16 gVaultRevisit_Scene6Cells[];
extern u16 gVaultRevisit_Scene7Cells[];
extern u16 gVaultRevisit_Scene8Cells[];
extern u16 gVaultRevisit_Scene9Cells[];
extern u16 gVaultRevisit_Scene10Cells[];
extern u16 gVaultRevisit_Scene12Cells[];
extern struct SceneEvent gVaultRevisit_Events[];

#endif
