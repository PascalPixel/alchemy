// Near miss, differ 2 of 70 (was 4 with the event byte read inline): the
// native builds the heap slots address before it zeroes the result; here the
// zero comes first. Empty asm barriers around the zeroing measured 6 and 8.
// Returns 1 when the event mode is not 3 and Func_08020308 reports the
// object's cell, ORed with 2 when it overlaps any other collidable script
// object.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

extern u8 gPartyState[];

s32 Func_08020308(s32 x, s32 z, s32 layer, s32 height);
s32 Runtime_CheckRadiusOverlapFar(s32 *a, s32 radius_a, s32 *b, s32 radius_b);
s32 Func_080dbcd8(s32 x, s32 z, s32 layer);
s32 Func_080dbd48(s32 x, s32 z, s32 layer);

s32 Func_080e4244(struct ObjectRuntime *object) {
  s32 result;
  struct ObjectRuntime *other;
  s32 i;

  s8 *work = Ram_HeapSlots->eventWork;
  result = 0;
  if (work[394] != 3) {
    if (Func_08020308(object->x, object->z, 0, object->terrainHeight) != 0)
      result = 1;
  }
  other = Ram_HeapSlots->scriptObjects;
  for (i = 63; i >= 0; i--, other++) {
    if (other->script != NULL && (other->collisionFlags & 1) &&
        other != object) {
      if (Runtime_CheckRadiusOverlapFar(&object->x, object->radius - 2,
                                        &other->x, other->radius - 2) >= 0)
        result |= 2;
    }
  }
  return result;
}
