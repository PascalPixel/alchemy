// Smoke rising from an actor: every eighth tick a black puff at 0.7 of its size rises from within 4
// pixels of it, 13 to 32 pixels up, at half a pixel to 0.9 a tick. The energy block smokes so once
// lightning has struck it (scJupiterLighthouseFlashSky).
Sint32 scActorEmitSmoke(ACTOR *actor)
{
	EFFECT_OPTIONS puff;

	if ((TickCount & 7) == 0)
	{
		FIXED x;
		FIXED y;
		FIXED speed;

		puff.priority    = 3;
		puff.startScaleX = toFIXED(7) / 10;
		puff.startScaleY = toFIXED(7) / 10;
		puff.palette     = SPRITE_COLOR_BLACK;
		x                = actor->x.fixed + toFIXED(syRandomBelow(9) - 4);
		y                = actor->y.fixed + toFIXED(32 - syRandomBelow(20));
		speed            = toFIXED(syRandomBelow(5) + 5) / 10;
		scFieldEffectSpawn(x,
		                   y,
		                   actor->z.fixed,
		                   0,
		                   speed,
		                   0,
		                   EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE,
		                   &puff);
	}
	return 0;
}
