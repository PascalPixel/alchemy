/// Sets up the large glow sprites from the field's cached tiles C.
void LightSprites_InitC(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWordsEntry(gLightSpriteActors, 64);
  Decompress_TaggedLz(gField_CachedTilesC, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  Vram_Load(gLightSpriteTile, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(LightSprites_Draw, TASK_PASS_VBLANK);
  gLightSpriteCount = 0;
  gLightSpritesLarge = 1;
  gLightSpritePriority = -1;
}
