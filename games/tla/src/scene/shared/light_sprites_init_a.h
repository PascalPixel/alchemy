/* Unused: loads the first set of light pictures (FieldCachedTilesA) into VRAM
   and starts drawing the lit actors (none yet), small, each at its own
   priority. */
UNUSED void scLightSprsInitA(void)
{
	Uint8 *buf = syHeapCallAlternatePool(syHeapAlloc, 3072);

	syWordsClearEntry(LightSpriteActors, sizeof(LightSpriteActors));
	syDecompressTaggedLZ(FieldCachedTilesA, buf);
	LightSpriteTile = syVRAMCacheFindFreeEntry();
	syVRAMCacheLoad(LightSpriteTile, 3072, buf);
	syHeapRelease(buf);
	sySetEvent(scLightSprsDraw, EVENT_PASS_TICK);
	LightSpriteCount = 0;
	LightSpritesLarge = 0;
	LightSpritePriority = -1;
}
