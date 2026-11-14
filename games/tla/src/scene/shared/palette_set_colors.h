/// Sets palette colours from a list and saves both palettes.
void YampiDesertCave_SetMarkColors(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gYampiDesertCave_MarkSlots = slots;
  gYampiDesertCave_MarkColors = colours;
  gYampiDesertCave_MarkDimColors = a;
  gYampiDesertCave_MarkCycleSlots = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      SET_RGB(((u16 *)BG_PLTT)[index], red, green, blue);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  if ((s8)gMainRoutineWork.unk2c2 == 2)
    YampiDesertCave_ArriveByTeleport();
}
