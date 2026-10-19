// Draft, differ 6 of 43: the native stores the cursor to its stack slot before
// the first key read (str r3, [sp]; ldrb r2, [r3]); flow drops that store as
// dead unless something stands between it and the step's store, and the
// empty asm below keeps it (differ 11 without it; a one-pass block there
// differs 7). What remains: the asm lets the copy of position (adds r6, r1)
// move ahead of the store, and in the loop the native reads the key before
// stepping past it (ldrb r2, [r2]; adds r3, #5), agscc after; a one-pass
// block or the read split from the step there changes nothing. A for loop
// is the same; a reader inline (s8 Stream_ReadS8(u8 **)) sign-extends twice
// (differ 37), a u8 one changes nothing.
#include "types.h"

extern u8 gData_0202e008[];

u32 Stream_ReadU16(u8 **cursor);

s32 Func_080ca9cc(s32 id, s32 *position) {
  u8 *cursor = gData_0202e008;
  s8 key;

  asm volatile("");
  key = *cursor++;

  while (key != -1) {
    if (key == id) {
      position[0] = Stream_ReadU16(&cursor) << 16;
      position[2] = Stream_ReadU16(&cursor) << 16;
      return 0;
    }
    cursor += 4;
    key = *cursor++;
  }
  return -1;
}
