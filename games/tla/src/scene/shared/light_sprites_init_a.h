/// Unused: loads the first set of light pictures (gField_CachedTilesA) into
/// VRAM and starts drawing the lit actors (none yet), small, each at its own
/// priority.
UNUSED void LightSprites_InitA(void) {
  u8 *buffer = Heap_CallAlternatePool(Heap_Alloc, 3072);

  Iwram_ClearWordsEntry(gLightSpriteActors, sizeof(gLightSpriteActors));
  Decompress_TaggedLz(gField_CachedTilesA, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  VramCache_Load(gLightSpriteTile, 3072, buffer);
  Heap_Release(buffer);
  Task_Add(LightSprites_Draw, TASK_PASS_FRAME);
  gLightSpriteCount = 0;
  gLightSpritesLarge = 0;
  gLightSpritePriority = -1;
}
