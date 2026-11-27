/* Loads the third set of light pictures (Field_CachedTilesC) into VRAM and */
/* starts drawing the lit actors (none yet), large, each at its own priority. */
void scLightSprsInitC(void) {
	u8 *buffer = syHeapCallAlternatePool(syHeapAlloc, 2048);

	syWordsClearEntry(LightSpriteActors, sizeof(LightSpriteActors));
	syDecompressTaggedLz(Field_CachedTilesC, buffer);
	LightSpriteTile = syVramCacheFindFreeEntry();
	mnVramLoad(LightSpriteTile, 2048, buffer);
	syHeapRelease(buffer);
	syTaskAddFrame(scLightSprsDraw, TASK_PASS_FRAME);
	LightSpriteCount = 0;
	LightSpritesLarge = 1;
	LightSpritePriority = -1;
}
