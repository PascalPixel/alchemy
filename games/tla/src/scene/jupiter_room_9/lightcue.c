#include "types.h"
#include "field_event.h"

extern s16 gData_0200265e;
extern struct FieldActor *gData_02002660[];
extern s16 gData_02002762;

struct FieldActor *Object_GetById(s32 id);

void Func_02000f5c(s32 id, s32 value)
{
    struct FieldActor *object = Object_GetById(id);

    if (object != NULL) {
        s16 *count;

        object->timer = value;
        asm volatile(""); /* FAKEMATCH: ⚓️ loads the list and its count after the timer store */
        count = &gData_0200265e;
        gData_02002660[*count] = object;
        (*count)++;
    }
}

void Func_02000f8c(s32 value)
{
    gData_02002762 = value;
}
