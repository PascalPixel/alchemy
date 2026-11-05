// Draft, differ 37 (tla-en, 2026-11-05). Func_020005a0 of overlay 6a0.
// Byte offsets stepping 4 from 0 and from 2 give the listing's shape. Left:
// the first read's ldrh with shifts, -1 in r6, and z's operand order:
// natively the giv sits first in the address.
#include "types.h"
#include "actor.h"

extern s16 gData_02002c64[];

s32 Func_020005a0(struct Actor *actor) {
  u8 *table = (u8 *)gData_02002c64;
  s32 x;
  s32 z;

  for (x = 0, z = 2; *(s16 *)(table + x) != -1; x += 4, z += 4) {
    if (*(s16 *)(table + x) == actor->x >> 20 && *(s16 *)(table + z) == actor->z >> 20)
      return 1;
  }
  return 0;
}
