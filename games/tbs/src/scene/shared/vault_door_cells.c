/* @file */
/* Vault's doors opening, on both visits (one map): two frames of map cells */
/* each, named for the room the door opens into (scVaultEnterGemOwnersHouse and
 */
/* the rest). */

/* Each door's opening, by the room it opens into. */
u16 Vault_GemOwnersHouseDoorCells[] = {
	MAPGRID_ANIMATION_STEP(35, 52, 2, 2, 4),
	MAPGRID_ANIMATION_STEP(33, 52, 2, 2, 4),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Vault_ChildsHouseDoorCells[] = {
	MAPGRID_ANIMATION_STEP(35, 50, 2, 2, 4),
	MAPGRID_ANIMATION_STEP(33, 50, 2, 2, 4),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Vault_InnDoorCells[] = {
	MAPGRID_ANIMATION_STEP(35, 48, 2, 2, 4),
	MAPGRID_ANIMATION_STEP(33, 48, 2, 2, 4),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Vault_ItemShopDoorCells[] = {
	MAPGRID_ANIMATION_STEP(42, 48, 2, 2, 4),
	MAPGRID_ANIMATION_STEP(40, 48, 2, 2, 4),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Vault_WeaponAndArmorShopDoorCells[] = {
	MAPGRID_ANIMATION_STEP(42, 46, 2, 2, 4),
	MAPGRID_ANIMATION_STEP(40, 46, 2, 2, 4),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Vault_MayorsHouseDoorCells[] = {
	MAPGRID_ANIMATION_STEP(35, 46, 2, 2, 4),
	MAPGRID_ANIMATION_STEP(33, 46, 2, 2, 4),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Vault_JailDoorCells[] = {
	MAPGRID_ANIMATION_STEP(35, 33, 2, 2, 4),
	MAPGRID_ANIMATION_STEP(33, 33, 2, 2, 4),
	MAPGRID_ANIMATION_STEPS_END,
};
