// Draft, differ 6 of 106: the slot machine's per-frame tail, split out of
// ☀️'s ReelGame_RunFrame (games/tbs/src/object/colorfx.c): while a win shows
// it bounces the eight title letters, then restarts the HBlank DMA of the
// scanline offsets into BLDY and copies the frame's sprites to OAM. Left:
// instruction order only, three single moves: the bounce limit's movs #128
// after the y reload (the draft before it), the DMA mask's movs #197 before
// the control read (the draft after it), and the second masked store before
// the source and destination constants (the draft after the first two).
#include "types.h"
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
      if (p->y > 0x400000) {
        p->y = 0x400000;
        if (p->variant <= 1)
          p->velocityY = -p->velocityY / 2;
        p->variant++;
      }
    }
  }

  dma = REG_DMA0;
  dma[5] &= ~0x3a00;
  dma[5] &= ~0x8000;
  dma[5];
  Dma_Set(work->scanlineOffsets, (void *)&REG_BLDY, 0xa2600001,
          (volatile u32 *)dma);
  if (work->spriteCount > 0)
    Dma_Set(work->obj, (void *)0x07000000, 0x84000000 | (work->spriteCount * 2),
            REG_DMA3);
}
