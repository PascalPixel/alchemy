/* Clears the flame throwers' state (unless the party keeps its place) and makes
   the flame whose sprite the crumbs share, keeping its VRAM entry. */
void scFlameThrowersCreate(void)
{
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;
	ACTOR			*actor;
	SPR				*animation;

	if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
		syWordsClearEntry(state, sizeof(*state));
	actor = (ACTOR *)scFieldActorCreate(SPRITE_SMALL_FLAME, 1, 0, 0);
	actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
	fdActorSelectAnimation(actor, 1);
	animation = actor->sprite;
	state->sharedFlame = actor;
	state->flameVramBlock = animation->slot;
	animation->retainCacheEntry = 1;
}
