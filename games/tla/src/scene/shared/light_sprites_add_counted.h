/* scLightSprsAdd with the count kept in a plain global. */
void scLightSprsAddCounted(Sint32 id, Sint32 frame)
{
	ACTOR *actor = fdActorGet(id);

	if(actor != NULL) {
		actor->tmr = frame;
		LightSpriteActors[LightSpriteCount] = actor;
		LightSpriteCount++;
	}
}
