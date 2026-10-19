// The reel game. Most of its code is not C yet; its tables, which end the
// bank, are The Broken Seal's.

#include "types.h"
#include "tla_edition.h"
#include "io_reg.h"
#include "dma.h"
#include "effect_step.h"
#include "ram_buffer.h"

struct ReelWork {
  u8 unk000[0x8c];
  s32 state;
  u8 unk090[0xa4 - 0x90];
  s32 timer;
  s32 spriteCount;
  u8 unk0ac[0xc8 - 0xac];
  u32 obj[(0x4cc - 0xc8) / 4];
  s32 subWindow;
  u8 unk4d0[8];
  u16 scanlineOffsets[160];
};

struct ReelEffectWork {
  struct EffectStep particles[8];
};

// The slot machine's per-frame tail, split out of what is ☀️'s
// ReelGame_RunFrame: while a win shows (state 3) it bounces the eight title
// letters, then restarts the HBlank DMA of the scanline offsets into BLDY and
// copies the frame's sprites to OAM.
void Func_081b3294(void) {
  struct ReelWork *work;
  struct ReelEffectWork *fx;
  volatile u16 *dma;
  s32 i;

  work = (struct ReelWork *)Ram_HeapSlots->canvasMap;
  fx = (struct ReelEffectWork *)Ram_HeapSlots->battleFxWork[0];
  if (work->state == 3) {
    for (i = 0; i != 8; i++) {
      struct EffectStep *p = &fx->particles[i];

      p->y += p->velocityY;
      p->velocityY += 0x4000;
      if (work->timer % 256 == i * 4 + 200) {
        p->velocityY = 0x60000;
        p->variant = 0;
      }
      /* FAKEMATCH(barrier): the y reload issued before the limit's constant;
       * as plain C, 2 of 106 instructions differ in tla-en. */
      asm("");
      if (p->y > 0x400000) {
        p->y = 0x400000;
        if (p->variant <= 1)
          p->velocityY = -p->velocityY / 2;
        p->variant++;
      }
    }
  }

  dma = REG_DMA0;
  {
    /* FAKEMATCH(temporary): the control read before the mask's constant,
     * with the barrier; as dma[5] &= ~DMA_START_TIMING_DRQ_REPEAT, 2 of 106
     * instructions differ in tla-en. */
    u16 control = dma[5];
    asm("");
    dma[5] = control & ~DMA_START_TIMING_DRQ_REPEAT;
  }
  dma[5] &= ~DMA_ENABLE;
  /* FAKEMATCH(barrier): the masked store issued before the transfer's
   * constants; as plain C, 2 of 106 instructions differ in tla-en. */
  asm("");
  (void)dma[5];
  // One halfword at each HBlank (repeating, destination reloaded).
  Dma_Set(work->scanlineOffsets, (void *)&REG_BLDY, 0xa2600001,
          (volatile u32 *)dma);
  if (work->spriteCount > 0)
    Dma_Set(work->obj, (void *)0x07000000, 0x84000000 | (work->spriteCount * 2),
            REG_DMA3);
}

#include "reel_tables.h"
