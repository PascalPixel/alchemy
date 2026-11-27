/* @file */
/* Vault, before and after the thieves are caught: the Sanctum's double door */
/* in the cliff in the west of town, which opens in two frames as the leader */
/* walks into it facing north. Both */
/* scenes give that exit one number, so the shared code names it */
/* VAULT_TO_SANCTUM. */

STATIC_ASSERT(VAULT_TO_SANCTUM == VAULT_REVISIT_TO_SANCTUM);

/* The door's half-open and open pictures among the map's cells, by column, */
/* each half two cells square. */
enum {
	VAULT_DOOR_OPENING_COLUMN = 42,
	VAULT_DOOR_OPEN_COLUMN = 40,
	VAULT_DOOR_HALF_SIZE = 2
};

/* The two square halves are stacked in the source layout and drawn side by */
/* side. */
static inline void scVaultDrawSanctumDoor(s32 sourceColumn) {
	fdMapGridCopyCellsTo(sourceColumn, 33, 34, 16, VAULT_DOOR_HALF_SIZE,
						 VAULT_DOOR_HALF_SIZE);
	fdMapGridCopyCellsTo(sourceColumn, 35, 36, 16, VAULT_DOOR_HALF_SIZE,
						 VAULT_DOOR_HALF_SIZE);
}

/* Isaac walks up to the Sanctum's wooden double door, in the cliff in the west
 */
/* of Vault (cells (3, 18) and (4, 18)). It thumps open and he goes into the */
/* Sanctum, vanishing into the doorway as he walks in. */
static void scVaultOpenDoorwayAndEnter(void) {
	ACTOR *actor = fdActorGetById(ACTOR_PARTY_LEADER);
	SPR *sprite = actor->spr;

	sySndPlay(SE_THUMP);
	scVaultDrawSanctumDoor(VAULT_DOOR_OPENING_COLUMN);
	fdEventWait(4);
	scVaultDrawSanctumDoor(VAULT_DOOR_OPEN_COLUMN);
	fdEventWait(4);
	fdMapGridCopyCellAttributes(33, 21, 2, 2, 3, 16);
	actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	sprite->image.pri = 3;
	scVaultWalkThroughExit(64, 272, VAULT_TO_SANCTUM);
}
