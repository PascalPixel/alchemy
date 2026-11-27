/* Adds an actor to the glow list, its glow showing `frm`. */
void scLightSprsAdd(Sint32 id, Sint32 frm)
{
	ACTOR *actor = fdActorGet(id);

	if(actor != NULL) {
		Sint16 *cnt;

		actor->tmr = frm;
		cnt = &LightSpriteCount;
		LightSpriteActors[*cnt] = actor;
		(*cnt)++;
	}
}
