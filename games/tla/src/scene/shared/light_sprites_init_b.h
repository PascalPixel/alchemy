/* Loads the second set of light pictures (FieldCachedTilesB) into VRAM and
   starts drawing the lit actors (none yet), small, each at its own priority. */
void scLightSpritesInitB(void)
{
	Uint8 *buf = syHeapAllocate(3072);

	syWordsClearEntry(LightSpriteActors, sizeof(LightSpriteActors));
	syDecompressTaggedLZ(FieldCachedTilesB, buf);
	LightSpriteTile = syVRAMCacheFindFreeEntry();
	mnVRAMLoad(LightSpriteTile, 2048, buf);
	syHeapRelease(buf);
	sySetTickEvent(scLightSpritesDraw, EVENT_PASS_TICK);
	LightSpriteCount = 0;
	LightSpritesLarge = 0;
	LightSpritePriority = -1;
}
