// Draft, differ 66 of 185 (no device): summon 25's screen wipe (called at
// its start with a midpoint callback). Written from the listing; no Broken
// Seal twin (twins12 0.46).
// The native stack frame is 140 bytes: work at sp+4, midpoint at sp+8, the ramp at
// sp+12 and a word at sp+0 that nothing reads; here the stack frame is 136 (work
// sp+0, midpoint sp+4), which shifts every spill (about 10 rows).
// Left: in the band loop the native keeps y in r5, x in r6, the row offset
// in r0 and copies the falling edge's giv (r7, 2y - 192) into r4 each row;
// it computes y * 2 inside the inner loop. Here y, x and the row offset take
// r4, r0 and r6, and y * 2 is computed once per row (shared with fall).
// After the push the IO queue entry's address is formed before the count
// increment (2 rows).
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
void Runtime_ReleaseHeapBlock(s32 slot);
void WaitFrames(s32 frames);

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
  s32 frames;
  s32 half;
  s32 x;
  s32 y;
  s32 i;

  scrollX = (side & 1) * 192 - 64;
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  BattleEffect_LoadBlitter(104, 3);
  work->transferMode = 0;
  work->transferValue = 75;
  REG_BG2PA = 0x48;

  // The band: colours 0 to 62 rising over the first 32 rows, 63 through the
  // middle 64 and falling again over the last 32, in 8x8 256-colour tiles.
  for (y = 0; y != 128; y++) {
    s32 row = (y & 7) + (y >> 3) * 64;
    s32 fall = y * 2 - 192;

    for (x = 0; x != 128; x++) {
      s32 color;

      color = y * 2;
      if (y > 31) {
        color = 63 - fall;
        if (y <= 95)
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
  frames = 192 / ((s8)speed < 0 ? -(s8)speed : (s8)speed) + 1;
  half = (frames - 1) >> 1;
  for (i = 0; i != frames; i++) {
    scrollX += (s8)speed;
    REG_BG2X = scrollX << 8;
    work->transferPending = 1;
    WaitFrames(1);
    if (i == half && midpoint != NULL)
      midpoint();
  }
  Runtime_ReleaseHeapBlock(104);
}
