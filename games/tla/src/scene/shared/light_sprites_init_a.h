/* Unused: loads the first set of light pictures (Field_CachedTilesA) into VRAM
   and starts drawing the lit actors (none yet), small, each at its own
   priority. */
UNUSED void scLightSprsInitA(void)
{
	Uint8 *buffer = syHeapCallAlternatePool(syHeapAlloc, 3072);

	syWordsClearEntry(LightSpriteActors, sizeof(LightSpriteActors));
	syDecompressTaggedLZ(Field_CachedTilesA, buffer);
	LightSpriteTile = syVRAMCacheFindFreeEntry();
	syVRAMCacheLoad(LightSpriteTile, 3072, buffer);
	syHeapRelease(buffer);
	sySetEvent(scLightSprsDraw, EVENT_PASS_FRAME);
	LightSpriteCount = 0;
	LightSpritesLarge = 0;
	LightSpritePriority = -1;
}
