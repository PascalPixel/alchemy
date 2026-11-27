/* @file */
/* The dust Scoop throws up, each grain growing as it flies. */
/* Part of field_psynergy.c. */

/* Advances one grain of Scoop's dust a frame: its sprite steps through eight */
/* pictures of eight tiles from tiles, four frames each, grows by a tenth of */
/* its size a frame to full size, is drawn at the grain's place on screen, and
 */
/* the grain moves on (fdAdvanceWithGravity3D). Sand's */
/* fdGrowingParticleUpdate is the same but for its own age check and */
/* fdAdvanceWithGravity2D, so each Psynergy keeps its own. */
void fdScoopDustUpdateGrain(EFFECT_SPR *sprite,
							FIELD_PSYNERGY_PARTICLE *particle, s32 tiles) {
	s32 position[3];
	FIXED scale;

	sprite->oam.idx = tiles + ((particle->tmr >> 2) & 7) * 8;
	scale = sprite->scaleX + FIXED_ONE / 10;
	sprite->scaleX = scale;
	if(scale > FIXED_ONE)
		scale = FIXED_ONE;
	sprite->scaleX = scale;
	sprite->scaleY = scale;
	SET_VECTOR(position, particle->x, particle->y, particle->z);
	fdCameraWorldToScr(position);
	sprite->x = position[0];
	sprite->y = position[2];
	fdEffectSprDraw(sprite);
	fdAdvanceWithGravity3D(particle, 60, 0);
}
