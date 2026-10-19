#include "types.h"
#include "ram_buffer.h"

// Finds the tag (key, value) in the 4-byte entries at gMapCollision, or adds
// it at the first free entry (key 0xff), and returns its index. Func_080eabd0
// gives a raised cell's shape this index.
s32 Func_080eab98(s32 unused, s32 key, s32 value) {
  u8 *entry = Ram_MapCollision;
  s8 tagValue;
  s32 i = 0;

  /* FAKEMATCH(barrier): the value's narrowing kept after the table's address;
   * as an s8 parameter, 2 of 25 instructions differ in tla-en. */
  asm("" : "+l"(value) : "l"(entry));
  tagValue = value;
  while (1) {
    if (entry[0] == key) {
      if (*(s8 *)++entry == tagValue)
        return i;
      entry--;
    } else if (entry[0] == 0xff) {
      entry[0] = key;
      entry[1] = tagValue;
      return i;
    }
    entry += 4;
    i++;
  }
}
