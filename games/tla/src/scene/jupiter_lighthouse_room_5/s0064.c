#include "types.h"
#include "field_event.h"
#include "event_inline.h"

void Func_02000e04(s32 id);
void Func_020029d8(void);

void Func_02000064(s32 unused, s32 id) {
  struct FieldActor *object;
  s32 i;

  object = ((struct FieldActor *)Actor_Get(id));
  if (id == 11) {
    if (!Event_GetFlag(0xa00) || object->x >> 20 != 13 || object->z >> 20 != 34)
      return;
    object->motionFlags = 0;
    for (i = 15; i >= 0; i--) {
      object->y += 0x20000;
      System_Wait(1);
    }
    Func_02000e04(id);
    Event_SetFlag(0xa09);
  } else {
    Func_020029d8();
  }
}
