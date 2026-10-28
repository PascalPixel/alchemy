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

/// Applies BG0 line scrolling with HBlank DMA.
void BattleEffect_ArmBg0HBlankDma(void) {
  struct WaveFxScroll *work = Ram_HeapSlots->unk4c[3];

  if (!work->delay) {
    /* FAKEMATCH: the temporary holds the page's offset in words until the
     * barrier, so the stopped control word is stored first; as
     * work->lines[work->page], 2 instructions differ in tla-en. */
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
s32 BattleEffect_HueChannelRamp(s16 hue) {
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
void BattleEffect_AdvanceHueCycle(void) {
  struct WaveFxWork *work = Ram_HeapSlots->unk4c[3];
  s16 r;
  s16 g;
  s16 b;
  u32 limit;

  /* FAKEMATCH: ⚓️ loads the work block before it builds the hue's offset;
   * without the barrier the offset's first half is scheduled ahead of the load
   * (differ 2) */
  asm volatile("");
  r = BattleEffect_HueChannelRamp(work->hue);
  g = BattleEffect_HueChannelRamp(work->hue + 120);
  b = BattleEffect_HueChannelRamp(work->hue + 240);

  // Scale 0-31 down to 4-11.
  work->red = (r / 4 + 4) & 31;
  work->green = (g / 4 + 4) & 31;
  work->blue = (b / 4 + 4) & 31;
  limit = 359 << 16;
  /* FAKEMATCH: the limit, 359 degrees in the high half, loads before the hue;
   * as work->hue += 4; if (work->hue >= 360), 2 instructions differ in
   * tla-en. */
  asm volatile("" : "+l"(limit));
  if ((u32)((work->hue += 4) << 16) > limit)
    work->hue = 0;
}
