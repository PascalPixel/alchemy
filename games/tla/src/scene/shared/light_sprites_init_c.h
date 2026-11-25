/// Loads the third set of light pictures (gField_CachedTilesC) into VRAM and
/// starts drawing the lit actors (none yet), large, each at its own priority.
void LightSprites_InitC(void) {
  u8 *buffer = Heap_CallAlternatePool(Heap_Alloc, 2048);

  Iwram_ClearWordsEntry(gLightSpriteActors, sizeof(gLightSpriteActors));
  Decompress_TaggedLz(gField_CachedTilesC, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  Vram_Load(gLightSpriteTile, 2048, buffer);
  Heap_Release(buffer);
  Task_AddFrame(LightSprites_Draw, TASK_PASS_FRAME);
  gLightSpriteCount = 0;
  gLightSpritesLarge = 1;
  gLightSpritePriority = -1;
}
