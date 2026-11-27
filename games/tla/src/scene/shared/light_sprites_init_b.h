/* Loads the second set of light pictures (Field_CachedTilesB) into VRAM and
   starts drawing the lit actors (none yet), small, each at its own priority. */
void scLightSprsInitB(void)
{
	Uint8 *buffer = syHeapAlloc(3072);

	syWordsClearEntry(LightSpriteActors, sizeof(LightSpriteActors));
	syDecompressTaggedLZ(Field_CachedTilesB, buffer);
	LightSpriteTile = syVRAMCacheFindFreeEntry();
	mnVRAMLoad(LightSpriteTile, 2048, buffer);
	syHeapRelease(buffer);
	sySetFrameEvent(scLightSprsDraw, EVENT_PASS_FRAME);
	LightSpriteCount = 0;
	LightSpritesLarge = 0;
	LightSpritePriority = -1;
}
