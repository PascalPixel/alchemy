/* @file */
/* Vault, on both visits: Isaac walking in through a house's open door. */

/* Walks Isaac slowly into the doorway at (x, z) as the screen changes to the */
/* room behind it. */
static void scVaultWalkThroughExit(s32 x, s32 z, s32 exitNumber) {
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorWalkTo(ACTOR_PARTY_LEADER, x, z);
	FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;
	syImportEventSetExitRequest(exitNumber);
}
