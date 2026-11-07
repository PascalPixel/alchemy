#include "types.h"
#include "ram_buffer.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The particle sheet's cell offsets by size (The Broken Seal's
// gBattleEffect_MoteCells).
extern u16 gData_08197410[];

// Summon 25: draws one falling drop at (x, y), large (20x40) when small is 0
// and small (12x24) otherwise, its animation picture chosen by frame / 2, and
// under it a trail of particles every half size down to row 112.
void Func_08192648(u32 frame, s32 x, s32 y, s32 small) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  u32 size;
  u32 half;
  s32 i;
  s32 trailY;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The drop: one 20x40 or 12x24 cell of its strip in the sheet.
  if (small == 0)
    draw[1](canvas, work->sheet + 0x1b12 + frame / 2 * (20 * 40), x - 10,
            y - 20, 20, 40);
  else
    draw[1](canvas, work->sheet + 0x4c12 + frame / 2 * (12 * 24), x - 6, y - 12,
            12, 24);

  // The trail: particles of size 8 (6 for a small drop), half a size apart.
  size = 8 - small * 2;
  half = size / 2;
  // A goto loop: loop.c leaves it alone, so y + i * half multiplies on every
  // pass and the table entry and frame / 2 * 0x302 are recomputed; as a for
  // loop, 111 of 120 instructions differ in tla-en. The argument x - size / 2
  // is not one GCSE carries out of the loop (as x - half, 83 differ).
  i = 1;
  trailY = y + i * half;
  if (trailY > 112)
    return;
next:
  // The offset grouped before the sheet base: ungrouped, 2 instructions
  // differ (the base is added before the product).
  draw[0](canvas, work->sheet + (gData_08197410[size - 1] + frame / 2 * 0x302),
          x - size / 2, trailY, size, size * 2);
  i++;
  trailY = y + i * half;
  if (trailY <= 112)
    goto next;
}
