#include "types.h"
#include "field_event.h"

extern s16 gData_02002d2e;
extern struct FieldActor *gData_02002d30[];
extern s16 gData_02002e32;

struct FieldActor *Actor_GetById(s32 id);

void Func_020016cc(s32 id, s32 value) {
  struct FieldActor *object = Actor_GetById(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    asm volatile(""); /* FAKEMATCH: ⚓️ loads the list and its count after the
                         timer store */
    count = &gData_02002d2e;
    gData_02002d30[*count] = object;
    (*count)++;
  }
}

void Func_020016fc(s32 value) { gData_02002e32 = value; }
