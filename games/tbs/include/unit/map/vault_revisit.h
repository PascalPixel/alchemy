#ifndef ALCHEMY_UNIT_MAP_VAULT_REVISIT_H
#define ALCHEMY_UNIT_MAP_VAULT_REVISIT_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern s32 gVaultRevisit_ShortEffectScript[];
extern s32 gVaultRevisit_MediumEffectScript[];
extern s32 gVaultRevisit_LongEffectScript[];
extern const s32 *gEffectScripts[];
extern s32 gVaultRevisit_BoyRoundTheWellScript[];
extern s32 gVaultRevisit_GirlRoundTheWellScript[];
extern struct MapWarpDest gVaultRevisit_WarpDests[];
extern u32 gVaultRevisit_Warps[];
extern struct ObjectEventTemplate gVaultRevisit_ObjectEventTemplates[];
extern u16 gVaultRevisit_GemOwnersHouseDoorCells[];
extern u16 gVaultRevisit_ChildsHouseDoorCells[];
extern u16 gVaultRevisit_InnDoorCells[];
extern u16 gVaultRevisit_ItemShopDoorCells[];
extern u16 gVaultRevisit_WeaponAndArmorShopDoorCells[];
extern u16 gVaultRevisit_MayorsHouseDoorCells[];
extern u16 gVaultRevisit_JailDoorCells[];
extern struct MapEvent gVaultRevisit_Events[];

#endif
