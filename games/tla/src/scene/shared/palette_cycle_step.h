/// Steps a five-colour palette cycle, unless an event holds the field.
void PaletteCycle_Step(void) {
  u16 colors[16];
  s32 phase = (gPaletteCycleFrame / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gPaletteCyclePausesInEvents != 0) {
    u8 *work = Ram_HeapSlots->eventWork;
    s16 *state = (s16 *)(work + 358);

    if (*state != 0 && (*state & 0x3ff) != 153)
      return;
    if (*(s8 *)(work + 0xca4) != 0)
      return;
    if (*(s16 *)(work + 350) != 0)
      return;
  }
  i = 0;
  offset = 4;
next:
  p = colors;
  {
    /* FAKEMATCH(temporary): the slot and the table address held in locals.
     */
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gPaletteCycleColors;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, DMA_COPY_WORDS(2), REG_DMA3);
  gPaletteCycleFrame++;
}
