/* Mist about an actor, as the leader walks into Aqua Rock's waterfall: every
   other tick a dust cloud at 0.8 of its size, white and pale blue by turns,
   sinks from 26 pixels above it at up to 1.4 pixels a tick, the clouds
   sweeping from 8 pixels right of it to 7 left every 16 ticks. */
Sint32 scActorEmitMist(ACTOR *actor)
{
	EFFECT_OPTIONS mist;

	if(TickCount & 1) {
		Sint32 drift;
		FIXED  x;
		FIXED  y;

		mist.palette = SPRITE_COLOR_WHITE;
		if((TickCount & 2) == 0)
			mist.palette = SPRITE_COLOR_BRG;
		mist.startScaleX = toFIXED(4) / 5;
		mist.startScaleY = toFIXED(4) / 5;
		mist.priority = 0;
		drift = syRandomBelow(8) * toFIXED(0.2);
		x = actor->x.fixed;
		y = actor->y.fixed;
		scFieldEffectSpawn(x + toFIXED(8 - (TickCount & 15)), y + toFIXED(26),
			actor->z.fixed, 0, -drift, 0,
			EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE,
			&mist);
	}
	return 0;
}
