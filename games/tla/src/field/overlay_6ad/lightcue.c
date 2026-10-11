#include "types.h"
#include "field_event.h"

extern s16 Data_020074ce;
extern struct FieldActor *Data_020074d0[];
extern s16 Data_020075d2;

struct FieldActor *Object_GetById(s32 id);

void Func_0200538c(s32 id, s32 value)
{
    struct FieldActor *object = Object_GetById(id);

    if (object != NULL) {
        s16 *count;

        object->timer = value;
        asm volatile(""); /* FAKEMATCH: ⚓️ loads the list and its count after the timer store */
        count = &Data_020074ce;
        Data_020074d0[*count] = object;
        (*count)++;
    }
}

void Func_020053bc(s32 value)
{
    Data_020075d2 = value;
}
