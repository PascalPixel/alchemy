/* The dust Scoop throws up, each grain growing as it flies. Part of
   field_psynergy.c. */

/* Advances one grain of Scoop's dust a tick: its sprite steps through eight
   pictures of eight tiles from tiles, four ticks each, grows by a tenth of its
   size a tick to full size, is drawn at the grain's place on screen, and the
   grain moves on (fdAdvanceWithGravity3D). Sand's fdGrowingParticleUpdate is
   the same but for its own age check and fdAdvanceWithGravity2D, so each
   Psynergy keeps its own. */
void fdScoopDustUpdateGrain(
	EFFECT_SPR *spr, FIELD_PSYNERGY_PARTICLE *particle, Sint32 tiles)
{
	Sint32 pos[3];
	FIXED  scale;

	spr->oam.idx = tiles + ((particle->tmr >> 2) & 7) * 8;
	scale = spr->scaleX + toFIXED(1) / 10;
	spr->scaleX = scale;
	if(scale > toFIXED(1))
		scale = toFIXED(1);
	spr->scaleX = scale;
	spr->scaleY = scale;
	SET_VECTOR(pos, particle->x, particle->y, particle->z);
	fdCameraWorldToScreen(pos);
	spr->x = pos[0];
	spr->y = pos[2];
	fdEffectSprDraw(spr);
	fdAdvanceWithGravity3D(particle, 60, 0);
}
