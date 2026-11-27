/* scLightSprsAdd with the count kept in a plain global. */
void scLightSprsAddCounted(s32 id, s32 frame) {
	ACTOR *actor = fdActorGet(id);

	if(actor != NULL) {
		actor->tmr = frame;
		LightSpriteActors[LightSpriteCount] = actor;
		LightSpriteCount++;
	}
}
