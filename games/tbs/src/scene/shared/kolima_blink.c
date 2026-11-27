/* Kolima and Kolima Forest (SceneId_Kolima and SceneId_KolimaForestEntrance):
   the sparkling dust that falls when Tret's curse strikes. "That sparkly stuff
   on the ground... What is it?" */

/* An actor script call, each tick of the fall: the speck flickers between a
   red tint and white, two ticks of each. */
static Sint32 scKolimaFlickerCurseDust(ACTOR *speck)
{
	if((TickCount >> 1) & 1) {
		fdActorSetSprCol(speck, SPRITE_COLOR_RED_TINT);
	} else {
		fdActorSetSprCol(speck, SPRITE_COLOR_WHITE);
	}
	return 0;
}
