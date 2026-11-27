/* Mercury Lighthouse and its entrance: the trail of sparks the leader leaves
   while passing between floors. */

/* The leader's update function while passing: each frame a small spark rises
   from a point that sweeps across the leader's chest, white and tinted by
   turns. */
static UNREAD_RESULT scMercuryLighthouseTrailSparks(ACTOR *actor)
{
	EFFECT_OPTIONS options;
	FIXED		   velocityY;

	options.pal = SPRITE_COLOR_WHITE;
	if((FrameCount & 1) == 0) {
		options.pal = SPRITE_COLOR_BRG;
	}
	options.startScaleX = toFIXED(4) / 5;
	options.startScaleY = toFIXED(4) / 5;
	options.pri = 0;
	velocityY = -((syRandomBelow(8)) * (toFIXED(1) / 5));
	fdEffectSpawn(actor->x.fixed + toFIXED(8 - (FrameCount & 15)),
		actor->y.fixed + toFIXED(26), actor->z.fixed, 0, velocityY, 0,
		EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
		&options);
	RETURN_UNREAD(0);
}
