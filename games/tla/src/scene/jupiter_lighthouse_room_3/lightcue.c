#include "types.h"
#include "field_event.h"
#include "event_inline.h"

extern s16 gData_02002d2e;
extern struct FieldActor *gData_02002d30[];
extern s16 gData_02002e32;

void Func_020016cc(s32 id, s32 value) {
  struct FieldActor *object = ((struct FieldActor *)Actor_Get(id));

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_02002d2e;
    gData_02002d30[*count] = object;
    (*count)++;
  }
}

void Func_020016fc(s32 value) { gData_02002e32 = value; }
