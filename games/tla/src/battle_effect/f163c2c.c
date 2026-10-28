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
      wait(1);
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
      wait(1);
    } while (front <= 191);
  }
}
