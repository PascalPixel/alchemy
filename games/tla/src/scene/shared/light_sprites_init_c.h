/* Loads the third set of light pictures (Field_CachedTilesC) into VRAM and
   starts drawing the lit actors (none yet), large, each at its own priority. */
void scLightSprsInitC(void)
{
	Uint8 *buffer = syHeapCallAlternatePool(syHeapAlloc, 2048);

	syWordsClearEntry(LightSpriteActors, sizeof(LightSpriteActors));
	syDecompressTaggedLZ(Field_CachedTilesC, buffer);
	LightSpriteTile = syVRAMCacheFindFreeEntry();
	mnVRAMLoad(LightSpriteTile, 2048, buffer);
	syHeapRelease(buffer);
	sySetFrameEvent(scLightSprsDraw, EVENT_PASS_FRAME);
	LightSpriteCount = 0;
	LightSpritesLarge = 1;
	LightSpritePriority = -1;
}
