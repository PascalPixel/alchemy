/* Draft, differ 99 of 120 (draft 117 instructions, no device): summon 25's
 * falling drop with its trail, written from the listing. The native frame
 * (28 bytes) holds x at sp+24, the blitter pair at sp+16/20 (only the first
 * is called; the second is read and stored, as in The Broken Seal's
 * BlitterPair), and the trail's size - 1 and size * 2 at sp+8/12.
 * Left: in the trail loop the native reloads the table entry (the table is
 * extern without const, so the call may change it: kept), recomputes
 * frame / 2 * 770 and x - half and recomputes y + i * half with a multiply
 * on every pass; here loop.c hoists the product and the offset and keeps
 * fewer spills (x stays in r8).
 * Tried: s32 size and half (differ 128; the native shifts them unsigned),
 * the table const (128); alchemy permute 6 minutes: best 92 (a different
 * register split, not kept). */
#include "types.h"
#include "ram_buffer.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The particle sheet's cell offsets by size (The Broken Seal's
// gParticleStreams_CellOffsets).
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
  if (small == 0)
    draw[0](canvas, work->sheet + 0x1b12 + frame / 2 * (20 * 40), x - 10,
            y - 20, 20, 40);
  else
    draw[0](canvas, work->sheet + 0x4c12 + frame / 2 * (12 * 24), x - 6,
            y - 12, 12, 24);

  // The trail: particles of size 8 (6 for a small drop), half a size apart.
  size = 8 - small * 2;
  half = size / 2;
  for (i = 1; (trailY = y + i * half) <= 112; i++)
    draw[0](canvas,
            work->sheet + gData_08197410[size - 1] + frame / 2 * 0x302,
            x - half, trailY, size, size * 2);
}
