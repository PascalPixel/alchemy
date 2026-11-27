#ifndef ALCHEMY_VAULT_H
#define ALCHEMY_VAULT_H

// The metatile behaviors of Vault's layout, which SceneIdVault (vault.c) and SceneIdVaultRevisit
// (vault_revisit.c) both draw and answer.

#include "types.h"

// The metatile behaviors Vault's step and examine cues answer; the exits have behaviors of their
// own.
typedef enum VAULT_METATILE_BEHAVIOR
{
	// Either end of the rope bridge in the town's south-east, where stepping on raises the path
	// that runs under the bridge to the bridge's deck (scVaultRaisePathToBridge).
	METATILE_BEHAVIOR_VAULT_BRIDGE_END                 = 1,
	// The three metatiles west of the bridge's west end, where stepping off lowers the path back
	// under the bridge (scVaultLowerPathUnderBridge).
	METATILE_BEHAVIOR_VAULT_WEST_OF_BRIDGE             = 2,
	// The door of the house where a man keeps a sparkling rock the volcano threw.
	METATILE_BEHAVIOR_VAULT_GEM_OWNERS_HOUSE_DOOR      = 5,
	METATILE_BEHAVIOR_VAULT_CHILDS_HOUSE_DOOR          = 6,
	METATILE_BEHAVIOR_VAULT_INN_DOOR                   = 7,
	METATILE_BEHAVIOR_VAULT_ITEM_SHOP_DOOR             = 8,
	METATILE_BEHAVIOR_VAULT_WEAPON_AND_ARMOR_SHOP_DOOR = 9,
	METATILE_BEHAVIOR_VAULT_MAYORS_HOUSE_DOOR          = 10,
	METATILE_BEHAVIOR_VAULT_SANCTUM_DOORWAY            = 11,
	METATILE_BEHAVIOR_VAULT_DOOR_TO_JAIL               = 12,
	// The top of the ladder against the inn's west wall, which Isaac climbs in through the roof
	// (scVaultClimbIntoInnLoft).
	METATILE_BEHAVIOR_VAULT_INN_LOFT_LADDER            = 13,
	// Before the south gate's exit (VAULT_TO_WORLD_MAP_SOUTH).
	METATILE_BEHAVIOR_VAULT_SOUTH_GATE                 = 20,
	// Before the north gate's exit (VAULT_TO_WORLD_MAP_NORTH).
	METATILE_BEHAVIOR_VAULT_NORTH_GATE                 = 21,
	METATILE_BEHAVIOR_VAULT_BELL                       = 30,
	// The hidden passage down into Vault Cave, which Reveal shows
	// (VAULT_REVISIT_TO_VAULT_CAVE_PASSAGE).
	METATILE_BEHAVIOR_VAULT_HIDDEN_PASSAGE             = 40,
	// The cave mouth in the town's northeast cliff (VAULT_REVISIT_TO_VAULT_CAVE_MOUTH).
	METATILE_BEHAVIOR_VAULT_CLIFF_CAVE_MOUTH           = 41,
	METATILE_BEHAVIOR_VAULT_WELL                       = 90,
	METATILE_BEHAVIOR_VAULT_WOODEN_BOX                 = 101
} VAULT_METATILE_BEHAVIOR;

// Both scenes' tables, defined by the shared parts they include: the children's routes round the
// well (shared/vault_well_routes.c) and the metatiles each door is redrawn with
// (shared/vault_door_metatiles.c).
extern Sint32 VaultBoyRoundTheWellRoute[];
extern Sint32 VaultGirlRoundTheWellRoute[];
extern Uint16 VaultGemOwnersHouseDoorMetatiles[];
extern Uint16 VaultChildsHouseDoorMetatiles[];
extern Uint16 VaultInnDoorMetatiles[];
extern Uint16 VaultItemShopDoorMetatiles[];
extern Uint16 VaultWeaponAndArmorShopDoorMetatiles[];
extern Uint16 VaultMayorsHouseDoorMetatiles[];
extern Uint16 VaultJailDoorMetatiles[];

#endif
