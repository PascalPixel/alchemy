#include "types.h"
#include "field_event.h"

extern s16                gData_02003382;
extern struct FieldActor *gData_02003384[];
extern s16                gData_02003486;

struct FieldActor *Object_GetById(s32 id);

void Func_02001dbc(s32 id, s32 value)
{
    struct FieldActor *object = Object_GetById(id);

    if (object != NULL)
    {
        s16 *count;

        object->timer = value;
        asm volatile(""); /* FAKEMATCH: ⚓️ loads the list and its count after the timer store */
        count = &gData_02003382;
        gData_02003384[*count] = object;
        (*count)++;
    }
}

void Func_02001dec(s32 value)
{
    gData_02003486 = value;
}
