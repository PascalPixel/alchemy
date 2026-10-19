#include "types.h"
#include "ram_buffer.h"
#include "dma.h"

struct AffineStepWork {
  u8 unk0000[0x7790];
  s32 counter;
  s32 period;
  s32 stepX;
  s32 stepY;
  u8 unk77a0[0x30];
  s32 x;
  s32 y;
};

/// Scheduler callback: every period frames, writes the BG2 reference point to
/// the hardware, then moves it by (stepX, stepY).
void BattleFx_AdvanceBg2OnInterval(void) {
  struct AffineStepWork *work;

  /* FAKEMATCH: the one-pass block loads the effect work before the counter
   * offset is started (one reordered pair, differ 2 without it). */
  do {
    work = (struct AffineStepWork *)Ram_HeapSlots->battleFxWork[0];
  } while (0);

  if (++work->counter == work->period) {
    /* FAKEMATCH: x pinned to r3 loads before the affine reference register's
     * address starts, as the native does; as a plain store, 2 instructions
     * differ in tla-en. */
    register s32 x __asm__("r3") = work->x;

    /* FAKEMATCH: the one-pass block's loop note holds the address until the
     * pinned value is loaded. */
    do {
    } while (0);
    *(volatile s32 *)0x04000028 = x;
    *(volatile s32 *)0x0400002c = work->y;
    work->x += work->stepX;
    work->y += work->stepY;
    work->counter = 0;
  }
}

/// Scheduler callback: restarts DMA0's HBlank copy of one WIN0H value per
/// scanline from the map cell buffer.
void BattleFx_ArmWin0HBlankDma(void) {
  volatile u16 *dma = (volatile u16 *)0x040000b0;
  u16 control = dma[5];

  /* FAKEMATCH: ⚓️ reads the control word before it starts the mask; written as
   * dma[5] &= 0xc5ff the mask is started first (differ 2) */
  asm volatile("");
  dma[5] = control & 0xc5ff; // Clear start timing and repeat.
  dma[5] &= 0x7fff;          // Stop.
  /* FAKEMATCH: ⚓️ stores the stopped control word before it starts the
   * destination address (differ 2 without) */
  asm volatile("");
  (void)dma[5];
  Dma_Set(Ram_MapCellBuffer, (void *)0x04000040, 0xa2600001,
          (volatile u32 *)dma);
}
