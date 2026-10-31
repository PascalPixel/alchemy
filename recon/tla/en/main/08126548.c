// Near miss, differ 2 of 199 (tla-en, 2026-10-31): scheduling only, the
// native spills the parameters before it shifts 0x03000000 into the heap base
// (the known store-before-build pattern): sched2 ranks the shift (priority 8,
// through the view load and the r2 reuse for 0x800) above the three spills
// (priority 7), and nothing in the C moves either. 2026-10-31: 4 measured
// with no source change; Q16_ONE held in ratio from the scale test to the
// Iwram_RatioMulQ14 call (a tagged temporary) lets reload rebuild it after
// scale is copied, as natively: 4 -> 2. Round 5 (2026-10-31), still 2: the
// shift's priority 8 runs view load -> r2's 0x800 -> horizon's store -> r2's
// 0x10000 -> the scale branch; the stores reach 7 through the view and
// scanlines loads. A barrier cannot place it, as the native schedules the
// scale load (before the heap base in RTL) after both builds. Measured: a
// heap pointer local, horizon and wrap set at their declarations, ratio set
// before or between them, centerX copied to a local (2); scanlines read
// first (plain 4), camera first (58), view last (91); wrap set first or
// declared first (18); wrap s32 or u32 (45); the scale test with an else
// (43); camera read just before the test (28, 30); any parameter volatile
// (75 to 133); 22,386 permuter candidates, none below 2.
// Draft, differ 6 of 199 (2026-10-26; 55 before): ☀️'s BattleCamera_SetRange
// (games/tbs/src/battle/battle/background.c), ported: builds the next affine
// background page from the view centre, offsets and zoom, dividing scanlines
// into sky, ground and hidden rows before swapping pages. ⚓️ reads the ground
// ratio's step and y back from the affine page (ldrsh pd; 0x4000 - affine->y,
// which keeps y in r0 as natively: 55 -> 6). Left, scheduling only: the native
// spills the four parameters before it shifts 0x03000000 into the heap base
// (an empty asm first measured 12).
// horizon declared before wrap puts them in the native stack slots.
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
     Iwram_RatioMulQ14 call; as the constant in both, 4 of 199 instructions
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
  affine->x = ((x + 0x7fff) >> 8) + offsetX + horizon;
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
