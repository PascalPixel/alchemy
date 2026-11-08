// Matches: differ 0 (tla-en, 2026-11-08)
// One tagged device, FAKEMATCH(dead). The native stack frame is the plain C's
// 136 bytes plus an outgoing-argument word at sp+0 that nothing writes or
// reads, so every slot sits 4 bytes higher (work sp+4, midpoint sp+8, the ramp
// sp+12). The build accumulates outgoing arguments, so a call with a fifth
// argument reserves that word; a never-taken call to the five-argument
// BattleEffect_BuildBlitterFar gives it, as in battle/presentation/f127068.c.
// Without the call, differ 18 (frame 136). It replaces the volatile word and
// the *&midpoint spelling (differ 4: midpoint's slot at sp+0, because a word
// declared in C is placed at expansion, above every spill).
// The band loop's last differences were loop.c's order of hoisting: i & 7 at
// the top of the inner loop and the row set after the colour (12 to 4). The 7
// is hoisted before the 63; the falling edge's per-row copy is set before the
// row, which then takes r0; and the 63 is moved only in loop.c's second pass,
// after the falling edge's start value -192. loop.c's threshold (30 here) drops
// by 3 for each move, so with four moves before it the 63 is not worth moving
// the first time. Not better: i & 7 and i >> 3 in the outer loop (6 to 16),
// the row at the top of the inner loop (12), the row inline (2, operands
// swapped), a fall local at the top (10, the giv becomes i - 96), else-if
// chains (6, 12), a frames local instead of reusing x (18).
// Editions: tla-ja (at 0819195c), tla-de, tla-es, tla-fr and tla-it hold the
// same bytes but for bl offsets the linker fills (ja: the function 4 bytes
// later; de, es, fr, it: wait at 0801358c); diff cannot measure their scaffold
// sections, which carry no Thumb marker.
// Earlier: differ 66, 48, 34, 12 of 185 (2026-10-19 to 2026-11-08); written
// from the listing, no Broken Seal twin (twins12 0.46). Summon 25's screen
// wipe, called at its start with a midpoint callback.
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "battle_effect_work.h"

s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);
// Declared as The Broken Seal's five-argument blitter builder, which this game
// does not link; it serves only the never-taken call below.
s32 BattleEffect_BuildBlitterFar(s32 slot, s32 widthShift, s32 heightShift,
                                 s32 flags, u32 mode);
void HeapSlot_Release(s32 slot);
void wait(s32 frames);

// Summon 25's screen wipe: a soft-edged grey band, drawn once into the
// canvas, slides across the screen on BG2 at speed pixels per frame, from
// the left or (when the low bit of side is set) from the right. Halfway
// through, while the band covers the screen, it calls midpoint to change
// the scene behind it.
void Func_08191958(u8 side, u8 speed, void (*midpoint)(void)) {
  s32 scrollX;
  u8 *canvas;
  struct BattleEffectWork *work;
  u16 ramp[63];
  s32 half;
  s32 x;
  s32 i;

  scrollX = (side & 1) * 192 - 64;
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  /* FAKEMATCH(dead): a never-taken five-argument call reserves the stack
   * frame's outgoing argument word at sp+0; as no call, 18 of 185
   * instructions differ in tla-en. */
  if (0)
    BattleEffect_BuildBlitterFar(104, 7, 7, 3, 3);
  BattleEffect_LoadBlitter(104, 3);
  work->transferMode = 0;
  work->transferValue = 75;
  REG_BG2PA = 0x48;

  // The band: colours 0 to 62 rising over the first 32 rows, 63 through the
  // middle 64 and falling again over the last 32, in 8x8 256-colour tiles.
  for (i = 0; i != 128; i++) {
    for (x = 0; x != 128; x++) {
      s32 line = i & 7;
      s32 color;
      s32 row;

      if (i <= 31)
        color = i * 2;
      else {
        color = 63 - ((i - 96) << 1);
        if (i <= 95)
          color = 63;
      }
      row = line + (i >> 3) * 64;
      canvas[row + ((x & 7) << 3) + ((x >> 3) << 10)] = color;
    }
  }

  // Colours 1 to 63: a grey ramp from black to half white.
  for (i = 0; i != 63; i++) {
    s32 level = i >> 1;

    ramp[i] = level << 10 | level << 5 | level;
  }
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT[1], (u32)ramp, 0x8000003f);

  REG_BG2X = scrollX << 8;
  // x, done with the band, now counts the slide's frames.
  x = 192 / ((s8)speed < 0 ? -(s8)speed : (s8)speed) + 1;
  half = (x - 1) >> 1;
  for (i = 0; i != x; i++) {
    scrollX += (s8)speed;
    REG_BG2X = scrollX << 8;
    work->transferPending = 1;
    wait(1);
    if (i == half && midpoint != NULL)
      midpoint();
  }
  HeapSlot_Release(104);
}
