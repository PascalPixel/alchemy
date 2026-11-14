/// Palette_SetColors, then applies the palette fade.
void Palette_SetColorsFade(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gPaletteCycle_Slots = slots;
  gPaletteCycle_FlashColors = colours;
  gPaletteCycle_Colors = a;
  gPaletteCycle_Order = b;
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
  PaletteFade_ApplyTarget(0x10000, 0);
  if ((s8)gMainRoutineWork.unk2c2 == 2)
    Teleport_ArriveWhirling();
}
