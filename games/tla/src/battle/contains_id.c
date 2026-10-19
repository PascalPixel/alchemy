/// @file
/// Battle: search a unit ID list.

#include "types.h"

/// Return 1 if the unit at this formation position is in the list.
/// list: unit IDs, terminated by 0xff, at most 14 entries.
/// no: formation position (0-7 allies, 8-13 enemies).
s32 BattlePlacement_ContainsId(s16 *list, s32 no) {
  u32 i;

  if (no > 7)
    no += 120; // Enemy unit IDs start at 128.
  i = 0;
  do {
    u16 id = *list++;

    if (id == 0xff)
      break;
    if (id == no)
      return 1;
  } while (++i < 14);
  return 0;
}
