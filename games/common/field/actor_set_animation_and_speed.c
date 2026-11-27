/* fdActorSetAnimationAndSpeed, the same in both games. Both games'
   field/field_actors.c include it where the function stands. */

/* Both of the above: switches all of an actor's sprites to animation and sets
   their timer step to step. */
void fdActorSetAnimationAndSpeed(ACTOR *actor, Sint32 anm, Sint32 step)
{
	void **sprs;
	void  *spr;
	Sint32 part;

	if(actor != 0) {
		switch(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) {
		case ACTOR_SPRITE_ONE:
			fdSprStartAnimation(actor->spr, anm);
			fdSprSetAnimationSpeed(actor->spr, step);
			break;
		case ACTOR_SPRITE_GROUP:
			sprs = (void **)actor->spr;
			for(part = 0; part < 4; part++) {
				spr = *sprs++;
				if(spr != 0) {
					fdSprStartAnimation(spr, anm);
					fdSprSetAnimationSpeed(spr, step);
				}
			}
			break;
		}
	}
}
