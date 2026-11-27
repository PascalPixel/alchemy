/* fdActorRegisterSprMetadata, the same in both games. Both games'
   field/field_actors.c include it where the function stands. */

/* Adds a sprite animation of animation id anmId to a one-sprite actor's
   sprite. A negative anmId does nothing. */
void fdActorRegisterSprMetadata(ACTOR *actor, Sint32 anmId)
{
	if(actor != 0 &&
		(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
		SPR *spr = actor->spr;
		if(anmId >= 0)
			fdSprAddAnimation(spr, anmId);
	}
}
