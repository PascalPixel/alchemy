#include "types.h"

extern const s32 gData_080f3228[];

/// Looks key up in gData_080f3228's pairs and returns its value, or -1.
s32 Func_080d1ee8(s32 key) {
  const s32 *entry = gData_080f3228;
  s32 value;

  while (1) {
    value = entry[0];
    if (value == -1)
      return value;
    if (value == key)
      return entry[1];
    entry += 2;
  }
}
