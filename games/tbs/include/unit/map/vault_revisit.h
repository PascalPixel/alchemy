#ifndef ALCHEMY_UNIT_SCENE_VAULT_REVISIT_H
#define ALCHEMY_UNIT_SCENE_VAULT_REVISIT_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gVaultRevisit_ShortEffectScript[];
extern s32 gVaultRevisit_MediumEffectScript[];
extern s32 gVaultRevisit_LongEffectScript[];
extern const s32 *gEffectScripts[];
extern s32 gVaultRevisit_BoyRoundTheWellScript[];
extern s32 gVaultRevisit_GirlRoundTheWellScript[];
extern struct SceneEntrance gVaultRevisit_Entrances[];
extern u32 gVaultRevisit_Exits[];
extern struct ScenePlacement gVaultRevisit_Placements[];
extern u16 gVaultRevisit_GemOwnersHouseDoorCells[];
extern u16 gVaultRevisit_ChildsHouseDoorCells[];
extern u16 gVaultRevisit_InnDoorCells[];
extern u16 gVaultRevisit_ItemShopDoorCells[];
extern u16 gVaultRevisit_WeaponAndArmorShopDoorCells[];
extern u16 gVaultRevisit_MayorsHouseDoorCells[];
extern u16 gVaultRevisit_JailDoorCells[];
extern struct SceneEvent gVaultRevisit_Events[];

#endif
