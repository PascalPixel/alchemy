#include "types.h"
#include "field_event.h"

extern s16 gData_02002202;
extern struct FieldActor *gData_02002204[];
extern s16 gData_02002306;

struct FieldActor *Actor_GetById(s32 id);

void Func_02000df0(s32 id, s32 value) {
  struct FieldActor *object = Actor_GetById(id);

  if (object != NULL) {
    s16 *count;

    /* FAKEMATCH: the one-pass block keeps the list and its count loads after
     * the timer store. */
    do {
      object->timer = value;
    } while (0);
    count = &gData_02002202;
    gData_02002204[*count] = object;
    (*count)++;
  }
}

void Func_02000e20(s32 value) { gData_02002306 = value; }
