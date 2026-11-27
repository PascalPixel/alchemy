/* @file */
/* Vault and its interiors: a villager's thought read with Mind Read. */

/* Shows the message already set as the actor's thought: the */
/* actor stops and stands while the thought is shown. */
static void scVaultShowThought(s32 actorId) {
	ACTOR *actor = fdActorGetById(actorId);

	actor->paused = 1;
	fdEventStart();
	fdActorSetAnimation(actorId, ANIM_STAND);
	fdEventWait(2);
	fdEventShowMessageAndWait(actorId, 0);
	fdEventEnd();
	actor->paused = 0;
}
