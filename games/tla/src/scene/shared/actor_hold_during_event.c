/* Holds the actor still while the field engine is handling requests. */
void scActorHoldDuringEvent(ACTOR *actor) {
	actor->paused = fdEventGetFlag(FLAG_FIELD_HANDLING_REQUESTS);
}
