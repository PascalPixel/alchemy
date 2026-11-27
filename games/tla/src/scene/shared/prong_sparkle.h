/* A Prong's update while it waits to be taken, the Left Prong in Ankohl Ruins
 */
/* and the Right Prong in the Shrine of the Sea God: every eighth frame a */
/* sparkle rises somewhere about it. */
void syProngSparkle(ACTOR *actor) {
	EFFECT_OPTIONS options;
	u32 phase = FrameCount & 7;
	s32 dx;
	s32 dy;

	if(phase == 0) {
		dx = (syRandom() & 15) - 8;
		dy = syRandom() & 7;
		options.type = SPRITE_SPARKLE;
		scFieldEffectSpawn(actor->x.fixed + INTtoFIXED(dx),
						   actor->y.fixed + INTtoFIXED(dy - 8), actor->z.fixed,
						   0, toFIXED(1) / 2, phase, EFFECT_USE_TYPE, &options);
	}
}
