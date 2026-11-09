#include "types.h"
#include "field_event.h"
#include "call.h"
#include "event_inline.h"

void Func_02004774();

void Func_0200039c(s32 unused, s32 id) {
  struct FieldActor *object = (struct FieldActor *)Actor_Get(id);

  Call4(Func_02004774, object->x, object->z, 0, -8);
}
