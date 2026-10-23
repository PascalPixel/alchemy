#ifndef ALCHEMY_UNIT_SCENE_VAULT_H
#define ALCHEMY_UNIT_SCENE_VAULT_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gVault_ShortEffectScript[];
extern s32 gVault_MediumEffectScript[];
extern s32 gVault_LongEffectScript[];
extern const s32 *gEffectScripts[];
extern s32 gVault_WomanWantingTheRoofFixedRoute[];
extern s32 gVault_BoyRoundTheWellRoute[];
extern s32 gVault_GirlRoundTheWellRoute[];
extern struct SceneEntrance gVault_Entrances[];
extern u32 gVault_Exits[];
extern struct ScenePlacement gVault_Placements[];
extern struct ScenePlacement gVault_PlacementsAfterThieves[];
extern struct SceneEvent gVault_Events[];
extern struct SceneEvent gVault_EventsAfterThieves[];
extern u16 gVault_GemOwnersHouseDoorCells[];
extern u16 gVault_ChildsHouseDoorCells[];
extern u16 gVault_InnDoorCells[];
extern u16 gVault_ItemShopDoorCells[];
extern u16 gVault_WeaponAndArmorShopDoorCells[];
extern u16 gVault_MayorsHouseDoorCells[];
extern u16 gVault_JailDoorCells[];
extern s32 gVault_DriftScriptA[];
extern s32 gVault_DriftScriptB[];

#endif
