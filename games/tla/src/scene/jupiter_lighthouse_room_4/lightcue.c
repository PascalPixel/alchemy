#include "types.h"
#include "field_event.h"

extern s16 gData_0200275e;
extern struct FieldActor *gData_02002760[];
extern s16 gData_02002862;

struct FieldActor *Actor_GetById(s32 id);

void Func_0200119c(s32 id, s32 value) {
  struct FieldActor *object = Actor_GetById(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    asm volatile(""); /* FAKEMATCH: ⚓️ loads the list and its count after the
                         timer store */
    count = &gData_0200275e;
    gData_02002760[*count] = object;
    (*count)++;
  }
}

void Func_020011cc(s32 value) { gData_02002862 = value; }
