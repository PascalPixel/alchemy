#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "io_write_queue.h"
#include "ram_buffer.h"
#include "battle_work.h"
#include "battle_display.h"

void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void BattlePres_UpdateHBlankScroll(void);
void Graphics_BuildSequentialTileTable(void *destination);
void Func_08125bb8(void *destination);

/// The Broken Seal's BattlePresentation_ConfigurePaletteFade, with this
/// game's heap slots, its callback BattlePres_UpdateHBlankScroll at 0xcff and
/// its IO write of 0x1f83 to BG1CNT.
void Func_0812628c(s32 mode, s32 value, s32 fade) {
  struct BattleBackgroundView *transition =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;

  if (transition->mode == 0)
    Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0xcff);
  transition->mode = mode;

  if (mode == 1)
    IO_WRITE_QUEUE_PUSH(0x0400000a, 0x1f83, 0x20000);

  Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
  *(u16 *)0x050000bc = *(u16 *)0x050001e8;

  if (fade == 0x80) {
    struct BattleWork *work;
    u16 *palette;

    work = Ram_HeapSlots->battleWork;
    palette = work->palette;
    Dma_Set(palette, (void *)0x050000c0, 0x80000080, REG_DMA3);
  } else if (fade != 0) {
    struct BattleWork *work;
    u16 *source;
    u16 *destination;
    s32 i;

    work = Ram_HeapSlots->battleWork;
    source = work->palette;
    destination = (u16 *)0x050000c0;

    for (i = 0; i != 128; i++) {
      s32 red = source[i] & 31;
      s32 green = (source[i] >> 5) & 31;
      s32 blue = (source[i] >> 10) & 31;

      if (red > fade)
        red -= fade;
      else
        red = 0;
      if (green > fade)
        green -= fade;
      else
        green = 0;
      if (blue > fade)
        blue -= fade;
      else
        blue = 0;
      destination[i] = (blue << 10) | (green << 5) | red;
    }
  }
  Graphics_BuildSequentialTileTable((void *)0x06003800);
  Func_08125bb8((void *)0x0600f800);
}
