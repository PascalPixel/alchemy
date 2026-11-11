#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_work.h"

/// Writes to out the ids of objects 0 to 63 standing in columns 4 to 8 and in
/// the three rows from 72 less the row of z. Nothing in this overlay calls it.
// As ☀️'s MercuryLighthouseAerie_CollectBandSlots does.
void Func_02003a28(s32 *out, s32 zArg) {
  s32 z = zArg;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  u32 slot;

  z = 64 - (z >> 20);
  for (slot = 0; slot <= 63; slot++) {
    struct Actor *object = work->objects[slot];

    if (object != NULL) {
      s32 column = object->x >> 20;
      s32 row = object->z >> 20;

      if (column >= 4 && column <= 8 && z + 8 <= row && row < z + 11)
        *out++ = slot;
    }
  }
}
