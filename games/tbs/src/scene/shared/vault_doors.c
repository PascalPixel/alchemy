/* Vault, before and after the thieves are caught: Isaac opening a door of the
   town and going in. The scene names the room behind each door, which is a room
   of Log Rooms A before and of Log Rooms B after: VAULT_GEM_OWNERS_HOUSE_EXIT,
   VAULT_CHILDS_HOUSE_EXIT, VAULT_INN_EXIT, VAULT_ITEM_SHOP_EXIT,
   VAULT_WEAPON_AND_ARMOR_SHOP_EXIT and VAULT_MAYORS_HOUSE_EXIT. */

/* Isaac opens the door of the house in the east, by the rope bridge (metatile
   (25, 21)), the gem owner's, and goes in. */
static void scVaultEnterGemOwnersHouse(void)
{
	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(VaultGemOwnersHouseDoorMetatiles, 56, 19);
	scVaultWalkThroughExit(408, 320, VAULT_GEM_OWNERS_HOUSE_EXIT);
}

/* Isaac opens the door of the house east of the inn (metatile (19, 20)), a
   child's home, and goes in. */
static void scVaultEnterChildsHouse(void)
{
	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(VaultChildsHouseDoorMetatiles, 50, 18);
	scVaultWalkThroughExit(312, 304, VAULT_CHILDS_HOUSE_EXIT);
}

/* Isaac opens the inn's door, in the middle of town (metatile (13, 19)), and
   goes in. */
static void scVaultEnterInn(void)
{
	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(VaultInnDoorMetatiles, 44, 17);
	scVaultWalkThroughExit(216, 288, VAULT_INN_EXIT);
}

/* Isaac opens the item shop's door, under the jar sign in the cliff in the east
   of town (metatile (23, 15)), and goes in. The shop is an old couple's home
   too.

   He vanishes into the doorway as he walks in: the leader is drawn behind the
   cliff from the door's first step. */
static void scVaultEnterItemShop(void)
{
	ACTOR *actor = fdActorGet(ACTOR_PARTY_LEADER);
	SPR	  *sprite = actor->sprite;

	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(VaultItemShopDoorMetatiles, 54, 13);
	fdMapGridCopyMetatileAttributes(33, 20, 1, 3, 23, 12);
	actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	sprite->image.priority = 3;
	scVaultWalkThroughExit(376, 224, VAULT_ITEM_SHOP_EXIT);
}

/* Isaac opens the weapon and armor shop's door, under the sword sign in the
   cliff below the bell tower (metatile (18, 12)), and goes in, as at the item
   shop. */
static void scVaultEnterWeaponAndArmorShop(void)
{
	ACTOR *actor = fdActorGet(ACTOR_PARTY_LEADER);
	SPR	  *sprite = actor->sprite;

	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(VaultWeaponAndArmorShopDoorMetatiles, 49, 10);
	fdMapGridCopyMetatileAttributes(33, 20, 1, 3, 18, 10);
	actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	sprite->image.priority = 3;
	scVaultWalkThroughExit(296, 176, VAULT_WEAPON_AND_ARMOR_SHOP_EXIT);
}

/* Isaac opens the door of the mayor's house, the big house in the town's
   north-west (metatile (7, 10)), and goes in. Before the thieves are caught,
   Ivan waits there before he joins. */
static void scVaultEnterMayorsHouse(void)
{
	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(VaultMayorsHouseDoorMetatiles, 38, 6);
	scVaultWalkThroughExit(120, 144, VAULT_MAYORS_HOUSE_EXIT);
}
