#include "ov6a4.h"

void Func_02001010(struct ObjectRuntime *object)
{
    u8 *fill = &gDecodeFillByte;
    u8 *step = &BYTE_AT(object, 99);
    s32 phase = *step & 3;

    *fill = 2;
    switch (phase) {
    case 1:
        Animation_ApplyChildValues(object, 13);
        break;
    case 0:
    case 2:
        Animation_ApplyChildValues(object, 4);
        break;
    case 3:
        Animation_ApplyChildValues(object, 4);
        break;
    }
    (*step)++;
}
