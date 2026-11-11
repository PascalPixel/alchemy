#include "types.h"
#include "field_event.h"
#include "event_inline.h"

extern s16 gData_02003446;
extern struct FieldActor *gData_02003448[];
extern s16 gData_0200354a;

void Func_02001bcc(s32 id, s32 value) {
  struct FieldActor *object = ((struct FieldActor *)Actor_Get(id));

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_02003446;
    gData_02003448[*count] = object;
    (*count)++;
  }
}

void Func_02001bfc(s32 value) { gData_0200354a = value; }
