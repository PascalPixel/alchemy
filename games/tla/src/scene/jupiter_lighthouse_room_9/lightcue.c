#include "types.h"
#include "field_event.h"
#include "event_inline.h"

extern s16 gData_0200265e;
extern struct FieldActor *gData_02002660[];
extern s16 gData_02002762;

void Func_02000f5c(s32 id, s32 value) {
  struct FieldActor *object = ((struct FieldActor *)Actor_Get(id));

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_0200265e;
    gData_02002660[*count] = object;
    (*count)++;
  }
}

void Func_02000f8c(s32 value) { gData_02002762 = value; }
