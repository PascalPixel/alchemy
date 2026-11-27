/* Adds an actor to the glow list, its glow showing `frame`. */
void scLightSprsAdd(s32 id, s32 frame) {
	ACTOR *actor = fdActorGet(id);

	if(actor != NULL) {
		s16 *count;

		actor->tmr = frame;
		count = &LightSpriteCount;
		LightSpriteActors[*count] = actor;
		(*count)++;
	}
}
