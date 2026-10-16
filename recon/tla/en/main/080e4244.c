// Near miss: listing 70 instructions, draft 70, common 68, differ 4; the rest
// differs in instruction order.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

extern u8 gPartyState[];

s32 Func_08020308(s32 x, s32 z, s32 layer, s32 height);
s32 Runtime_CheckRadiusOverlapFar(s32 *a, s32 radius_a, s32 *b, s32 radius_b);
s32 Func_080dbcd8(s32 x, s32 z, s32 layer);
s32 Func_080dbd48(s32 x, s32 z, s32 layer);

s32 Func_080e4244(struct ObjectRuntime *object) {
  s32 result = 0;
  struct ObjectRuntime *other;
  s32 i;

  if (*((s8 *)Ram_HeapSlots->eventWork + 394) != 3) {
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
