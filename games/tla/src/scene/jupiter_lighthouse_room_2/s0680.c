#include "types.h"
#include "field_event.h"
#include "event_inline.h"

void Func_02000b28(s32 id);

void Func_02000680(s32 unused, s32 id) {
  struct FieldActor *object = ((struct FieldActor *)Actor_Get(id));
  s32 i;

  if (Event_GetFlag(0xa00) && object->x >> 20 == 18 && object->z >> 20 == 6) {
    object->motionFlags = 0;
    for (i = 15; i >= 0; i--) {
      object->y += 0x20000;
      System_Wait(1);
    }
    Func_02000b28(id);
    Event_SetFlag(0xa03);
  }
}
