/* scLightSprsAdd with the count kept in a plain global. */
void scLightSprsAddCounted(Sint32 id, Sint32 frm)
{
	ACTOR *actor = fdActorGet(id);

	if(actor != NULL) {
		actor->tmr = frm;
		LightSpriteActors[LightSpriteCount] = actor;
		LightSpriteCount++;
	}
}
