/* Loads the second set of light pictures (Field_CachedTilesB) into VRAM and
   starts drawing the lit actors (none yet), small, each at its own priority. */
void scLightSprsInitB(void)
{
	Uint8 *buf = syHeapAlloc(3072);

	syWordsClearEntry(LightSpriteActors, sizeof(LightSpriteActors));
	syDecompressTaggedLZ(Field_CachedTilesB, buf);
	LightSpriteTile = syVRAMCacheFindFreeEntry();
	mnVRAMLoad(LightSpriteTile, 2048, buf);
	syHeapRelease(buf);
	sySetTickEvent(scLightSprsDraw, EVENT_PASS_TICK);
	LightSpriteCount = 0;
	LightSpritesLarge = 0;
	LightSpritePriority = -1;
}
