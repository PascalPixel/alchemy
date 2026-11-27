/* scLightSpritesAdd with the count kept in a plain global. */
void scLightSpritesAddCounted(Sint32 id, Sint32 frame)
{
	ACTOR *actor = fdActorGet(id);

	if(actor != NULL) {
		actor->timer = frame;
		LightSpriteActors[LightSpriteCount] = actor;
		LightSpriteCount++;
	}
}
