#include "types.h"
#include "field_event.h"

extern s16 gData_02003446;
extern struct FieldActor *gData_02003448[];
extern s16 gData_0200354a;

struct FieldActor *Object_GetById(s32 id);

void Func_02001bcc(s32 id, s32 value)
{
    struct FieldActor *object = Object_GetById(id);

    if (object != NULL) {
        s16 *count;

        object->timer = value;
        asm volatile(""); /* FAKEMATCH: ⚓️ loads the list and its count after the timer store */
        count = &gData_02003446;
        gData_02003448[*count] = object;
        (*count)++;
    }
}

void Func_02001bfc(s32 value)
{
    gData_0200354a = value;
}
