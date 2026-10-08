#include "types.h"
#include "field_event.h"

extern s16 Data_020023f2;
extern struct FieldActor *Data_020023f4[];
extern s16 Data_020024f6;

struct FieldActor *Object_GetById(s32 id);

void Func_02000fd8(s32 id, s32 value)
{
    struct FieldActor *object = Object_GetById(id);

    if (object != NULL) {
        s16 *count;

        object->timer = value;
        asm volatile(""); /* FAKEMATCH: ⚓️ loads the list and its count after the timer store */
        count = &Data_020023f2;
        Data_020023f4[*count] = object;
        (*count)++;
    }
}

void Func_02001008(s32 value)
{
    Data_020024f6 = value;
}
