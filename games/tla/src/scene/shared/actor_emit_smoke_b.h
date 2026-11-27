/* Unused: a copy of scActorEmitSmoke, which three overlays keep and nothing
   sets as an update: every eighth tick a black puff of smoke at 0.7 of its
   size rises from within 4 pixels of the actor, 13 to 32 pixels up, at half a
   pixel to 0.9 a tick. */
UNUSED Sint32 scActorEmitSmokeB(ACTOR *actor)
{
	EFFECT_OPTIONS puff;

	if((TickCount & 7) == 0) {
		FIXED x;
		FIXED y;

		puff.pri = 3;
		puff.startScaleX = toFIXED(7) / 10;
		puff.startScaleY = toFIXED(7) / 10;
		puff.pal = SPRITE_COLOR_BLACK;
		x = actor->x.fixed + toFIXED(syRandomBelow(9) - 4);
		y = actor->y.fixed + toFIXED(32 - syRandomBelow(20));
		scFieldEffectSpawn(x, y, actor->z.fixed, 0,
			toFIXED(syRandomBelow(5) + 5) / 10, 0,
			EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE,
			&puff);
	}
	return 0;
}
