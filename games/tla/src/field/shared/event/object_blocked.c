/// @file
/// Whether a script object stands where it may not.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

/// The script objects (Ram_HeapSlots->scriptObjects).
#define SCRIPT_OBJECT_COUNT 64

/// The event work's mode in which the cell test is skipped.
#define EVENT_MODE_NO_CELL_TEST 3

s32 Func_08020308(s32 x, s32 z, s32 layer, s32 height);
s32 Runtime_CheckRadiusOverlapFar(s32 *a, s32 radiusA, s32 *b, s32 radiusB);

/// Says whether object is blocked: 1 when, outside event mode 3,
/// Func_08020308 reports its cell, ORed with 2 when it overlaps any other
/// collidable script object (each radius less 2 pixels).
s32 Func_080e4244(struct ObjectRuntime *object) {
  s32 result;
  struct ObjectRuntime *other;
  s32 i;

  s8 *work = Ram_HeapSlots->eventWork;
  result = 0;
  if (work[394] != EVENT_MODE_NO_CELL_TEST) {
    if (Func_08020308(object->x, object->z, 0, object->terrainHeight) != 0)
      result = 1;
  }
  other = Ram_HeapSlots->scriptObjects;
  for (i = SCRIPT_OBJECT_COUNT - 1; i >= 0; i--, other++) {
    if (other->script != NULL && (other->collisionFlags & 1) &&
        other != object) {
      if (Runtime_CheckRadiusOverlapFar(&object->x, object->radius - 2,
                                        &other->x, other->radius - 2) >= 0)
        result |= 2;
    }
  }
  return result;
}
