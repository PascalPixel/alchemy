// Near miss under -mthumb-slow-loads, differ 2 of 43 (2026-10-25, the Makefile's
// options): the native copies the camera pointer back (mov r2, r8) right after
// the second __divsi3 call; this compiles it after the quotient's shift and
// store (lsls r0, r0, #16; str r0, [r6, #16]). Without the option it matched
// and was adopted in all six editions (1c5de30c6f).
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

s32 EventRuntime_GetControlledOwner(void);

/// Moves the controlled object to (x, z) converted by fixed scale factors
/// (x / 853, z / 640 after offsets) and puts the camera on it.
void Func_080ec16c(s32 x, s32 z) {
  s32 owner = EventRuntime_GetControlledOwner();
  s32 *camera = *(s32 **)Ram_HeapSlots->mapWork;
  struct ObjectRuntime *object = ObjectTable_Get(owner);

  if (object != NULL) {
    x >>= 16;
    x = ((x << 14) + 0x434000) / 853;
    z >>= 16;
    object->x = x << 16;
    object->z = (((z << 14) + 0x1c0000) / 640) << 16;
    camera[0] = object->x;
    camera[2] = object->z;
  }
}
