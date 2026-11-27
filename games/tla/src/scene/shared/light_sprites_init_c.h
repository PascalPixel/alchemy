// Loads the third set of light pictures (FieldCachedTilesC) into VRAM and starts drawing the lit
// actors (none yet), large, each at its own priority.
void scLightSpritesInitC(void)
{
	Uint8 *buf = syHeapCallAlternatePool(syHeapAllocate, 2048);

	syWordsClearEntry(LightSpriteActors, sizeof(LightSpriteActors));
	syDecompressTaggedLZ(FieldCachedTilesC, buf);
	LightSpriteTile = syVRAMCacheFindFreeEntry();
	mnVRAMLoad(LightSpriteTile, 2048, buf);
	syHeapRelease(buf);
	sySetTickEvent(scLightSpritesDraw, EVENT_PASS_TICK);
	LightSpriteCount    = 0;
	LightSpritesLarge   = 1;
	LightSpritePriority = -1;
}
