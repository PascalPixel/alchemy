/// @file
/// One routine of the canvas layer, not yet understood.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

/// Wipes the whole canvas to one colour with a ragged edge: each row (or
/// column) lags a random 0 to 63 pixels behind the front, which speeds up as
/// it goes. Returns when the wipe is done, a frame at a time: 23 frames
/// across, 15 down.
///
/// Called by the summons and effects that open on a wiped canvas: Boreas,
/// Thor, Atalanta, Neptune, Kirin, Ramses, the particle streams and
/// Judgment. Writes the canvas and sets work->transferPending each frame;
/// the caller's transfer task must already be running.
///
/// direction: CANVAS_WIPE_ACROSS sweeps left to right, anything else top to
///            bottom (enum CanvasWipeDirection).
/// fill: what the wipe leaves (enum CanvasWipeFill).
void BattleEffect_WipeCanvas(s32 direction, s32 fill) {
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

  if (direction == 1) {
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
                  1 - fill;
        }
      }
      work->transferPending = 1;
      wait(1);
    } while (front <= 256);
  } else if (direction != 3) {
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
                  1 - fill;
        }
      }
      work->transferPending = 1;
      wait(1);
    } while (front <= 191);
  }
}
#undef HEAP_BLITTER
#undef HEAP_BLITTER_ALTERNATE
#undef BLITTER
#undef SPARK_COUNT
#undef IWRAM
#undef PLTT
#undef SPARKS
#undef PHASE_RESULTS
