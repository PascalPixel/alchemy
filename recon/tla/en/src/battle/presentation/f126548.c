#include "types.h"
#include "fixed_math.h"
#include "battle_display.h"
#include "iwram_call.h"
#include "ram_buffer.h"

struct BgScroll {
  s16 unk0;
  s16 unk2;
  s16 x;
  s16 y;
};

extern struct BgScroll gBgScroll[];

s32 GameFlag_Test(s32 flag);

void BattleCamera_SetRange(s32 centerX, s32 centerY, s32 offsetX, s32 offsetY,
                           s32 scale) {
  struct BattleBackgroundView *view =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  struct BattleAffineHdma *scanlines = Ram_HeapSlots->unk28[0];
  struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
  s32 horizon;
  u16 wrap;
  u16 *line;
  struct BattleAffine *affine;
  s32 ratio;
  s32 step;
  s32 x;
  s32 y;
  u32 scanline;
  u32 groundEndLine;

  horizon = 0x800;
  wrap = 0;
  /* FAKEMATCH(temporary): Q16_ONE held in ratio for the scale test and the
     Iwram_RatioMulQ14Entry call; as the constant in both, 4 of 199 instructions
     differ in tla-en. */
  ratio = Q16_ONE;
  if (scale >= ratio) {
    wrap = 0x2000;
    horizon = 0x6800 + -(s16)camera->yaw * 3;
  }
  if (scanlines == NULL)
    return;
  if ((view->mode == 1 || view->secondMode == 1) && view->busy == 0)
    gBgScroll[0].x = horizon >> 8;
  if (view->mode != 2)
    return;
  line = scanlines->lines[scanlines->page ^ 1];
  ratio = Iwram_RatioMulQ14(scale, ratio);
  step = ratio >> 8;
  affine = &scanlines->affine;
  affine->pa = step;
  affine->pb = 0;
  affine->pc = 0;
  affine->pd = step;
  x = Iwram_MulQ16(ratio, Iwram_MulQ16(centerX, scale - 0x10000));
  y = Iwram_MulQ16(ratio, Iwram_MulQ16(centerY, scale - 0x10000));
  affine->x = ((x + SPLIT_CONSTANT(0x7fff)) >> 8) + offsetX + horizon;
  affine->y = ((y + 0x7fff) >> 8) + offsetY - 0x1000;
  groundEndLine =
      (Iwram_RatioMulQ14((s16)affine->pd, 0x4000 - affine->y) >> 16) + 1;
  scanline = 0;
  if (!GameFlag_Test(0x16b)) {
    for (; scanline < 16; scanline++)
      *line++ = 0x3f8e;
  }
  if (groundEndLine > 136)
    groundEndLine = 136;
  for (; scanline < groundEndLine; scanline++)
    *line++ = wrap | 0x478a;
  for (; scanline < 136; scanline++)
    *line++ = wrap | 0x478e;
  for (; scanline < 160; scanline++)
    *line++ = 0x3f8e;
  scanlines->page ^= 1;
}
