#include "types.h"
#include "ram_buffer.h"

u32 Stream_ReadU16(u8 **cursor);

s32 Func_080ca9cc(s32 id, s32 *position) {
  u8 *cursor = Ram_Data_0202e008;
  s8 key;

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
