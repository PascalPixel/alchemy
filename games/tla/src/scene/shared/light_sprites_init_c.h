/* Loads the third set of light pictures (Field_CachedTilesC) into VRAM and
   starts drawing the lit actors (none yet), large, each at its own priority. */
void scLightSprsInitC(void)
{
	Uint8 *buf = syHeapCallAlternatePool(syHeapAlloc, 2048);

	syWordsClearEntry(LightSpriteActors, sizeof(LightSpriteActors));
	syDecompressTaggedLZ(Field_CachedTilesC, buf);
	LightSpriteTile = syVRAMCacheFindFreeEntry();
	mnVRAMLoad(LightSpriteTile, 2048, buf);
	syHeapRelease(buf);
	sySetTickEvent(scLightSprsDraw, EVENT_PASS_TICK);
	LightSpriteCount = 0;
	LightSpritesLarge = 1;
	LightSpritePriority = -1;
}
