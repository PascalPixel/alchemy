#ifndef ALCHEMY_UNIT_MAP_VAULT_REVISIT_H
#define ALCHEMY_UNIT_MAP_VAULT_REVISIT_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gVaultRevisit_ShortEffectScript[];
extern s32 gVaultRevisit_MediumEffectScript[];
extern s32 gVaultRevisit_LongEffectScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gVaultRevisit_BoyRoundTheWellScript[];
extern s32 gVaultRevisit_GirlRoundTheWellScript[];
extern struct SceneEntrance gVaultRevisit_Entrances[];
extern u32 gVaultRevisit_Exits[];
extern struct ActorTemplate gVaultRevisit_ActorTemplates[];
extern u16 gVaultRevisit_GemOwnersHouseDoorCells[];
extern u16 gVaultRevisit_ChildsHouseDoorCells[];
extern u16 gVaultRevisit_InnDoorCells[];
extern u16 gVaultRevisit_ItemShopDoorCells[];
extern u16 gVaultRevisit_WeaponAndArmorShopDoorCells[];
extern u16 gVaultRevisit_MayorsHouseDoorCells[];
extern u16 gVaultRevisit_JailDoorCells[];
extern struct Trigger gVaultRevisit_Triggers[];

#endif
