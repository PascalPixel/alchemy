#include "ov664.h"

void Func_02000f34(void);

void Func_02001084(struct FieldActor *object, s16 timer, u8 value)
{
    if (!GameFlag_Test(265)) {
        BYTE_AT(object, 90) = 0;
        object->timer = timer;
        BYTE_AT(object, 98) = value;
        WORD_AT(object, 108) = (s32)Func_02000f34;
    }
}
