/* Draft, differ 8 of 170: The Broken Seal's BattleEffect_WipeCanvas with
 * The Lost Age's third mode (3 wipes nothing) and its transfer flag at
 * 0x78e8 (BattleFxWork). The one-pass block around the canvas read fixes
 * the prologue's value store and the delay array's address (add r2, sp,
 * #8), but moves the mode copy (adds r6, r0, #0) and the canvas copy (mov
 * r9, r1) up to it; the native has them after add r3, sp, #136. Left too:
 * at each frame's end the native reloads the work (ldr r1, [sp]) before it
 * starts the flag's offset 0x78e8; here the offset first (twice). That
 * reload and the offset are both reloads for one add: the offset's three
 * rows outrank the load in sched2 (prio 8 against 7). Tried: a one-pass
 * block or an input asm around the store (9, 58), the work copied through
 * an asm (58), the offset held in an asm (93), the work or both loads in a
 * block (10 to 12), a pointer for the delay loop (51), alchemy permute 7
 * minutes (8 at best, this block). */
#include "types.h"
#include "ram_buffer.h"
#include "system.h"
#include "battle_fx_work.h"

void BattleEffect_WipeCanvas(s32 mode, s32 value) {
  struct BattleFxWork *work;
  u8 *canvas;
  u8 delay[128];
  s32 x;
  s32 y;
  s32 pos;

  /* FAKEMATCH: the one-pass block stores the value argument before 0x03000000
   * is started and builds the delay array's address in one add (add r2, sp,
   * #8), as the native does (differ 9 without it). */
  do {
    canvas = Ram_HeapSlots->battleFxWork[1];
  } while (0);
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
