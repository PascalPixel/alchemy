/* fdActorRelease, the same in both games. Both games' field/field_actors.c
   include it where the function stands. */

/* Releases an actor's sprites and zeroes it, releasing its slot. Clearing the
   script lets fdActorFindFree hand the actor out again. */
void fdActorRelease(ACTOR *actor)
{
	Sint32 sprsRemaining;
	void **sprs;

	if(actor) {
		switch(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) {
		case ACTOR_SPRITE_ONE:
			fdSprDestroy(actor->spr);
			break;
		case ACTOR_SPRITE_GROUP:
			sprs = (void **)actor->spr;
			for(sprsRemaining = 0; sprsRemaining < 4; sprsRemaining++) {
				SPR *spr = *sprs++;
				if(spr)
					fdSprDestroy(spr);
			}
			break;
		}
		DMA_FILL32(0, actor, sizeof(ACTOR));
	}
}
