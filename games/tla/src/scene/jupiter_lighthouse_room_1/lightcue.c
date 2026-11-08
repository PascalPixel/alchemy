#include "types.h"
#include "field_event.h"

extern s16 gData_0200229a;
extern struct FieldActor *gData_0200229c[];
extern s16 gData_0200239e;

struct FieldActor *Actor_GetById(s32 id);

void Func_02000e80(s32 id, s32 value) {
  struct FieldActor *object = Actor_GetById(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    /* FAKEMATCH: ⚓️ loads the list and its count after the timer store; without
     * it the count's address is loaded before the store (6 differ), and a
     * post-increment index is worse (17) */
    asm volatile("");
    count = &gData_0200229a;
    gData_0200229c[*count] = object;
    (*count)++;
  }
}

void Func_02000eb0(s32 value) { gData_0200239e = value; }
