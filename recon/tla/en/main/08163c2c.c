// Draft, differ 2 of 170 under -mthumb-slow-loads (2026-10-25): The Broken
// Seal's Func_08163c2c with The Lost Age's third mode (3 wipes nothing)
// and its transfer flag at 0x78e8 (BattleFxWork).
//
// Left: one row. The native stores the value argument (str r1, [sp, #4])
// before it starts the RAM base (movs r3, #192); here the base first. An
// empty one-pass block or an empty asm before the heap reads stores it
// first but also moves the mode's copy (adds r6, r0, #0) up from after
// add r3, sp, #136 (differ 2 either way); the old one-pass block around the
// canvas read differs 4 now and is gone. Tried as well (differ): the heap
// reads in the other order (12), mode or value unsigned (2), value as u8
// (4), the work or both reads in a one-pass block (6, 8), alchemy permute 5
// minutes, 2 jobs (nothing below 2).
#include "types.h"
#include "ram_buffer.h"
#include "system.h"
#include "battle_fx_work.h"

void Func_08163c2c(s32 mode, s32 value) {
  struct BattleFxWork *work;
  u8 *canvas;
  u8 delay[128];
  s32 x;
  s32 y;
  s32 pos;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
  for (x = 0; x != 128; x++)
    delay[x] = Random16() & 0x3f;

  if (mode == 1) {
    s32 front;
    s32 speed;

    front = 0;
    speed = 1;
    x = 0;
    do {
      front += speed;
      speed++;
      for (; x != front; x++) {
        for (y = 0; y != 128; y++) {
          pos = x - delay[y];
          if (pos >= 0)
            if (pos <= 127)
              canvas[(((y / 8) * 16 + pos / 8) * 8 + (y & 7)) * 8 + (pos & 7)] =
                  1 - value;
        }
      }
      work->transferPending = 1;
      WaitFrames(1);
    } while (front <= 256);
  } else if (mode != 3) {
    s32 front;
    s32 speed;

    front = 0;
    speed = 1;
    y = 0;
    do {
      front += speed / 2;
      speed += 4;
      for (; y != front; y++) {
        for (x = 0; x != 128; x++) {
          pos = y - delay[x];
          if (pos >= 0)
            if (pos <= 127)
              canvas[(((pos / 8) * 16 + x / 8) * 8 + (pos & 7)) * 8 + (x & 7)] =
                  1 - value;
        }
      }
      work->transferPending = 1;
      WaitFrames(1);
    } while (front <= 191);
  }
}
