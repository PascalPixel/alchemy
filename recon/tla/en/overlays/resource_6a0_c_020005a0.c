// Draft, differ 37 (tla-en, 2026-11-05). Func_020005a0 of overlay 6a0.
// Byte offsets stepping 4 from 0 and from 2 give the listing's shape. Left:
// the first read's ldrh with shifts, -1 in r6, and z's operand order:
// natively the giv sits first in the address.
#include "types.h"
#include "actor.h"

extern s16 gData_02002c64[][2];

s32 Func_020005a0(struct Actor *actor) {
  s32 i;

  for (i = 0; gData_02002c64[i][0] != -1; i++) {
    if (gData_02002c64[i][0] == actor->x >> 20 &&
        gData_02002c64[i][1] == actor->z >> 20)
      return 1;
  }
  return 0;
}
