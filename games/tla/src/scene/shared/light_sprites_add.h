/* Adds an actor to the glow list, its glow showing `frame`. */
void scLightSprsAdd(Sint32 id, Sint32 frame)
{
	ACTOR *actor = fdActorGet(id);

	if(actor != NULL) {
		Sint16 *count;

		actor->tmr = frame;
		count = &LightSpriteCount;
		LightSpriteActors[*count] = actor;
		(*count)++;
	}
}
