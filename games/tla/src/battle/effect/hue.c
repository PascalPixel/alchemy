#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"

struct WaveFxScroll {
  u16 lines[2][162];
  u8 unk288[2];
  u8 page;
  u8 unk28b[9];
  u8 delay;
};

/// Applies BG0 scanline scrolling with HBlank DMA.
void BattleFx_ArmBg0HBlankDma(void) {
  struct WaveFxScroll *work = Ram_HeapSlots->unk4c[3];

  if (!work->delay) {
    /* FAKEMATCH: ⚓️ stores the stopped control word before it starts the
     * destination address and before the line offset's last shift; written as
     * work->lines[work->page] the address is started ahead of the store (differ
     * 2), and with the barrier alone the whole offset is (differ 2). The
     * temporary holds the page's offset in words until the barrier. */
    s32 line = work->page * 81;
    volatile u16 *dma = (volatile u16 *)0x040000b0;

    dma[5] &= 0xc5ff; // Clear start timing and repeat.
    dma[5] &= 0x7fff; // Stop.
    /* FAKEMATCH: see above; the control word is stored before the rest. */
    asm volatile("");
    line *= 4;
    (void)dma[5];
    // One halfword per HBlank, repeating with destination reload.
    Dma_Set((u8 *)work + line, (void *)0x04000010, 0xa2600001,
            (volatile u32 *)dma);
  }
}

/// One colour channel's level (0 to 31) at a hue in degrees: rising from 0
/// to 60, full from 60 to 180, falling from 180 to 240, else 0.
s32 BattleFx_HueChannelRamp(s16 hue) {
  s32 level = 0;
  s16 angle = hue;

  angle = (angle + 360) % 360;
  if (angle < 60) {
    level = angle * 31 / 60;
  } else if (angle < 180) {
    level = 31;
  } else if (angle < 240) {
    level = (240 - angle) * 31 / 60;
  }
  return level;
}

struct WaveFxWork {
  u8 unk000[0x28b];
  u8 red;
  u8 green;
  u8 blue;
  u16 hue; ///< Degrees.
};

/// Advances the hue by four degrees and calculates its colour.
void BattleFx_AdvanceHueCycle(void) {
  struct WaveFxWork *work = Ram_HeapSlots->unk4c[3];
  s16 r;
  s16 g;
  s16 b;
  u32 limit;

  /* FAKEMATCH: ⚓️ loads the work block before it builds the hue's offset;
   * without the barrier the offset's first half is scheduled ahead of the load
   * (differ 2) */
  asm volatile("");
  r = BattleFx_HueChannelRamp(work->hue);
  g = BattleFx_HueChannelRamp(work->hue + 120);
  b = BattleFx_HueChannelRamp(work->hue + 240);

  // Scale 0-31 down to 4-11.
  work->red = (r / 4 + 4) & 31;
  work->green = (g / 4 + 4) & 31;
  work->blue = (b / 4 + 4) & 31;
  limit = 359 << 16;
  /* FAKEMATCH: ⚓️ loads the wrap limit from the pool before it reads the hue;
   * written as work->hue += 4; if (work->hue >= 360) the same instructions come
   * with the hue read first (differ 2). The temporary holds the limit as the
   * compare uses it, 359 degrees in the high half. */
  asm volatile("" : "+l"(limit));
  if ((u32)((work->hue += 4) << 16) > limit)
    work->hue = 0;
}
