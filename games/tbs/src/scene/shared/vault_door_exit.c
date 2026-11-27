/* Vault, on both visits: Isaac walking in through a house's open door. */

/* Walks Isaac slowly into the doorway at (x, z) as the screen changes to the
   room behind it. */
static void scVaultWalkThroughExit(Sint32 x, Sint32 z, Sint32 exitNum)
{
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorWalkTo(ACTOR_PARTY_LEADER, x, z);
	FieldStateWork->transitionFrm = MAP_TRANSITION_TICKS;
	syImportScriptSetExitRequest(exitNum);
}
