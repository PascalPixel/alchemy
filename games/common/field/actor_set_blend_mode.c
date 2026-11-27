/* fdActorSetBlendMode, the same in both games. Both games'
   field/field_actors.c include it where the function stands. */

/* Sets the OAM blend mode of a one-sprite actor's sprite. */
void fdActorSetBlendMode(ACTOR *actor, Uint32 mode)
{
	if(actor != 0 && actor->sprStorageKind == ACTOR_SPRITE_ONE) {
		SPR *spr = actor->spr;
		spr->image.objMode = mode;
	}
}
