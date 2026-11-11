#include "types.h"
#include "field_event.h"
#include "event_inline.h"

extern s16 gData_02006a6e;
extern struct FieldActor *gData_02006a70[];
extern s16 gData_02006b72;

void Func_02004b18(s32 id, s32 value) {
  struct FieldActor *object = ((struct FieldActor *)Actor_Get(id));

  if (object != NULL) {
    s32 once = 0;

    /* FAKEMATCH(dead): a once-only loop on a local 0 keeps the list and count
     * loads after the timer store; as plain statements, the count's address
     * loads before the store and 6 instructions differ. */
    do {
      object->timer = value;
      gData_02006a70[gData_02006a6e] = object;
      gData_02006a6e++;
    } while (once);
  }
}

void Func_02004b48(s32 value) { gData_02006b72 = value; }
