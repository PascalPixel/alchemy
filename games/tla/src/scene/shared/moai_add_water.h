/* Unused: gives a moai its water, drawn in front of it: the pool in the top of
   its head, rippling (animation 2). */
UNUSED void scMoaiAddWater(Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	if(actor != 0) {
		SPR_ANIMATION *water = (SPR_ANIMATION *)fdSpriteAddAnimation(
			actor->sprite, SPRITE_MOAI_WATER);

		water->colorMode = SPRITE_COLOR_NORMAL;
		water->priority = 0;
		fdActorSelectAnimation(actor, 0);
		fdActorSelectAnimation(actor, 2);
	}
}
