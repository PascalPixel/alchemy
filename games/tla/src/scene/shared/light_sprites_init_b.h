/// Loads the second set of light pictures (gField_CachedTilesB) into VRAM and
/// starts drawing the lit actors (none yet), small, each at its own priority.
void LightSprites_InitB(void) {
  u8 *buffer = Heap_Alloc(3072);

  Iwram_ClearWordsEntry(gLightSpriteActors, sizeof(gLightSpriteActors));
  Decompress_TaggedLz(gField_CachedTilesB, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  Vram_Load(gLightSpriteTile, 2048, buffer);
  Heap_Release(buffer);
  Task_AddFrame(LightSprites_Draw, TASK_PASS_FRAME);
  gLightSpriteCount = 0;
  gLightSpritesLarge = 0;
  gLightSpritePriority = -1;
}
