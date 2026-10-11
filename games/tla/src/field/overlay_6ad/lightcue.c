#include "types.h"
#include "field_event.h"

extern s16 Data_020074ce;
extern struct FieldActor *Data_020074d0[];
extern s16 Data_020075d2;

struct FieldActor *Object_GetById(s32 id);

void Func_020053bc(s32 value)
{
    Data_020075d2 = value;
}
