// Draft, differ 65 of 199: ☀️'s BattleCamera_SetRange (games/tbs/src/battle/
// battle.c), ported: builds the next affine background page from the view
// centre, offsets and zoom, dividing scanlines into sky, ground and hidden
// rows before swapping pages. ⚓️ reads the ground ratio's step back from
// affine->pd (ldrsh) rather than from the local (149 with (s16)step). Left:
// instruction order and register choice: the prologue's heap-slot loads and
// the shift of 0x03000000 sit a few slots from the native's, and in the
// affine y sum the native keeps y in r0 and reads pd into r3 where here y
// takes r2 and pd r0, moving the pool loads around them. horizon declared
// before wrap puts them in the native stack slots; ☀️'s order gives 75, and
// wrap set before horizon 77.
#include "types.h"
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
  if (scale >= 0x10000) {
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
  ratio = Iwram_RatioMulQ14(scale, 0x10000);
  step = ratio >> 8;
  affine = &scanlines->affine;
  affine->pa = step;
  affine->pb = 0;
  affine->pc = 0;
  affine->pd = step;
  x = Iwram_MulQ16(ratio, Iwram_MulQ16(centerX, scale - 0x10000));
  y = Iwram_MulQ16(ratio, Iwram_MulQ16(centerY, scale - 0x10000));
  affine->x = ((x + 0x7fff) >> 8) + offsetX + horizon;
  y = ((y + 0x7fff) >> 8) + offsetY - 0x1000;
  affine->y = y;
  groundEndLine = (Iwram_RatioMulQ14((s16)affine->pd, 0x4000 - y) >> 16) + 1;
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
