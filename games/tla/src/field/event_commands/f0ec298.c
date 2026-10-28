#include "types.h"

extern s16 gData_0202a642;

/// Returns entry[1]'s high nibble when gData_0202a642 is set, else its low
/// nibble.
u32 Func_080ec298(u8 *entry) {
  if (gData_0202a642 != 0)
    return entry[1] >> 4;
  return entry[1] & 15;
}
