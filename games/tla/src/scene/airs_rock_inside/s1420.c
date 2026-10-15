#include "airs_rock_inside.h"

void Func_02001290(void);

void Func_02001420(struct FieldActor *object, s16 timer, u8 value)
{
    if (!GameFlag_Test(265)) {
        BYTE_AT(object, 90) = 0;
        object->timer = timer;
        BYTE_AT(object, 98) = value;
        WORD_AT(object, 108) = (s32)Func_02001290;
        ObjectDispatch_SetSingleChildFlags(object, 0);
    }
}
