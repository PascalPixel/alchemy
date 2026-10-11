#include "types.h"
#include "field_event.h"

extern s16 Data_02006a6e;
extern struct FieldActor *Data_02006a70[];
extern s16 Data_02006b72;

struct FieldActor *Object_GetById(s32 id);

void Func_02004b18(s32 id, s32 value)
{
    struct FieldActor *object = Object_GetById(id);

    if (object != NULL) {
        s32 once = 0;

        /* FAKEMATCH: a one-pass loop keeps the list and count loads after the timer store; as plain statements the count address loads before the store (differ 6) */
        do {
            object->timer = value;
            Data_02006a70[Data_02006a6e] = object;
            Data_02006a6e++;
        } while (once);
    }
}

void Func_02004b48(s32 value)
{
    Data_02006b72 = value;
}
