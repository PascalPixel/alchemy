/* Unused: gives a moai its water, drawn in front of it: the pool in the top of
 */
/* its head, rippling (animation 2). */
UNUSED void syMoaiAddWater(s32 id) {
	ACTOR *actor = fdActorGet(id);

	if(actor != 0) {
		SPR_ANIMATION *water =
			(SPR_ANIMATION *)fdSprAddAnimation(actor->spr, SPRITE_MOAI_WATER);

		water->colMode = SPRITE_COLOR_NORMAL;
		water->pri = 0;
		fdActorSelectAnimation(actor, 0);
		fdActorSelectAnimation(actor, 2);
	}
}
