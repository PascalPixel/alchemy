#include "types.h"

u32 Stream_ReadU16(u8 **cursor);

s32 Func_080ca9cc(s32 id, s32 *position) {
  u8 *cursor = FIXED_ADDRESS((u8 *), 0x0202e008, gData_0202e008);
  s8 key;

  /* FAKEMATCH(volatile): the key read through a volatile pointer, a plain
   * byte load that waits after the cursor's store; as key = *cursor++, 4 of
   * 43 instructions differ in tla-en. */
  key = *(volatile u8 *)cursor++;
  while (key != -1) {
    if (key == id) {
      position[0] = Stream_ReadU16(&cursor) << 16;
      position[2] = Stream_ReadU16(&cursor) << 16;
      return 0;
    }
    cursor += 4;
    /* FAKEMATCH(volatile): the key read through a volatile pointer, a plain
     * byte load that waits after the cursor's store; as key = *cursor++, 4 of
     * 43 instructions differ in tla-en. */
    key = *(volatile u8 *)cursor++;
  }
  return -1;
}
