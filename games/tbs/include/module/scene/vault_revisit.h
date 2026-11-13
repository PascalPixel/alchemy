#ifndef ALCHEMY_MODULE_SCENE_VAULT_REVISIT_H
#define ALCHEMY_MODULE_SCENE_VAULT_REVISIT_H

#include "types.h"
#include "scene_tables.h"

extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gVault_BoyRoundTheWellRoute[];
extern s32 gVault_GirlRoundTheWellRoute[];
extern struct SceneEntrance gVaultRevisit_Entrances[];
extern u32 gVaultRevisit_Exits[];
extern struct ActorPlacement gVaultRevisit_ActorPlacements[];
extern u16 gVaultRevisit_GemOwnersHouseDoorCells[];
extern u16 gVaultRevisit_ChildsHouseDoorCells[];
extern u16 gVaultRevisit_InnDoorCells[];
extern u16 gVaultRevisit_ItemShopDoorCells[];
extern u16 gVaultRevisit_WeaponAndArmorShopDoorCells[];
extern u16 gVaultRevisit_MayorsHouseDoorCells[];
extern u16 gVaultRevisit_JailDoorCells[];
extern struct Trigger gVaultRevisit_Triggers[];

#endif
