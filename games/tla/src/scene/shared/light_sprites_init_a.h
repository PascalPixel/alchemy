/* Unused: loads the first set of light pictures (Field_CachedTilesA) into */
/* VRAM and starts drawing the lit actors (none yet), small, each at its own */
/* priority. */
UNUSED void scLightSprsInitA(void) {
	u8 *buffer = syHeapCallAlternatePool(syHeapAlloc, 3072);

	syWordsClearEntry(LightSpriteActors, sizeof(LightSpriteActors));
	syDecompressTaggedLz(Field_CachedTilesA, buffer);
	LightSpriteTile = syVramCacheFindFreeEntry();
	syVramCacheLoad(LightSpriteTile, 3072, buffer);
	syHeapRelease(buffer);
	sySetEvent(scLightSprsDraw, TASK_PASS_FRAME);
	LightSpriteCount = 0;
	LightSpritesLarge = 0;
	LightSpritePriority = -1;
}
