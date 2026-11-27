/* Holds the actor still while the field engine is handling requests. */
void scActorHoldDuringScript(ACTOR *actor)
{
	actor->paused = fdScriptGetFlag(FLAG_FIELD_HANDLING_REQUESTS);
}
