// Differ 12 of 185 (tla-en, 2026-11-08, -mtune=gba). Rechecked 8 November: the
// word declared elsewhere, as u8 or s32[1] (12 to 18); midpoint a volatile
// parameter (51) or a volatile local copy (42 to 48), which put it above the
// ramp; the row inline, set late or in its own scope (12 to 14); the outer
// test i <= 127 (16); the falling edge 63 - (i * 2 - 192) (23); permuter, 9
// minutes: none better. The native word at sp+0 looks like a spill slot
// allocated after work's, which a declared local cannot give. Left: midpoint at sp+0
// here, sp+8 in the listing (2 rows), and, in the band loop, the row offset and
// the falling edge's per-row copy in each other's registers (r4, r0). This
// round: x reused as the frame count (34 to 26; ordinary C, a local reused for
// a second value), the row as (i & 7) + (i >> 3) * 64 (to 28 alone, 22 with
// x), and an unused volatile word declared last, which gives the frame its
// 140 bytes and puts the ramp at sp+12 (20); it sits above the spills, where
// the listing's word is below them. Then midpoint tested as *&midpoint (an
// equivalent spelling; found by the permuter as a pointer to it), which gives
// midpoint a stack slot of its own and work its sp+4 (12; 18 without the
// volatile word; the word in a block after the address is taken, 12). Not
// moving the swap: the row hoisted to
// the outer loop (30), declared at the top, set late or in a block (20), the
// colour at the top (20) or as an else-if chain (26), the falling edge spelt
// 63 - (i * 2 - 192), 255 - (i << 1) and others (33), a block-scoped column
// counter (30); a copy of midpoint called through it (24 to 60).
// Earlier: differ 34 of 185 (tla-en, 2026-11-08, -mtune=gba).
// Differ 34 of 185 (tla-en, 2026-11-06).
// The frame (native 140, here 136: every spill one slot lower, 16 rows) and,
// in the band loop, the row offset and the falling edge in each other's
// registers. 2026-11-06: the row offset declared in the inner loop (36 to
// 34). x reused as the frame count (a temporary) gives 28, only the frame and
// one register swap left; ramp[66] sizes the frame (24) but misstates it.
// 2026-11-06: the colour as if (i <= 31) i * 2, else 63 - ((i - 96) << 1)
// unless i <= 95. With y * 2 on one arm only, gcse no longer carries it out of
// the inner loop (ifcvt still sets it first, as the native does), and the
// falling edge written inline is hoisted by loop.c, so the outer giv keeps
// the native's per-row copy (r4 = r7). Row as (i >> 3) * 64 + (i & 7) (36,
// against 42 the other way round). Not moving x and row: u32 x or row,
// declaration order or scope, (canvas + row)[...], the index's terms
// reordered (36 to 75), x < 128 (38). An unused s32 pad[1] does not size the
// frame; the extra word below the spills looks like a third spill slot.
// 2026-11-06, second round: a local copy of midpoint (void (*callback)(void)
// = midpoint, called through it) gives the native's 140-byte frame and every
// spill offset (32): the copy takes the lowest slot, as the native's unread
// word. But the copy is stored there and read for the call, where the native
// reads midpoint's own slot; testing the copy and calling midpoint lets CSE
// merge them (136 again). The third slot is a pseudo numbered after work,
// spilled, whose loads and stores all disappear.
// Earlier, 2026-11-05 (tla-en): differ 48 of 185, 34 with slots and pool
// offsets normalized. A loop more takes a counter of its own.
// Earlier draft, differ 66 of 185 (no device): summon 25's screen wipe (called at
// its start with a midpoint callback). Written from the listing; no Broken
// Seal twin (twins12 0.46).
// The native stack frame is 140 bytes: work at sp+4, midpoint at sp+8, the ramp at
// sp+12 and a word at sp+0 that nothing reads; here the stack frame is 136 (work
// sp+0, midpoint sp+4), which shifts every spill.
// Tried: the colour as an if/else chain or as the native's shape
// (color = y * 2; if (y > 31) { ...; if (y <= 95) color = 63; }) (129-131
// without the row local); the row offset as a local (109) and the tile
// index in shifts (105); the falling edge as 63 - (y - 96) * 2,
// 63 - ((y << 1) - 192) or 63 + (192 - y * 2) (all fold to 255 - 2y, 105),
// as 63 - ((y - 96) << 1) (123), through a local fall = y * 2 - 192 (83);
// frames as 192 / |speed| + 1 with half = (frames - 1) >> 1 (66, the
// native's order of the two).
// 2026-10-19: the native recomputes y * 2 in the inner loop, where agscc
// carries it out to the row: the inner loop as a goto loop (64), fall as a
// running -192 + 2y (66, and with the goto loop 66), y + y or y << 1 (66),
// the colour as an if/else chain (66, 70) and y held by an empty asm operand
// in the inner loop (94, 126) all leave it carried out.
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "battle_effect_work.h"

void BattleEffect_LoadBlitter(s32 slot, s32 value);
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
  volatile s32 unused; // Unused: gives the stack frame its 140 bytes.

  scrollX = (side & 1) * 192 - 64;
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  BattleEffect_LoadBlitter(104, 3);
  work->transferMode = 0;
  work->transferValue = 75;
  REG_BG2PA = 0x48;

  // The band: colours 0 to 62 rising over the first 32 rows, 63 through the
  // middle 64 and falling again over the last 32, in 8x8 256-colour tiles.
  for (i = 0; i != 128; i++) {
    for (x = 0; x != 128; x++) {
      s32 row = (i & 7) + (i >> 3) * 64;
      s32 color;

      if (i <= 31)
        color = i * 2;
      else {
        color = 63 - ((i - 96) << 1);
        if (i <= 95)
          color = 63;
      }
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
  x = 192 / ((s8)speed < 0 ? -(s8)speed : (s8)speed) + 1;
  half = (x - 1) >> 1;
  for (i = 0; i != x; i++) {
    scrollX += (s8)speed;
    REG_BG2X = scrollX << 8;
    work->transferPending = 1;
    wait(1);
    // Spelt *&midpoint: the address puts midpoint in its own stack slot.
    if (i == half && *&midpoint != NULL)
      midpoint();
  }
  HeapSlot_Release(104);
}
