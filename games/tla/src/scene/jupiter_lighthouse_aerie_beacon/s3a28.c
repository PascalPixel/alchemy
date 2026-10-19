#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "event_work.h"

/// Writes to out the ids of objects 0 to 63 standing in columns 4 to 8 and in
/// the three rows from 72 less the row of z. Nothing in this overlay calls it.
// As ☀️'s MercuryLighthouseAerie_CollectBandSlots does.
void Func_02003a28(s32 *out, s32 zArg) {
  /* FAKEMATCH: four pins (z in r4, its shifted value in r2, the 64 in r3 and
   * the column in r3) give the native's allocation; as plain locals z goes to
   * ip, the 64 takes r4 and the column and row swap r2 and r3 (differ 9 to 40).
   */
  register s32 z asm("r4") = zArg;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  u32 slot;

  {
    register s32 shifted asm("r2") = z >> 20;
    register s32 top asm("r3") = 64;

    z = top - shifted;
  }
  for (slot = 0; slot <= 63; slot++) {
    struct ObjectRuntime *object = work->objects[slot];

    if (object != NULL) {
      register s32 column asm("r3") = object->x >> 20;
      s32 row = object->z >> 20;

      if (column >= 4 && column <= 8 && z + 8 <= row && row < z + 11)
        *out++ = slot;
    }
  }
}
