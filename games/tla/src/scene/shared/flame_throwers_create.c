/* Clears the flame throwers' state (unless the party keeps its place) and makes
   the flame whose sprite the crumbs share, keeping its VRAM entry. */
void scFlameThrowersCreate(void)
{
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;
	ACTOR			*actor;
	SPR				*anm;

	if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
		syWordsClearEntry(state, sizeof(*state));
	actor = (ACTOR *)scFieldActorCreate(SPRITE_SMALL_FLAME, 1, 0, 0);
	actor->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN;
	fdActorSelectAnimation(actor, 1);
	anm = actor->spr;
	state->sharedFlame = actor;
	state->flameVramBlock = anm->slot;
	anm->retainCacheEntry = 1;
}
