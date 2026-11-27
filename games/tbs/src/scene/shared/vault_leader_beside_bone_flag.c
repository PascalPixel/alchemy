/* Vault's rooms, before and after the thieves are caught: whether the leader
   stands beside the Bone on the floor, which the scenes' cue tables test
   through FLAG_VAULT_LEADER_BESIDE_BONE. */

/* The leader stands beside the Bone on the inn's barrel, so the girl at the
   inn's talk row gives way and the A button searches the Bone. */
#define FLAG_VAULT_LEADER_BESIDE_BONE FLAG_SCENE_TEMP_50

/* An event: sets FLAG_VAULT_LEADER_BESIDE_BONE while the leader stands in
   metatiles (34, 41) to (35, 42), beside the barrel, and clears it anywhere
   else. */
static void scVaultUpdateLeaderBesideBoneFlag(void)
{
	ACTOR *leader = fdActorGetById(ACTOR_PARTY_LEADER);
	FIXED  fixedX = leader->x.fixed;
	Sint32 column;
	Sint32 row;

	row = fdActorGetById(ACTOR_PARTY_LEADER)->z.fixed;
	column = FIELD_METATILE(fixedX);
	column -= 34;
	row = FIELD_METATILE(row);

	if(column >= 0 && column <= 1 && row > 40 && row <= 42) {
		gmFlagSet(FLAG_VAULT_LEADER_BESIDE_BONE);
	} else {
		gmFlagClear(FLAG_VAULT_LEADER_BESIDE_BONE);
	}
}
