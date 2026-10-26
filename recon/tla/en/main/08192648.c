// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): differ 10 of
// 120; a greedy device search found nothing lower.
// Draft, differ 16 of 120 (no device): summon 25's
// falling drop with its trail, written from the listing. The native frame
// (28 bytes) holds x at sp+24, the blitter pair at sp+16/20 (only the first
// is called; the second is read and stored, as in The Broken Seal's
// BlitterPair), and the trail's size - 1 and size * 2 at sp+8/12.
// Earlier, with a for loop: in the trail loop the native reloads the table
// entry (the table is extern without const, so the call may change it:
// kept), recomputes frame / 2 * 770 and x - half and recomputes y + i *
// half with a multiply on every pass; there loop.c hoisted the product and
// the offset and kept fewer spills (x in r8).
// Tried: s32 size and half (differ 128; the native shifts them unsigned),
// the table const (128); alchemy permute 6 minutes: best 92 (a different
// register split, not kept).
// 2026-10-19: the trail as a goto loop (99 to 79): the native's trail loop is
// not one loop.c sees, so nothing is strength-reduced or hoisted (y + i *
// half multiplies on every pass, frame / 2 * 770 and the table entry are
// recomputed); its argument x - size / 2 rather than x - half (40: the
// expression is then not one GCSE carries out of the loop, and x stays
// spilled at sp+24); the large drop's y - 20 in a local before the call
// (16, a alchemy-devtools permute find). Left, all one-row orders: x's spill before
// the copy of small, the second blitter word loaded into r3 (r4 natively),
// the large drop's 20 and y - 20, the small drop's x reload, and the
// trail's sheet base and 0x700. alchemy-devtools permute (8 minutes twice, seeds 1
// and 7): nothing below 16; x held in memory by an asm operand: 32 to 34.
#include "types.h"
#include "ram_buffer.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The particle sheet's cell offsets by size (The Broken Seal's
// gBattleEffect_MoteCells).
extern u16 gData_08197410[];

// Summon 25: draws one falling drop at (x, y), large (20x40) when small is 0
// and small (12x24) otherwise, its animation frame chosen by frame / 2, and
// under it a trail of particles every half size down to row 112.
void Func_08192648(u32 frame, s32 x, s32 y, s32 small) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  u32 size;
  u32 half;
  s32 i;
  s32 trailY;

  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The drop: one 20x40 or 12x24 cell of its strip in the sheet.
  if (small == 0) {
    s32 top = y - 20;

    draw[0](canvas, work->sheet + 0x1b12 + frame / 2 * (20 * 40), x - 10, top,
            20, 40);
  } else
    draw[0](canvas, work->sheet + 0x4c12 + frame / 2 * (12 * 24), x - 6, y - 12,
            12, 24);

  // The trail: particles of size 8 (6 for a small drop), half a size apart.
  size = 8 - small * 2;
  half = size / 2;
  i = 1;
  trailY = y + i * half;
  if (trailY > 112)
    return;
next:
  draw[0](canvas, work->sheet + gData_08197410[size - 1] + frame / 2 * 0x302,
          x - size / 2, trailY, size, size * 2);
  i++;
  trailY = y + i * half;
  if (trailY <= 112)
    goto next;
}
