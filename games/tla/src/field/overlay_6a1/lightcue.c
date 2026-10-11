#include "types.h"
#include "field_event.h"

extern s16 Data_02002af2;
extern struct FieldActor *Data_02002af4[];
extern s16 Data_02002bf6;

struct FieldActor *Object_GetById(s32 id);

void Func_02001574(s32 value)
{
    Data_02002bf6 = value;
}
