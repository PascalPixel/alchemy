#include "types.h"
#include "field_event.h"
#include "event_inline.h"

extern s16 gData_02002af2;
extern struct FieldActor *gData_02002af4[];
extern s16 gData_02002bf6;

void Func_02001544(s32 id, s32 value) {
  struct FieldActor *object = ((struct FieldActor *)Actor_Get(id));

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    /* FAKEMATCH(barrier): an empty asm after the timer store, so the list and
     * its count load after it; as the same statements with no asm, not
     * measured. */
    asm volatile("");
    count = &gData_02002af2;
    gData_02002af4[*count] = object;
    (*count)++;
  }
}

void Func_02001574(s32 value) { gData_02002bf6 = value; }
