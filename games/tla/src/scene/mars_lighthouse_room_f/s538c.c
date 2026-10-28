#include "types.h"
#include "field_event.h"

extern s16 gData_020074ce;
extern struct FieldActor *gData_020074d0[];
extern s16 gData_020075d2;

struct FieldActor *Actor_GetById(s32 id);

void Func_0200538c(s32 id, s32 value) {
  struct FieldActor *object = Actor_GetById(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    asm volatile(""); /* FAKEMATCH: ⚓️ loads the list and its count after the
                         timer store */
    count = &gData_020074ce;
    gData_020074d0[*count] = object;
    (*count)++;
  }
}
