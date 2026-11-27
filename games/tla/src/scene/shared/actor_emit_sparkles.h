/* Sparkles rising about an actor, as about an Adept a tablet has taught */
/* Psynergy: every eighth frame a white sparkle starts within 16 pixels to */
/* either side of it, 8 to 39 pixels up, and rises half a pixel a frame. */
void fdActorEmitSparkles(ACTOR *actor) {
	if((FrameCount & 7) == 0) {
		EFFECT_OPTIONS sparkle;
		FIXED position[3];

		sparkle.pal = SPRITE_COLOR_WHITE;
		sparkle.type = SPRITE_SPARKLE;
		sparkle.pri = 1;
		position[0] = actor->x.fixed + toFIXED((syRandom() & 31) - 16);
		position[1] = actor->y.fixed + toFIXED(syRandom() & 31) + toFIXED(8);
		position[2] = actor->z.fixed;
		scFieldEffectSpawn(
			position[0], position[1], position[2] + toFIXED(4), 0, toFIXED(0.5),
			0, EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
			&sparkle);
	}
}
