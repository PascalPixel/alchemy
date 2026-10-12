#include "types.h"
#include "field_event.h"

extern s16 Data_02002af2;
extern struct FieldActor *Data_02002af4[];
extern s16 Data_02002bf6;

struct FieldActor *Object_GetById(s32 id);

void Func_02001544(s32 id, s32 value)
{
    struct FieldActor *object = Object_GetById(id);

    if (object != NULL) {
        s16 *count;

        object->timer = value;
        asm volatile(""); /* FAKEMATCH: ⚓️ loads the list and its count after the timer store */
        count = &Data_02002af2;
        Data_02002af4[*count] = object;
        (*count)++;
    }
}

void Func_02001574(s32 value)
{
    Data_02002bf6 = value;
}
