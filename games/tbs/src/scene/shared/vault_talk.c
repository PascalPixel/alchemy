/* Vault and its interiors: a villager's thought read with Mind Read. */

/* Shows the message already set as the actor's thought: the actor stops and
   stands while the thought is shown. */
static void scVaultShowThought(Sint32 actorId)
{
	ACTOR *actor = fdActorGetById(actorId);

	actor->paused = 1;
	fdScriptStart();
	fdActorSetAnimation(actorId, ANIM_STAND);
	fdScriptWait(2);
	fdScriptShowMessageAndWait(actorId, 0);
	fdScriptEnd();
	actor->paused = 0;
}
