#include "types.h"
#include "field_event.h"
#include "event_inline.h"

extern s16 gData_020023f2;
extern struct FieldActor *gData_020023f4[];
extern s16 gData_020024f6;

void Func_02000fd8(s32 id, s32 value) {
  struct FieldActor *object = ((struct FieldActor *)Actor_Get(id));

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_020023f2;
    gData_020023f4[*count] = object;
    (*count)++;
  }
}

void Func_02001008(s32 value) { gData_020024f6 = value; }
