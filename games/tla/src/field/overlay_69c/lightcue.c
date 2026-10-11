#include "types.h"
#include "field_event.h"

extern s16 Data_02002d2e;
extern struct FieldActor *Data_02002d30[];
extern s16 Data_02002e32;

struct FieldActor *Object_GetById(s32 id);

void Func_020016fc(s32 value)
{
    Data_02002e32 = value;
}
