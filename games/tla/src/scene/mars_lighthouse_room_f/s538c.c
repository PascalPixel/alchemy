#include "types.h"
#include "field_event.h"
#include "event_inline.h"

extern s16 gData_020074ce;
extern struct FieldActor *gData_020074d0[];
extern s16 gData_020075d2;


void Func_0200538c(s32 id, s32 value) {
  struct FieldActor *object = (struct FieldActor *)Actor_Get(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_020074ce;
    gData_020074d0[*count] = object;
    (*count)++;
  }
}
