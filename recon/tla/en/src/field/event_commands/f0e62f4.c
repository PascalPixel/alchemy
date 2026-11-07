#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"

void Func_080e62f4(u8 *source, u8 *destination, s32 phase) {
  s32 width = 32;
  s32 height;
  s32 column;
  s32 row;

  Iwram_ClearWords(destination, 32 * 32);
  // Set after the clear, which keeps it out of the call's registers.
  height = 32;
  for (column = 0; column < width; column++) {
    s32 wave = (Trig_Sin(column << 13) << 3) + Q16_ONE * 8;

    for (row = 0; row < height; row++) {
      s32 position = phase * 0x19999 - Q16_ONE * 124 + row * (Q16_ONE * 4);
      // In two steps: the wave subtracted, then the shift.
      s32 shift = position - wave;
      shift >>= 16;

      if (shift >= 0 && shift < height)
        destination[(shift << 5) + column] = source[row * width + column];
    }
  }
}
