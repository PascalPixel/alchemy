// Draft, differ 43 of 33: the native walks the cell table with two byte offsets
// (x at 4i in r1, z at 4i + 2 in r0, both stepped by 4), reloads the object's x
// and z and rebuilds -1 on every pass, and keeps the table base in r4; this
// loop hoists the object's cells and the -1. Tried: the table read as s16 pairs
// (49), a goto loop over explicit byte offsets (51, the first test merges with
// the entry check), a while loop (43).
#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_7/jupiter_lighthouse_room_7.h"

struct Cell {
  s16 x;
  s16 z;
};

s32 Func_020005a0(struct ObjectRuntime *object) {
  struct Cell *cells = (struct Cell *)gData_02002c64;
  s32 i;

  for (i = 0; cells[i].x != -1; i++) {
    if (cells[i].x == object->x >> 20 && cells[i].z == object->z >> 20)
      return 1;
  }
  return 0;
}
