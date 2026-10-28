#ifndef ALCHEMY_UNIT_MAP_VAULT_H
#define ALCHEMY_UNIT_MAP_VAULT_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gVault_ShortEffectScript[];
extern s32 gVault_MediumEffectScript[];
extern s32 gVault_LongEffectScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gVault_WomanWantingTheRoofFixedRoute[];
extern s32 gVault_BoyRoundTheWellRoute[];
extern s32 gVault_GirlRoundTheWellRoute[];
extern struct SceneEntrance gVault_Entrances[];
extern u32 gVault_Exits[];
extern struct ActorPlacement gVault_ActorPlacements[];
extern struct ActorPlacement gVault_ActorPlacementsAfterThieves[];
extern struct Trigger gVault_Triggers[];
extern struct Trigger gVault_TriggersAfterThieves[];
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
