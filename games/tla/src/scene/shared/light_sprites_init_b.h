/* Loads the second set of light pictures (Field_CachedTilesB) into VRAM and */
/* starts drawing the lit actors (none yet), small, each at its own priority. */
void scLightSprsInitB(void) {
	u8 *buffer = syHeapAlloc(3072);

	syWordsClearEntry(LightSpriteActors, sizeof(LightSpriteActors));
	syDecompressTaggedLz(Field_CachedTilesB, buffer);
	LightSpriteTile = syVramCacheFindFreeEntry();
	mnVramLoad(LightSpriteTile, 2048, buffer);
	syHeapRelease(buffer);
	syTaskAddFrame(scLightSprsDraw, TASK_PASS_FRAME);
	LightSpriteCount = 0;
	LightSpritesLarge = 0;
	LightSpritePriority = -1;
}
