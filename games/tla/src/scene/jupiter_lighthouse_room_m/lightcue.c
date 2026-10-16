#include "types.h"
#include "field_event.h"

extern s16 gData_02005a82;
extern struct FieldActor *gData_02005a84[];
extern s16 gData_02005b86;

struct FieldActor *Object_GetById(s32 id);

void Func_02003ac0(s32 id, s32 value) {
  struct FieldActor *object = Object_GetById(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    /* FAKEMATCH: ⚓️ loads the list and its count after the timer store; without
     * it the count's address is loaded before the store (6 differ) */
    asm volatile("");
    count = &gData_02005a82;
    gData_02005a84[*count] = object;
    (*count)++;
  }
}

void Func_02003af0(s32 value) { gData_02005b86 = value; }
