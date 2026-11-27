// The dust Scoop throws up, each grain growing as it flies. Part of field_psynergy.c.

// Advances one grain of Scoop's dust a tick: its sprite steps through eight pictures of eight tiles
// from tiles, four ticks each, grows by a tenth of its size a tick to full size, is drawn at the
// grain's place on screen, and the grain moves on (fdAdvanceWithGravity3D). Sand's
// fdGrowingParticleUpdate is the same but for its own age check and fdAdvanceWithGravity2D, so each
// Psynergy keeps its own.
void fdScoopDustUpdateGrain(EFFECT_SPR *sprite, FIELD_PSYNERGY_PARTICLE *particle, Sint32 tiles)
{
	Sint32 pos[3];
	FIXED  scale;

	sprite->oam.index = tiles + ((particle->timer >> 2) & 7) * 8;
	scale             = sprite->scaleX + toFIXED(1) / 10;
	sprite->scaleX    = scale;
	if (scale > toFIXED(1))
		scale = toFIXED(1);
	sprite->scaleX = scale;
	sprite->scaleY = scale;
	SET_VECTOR(pos, particle->x, particle->y, particle->z);
	fdCameraWorldToScreen(pos);
	sprite->x = pos[0];
	sprite->y = pos[2];
	fdEffectSpriteDraw(sprite);
	fdAdvanceWithGravity3D(particle, 60, 0);
}
