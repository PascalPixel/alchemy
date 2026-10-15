#include "airs_rock_inside.h"

void Func_02002cc0(struct FieldActor *object)
{
    s32 bit = ((gFrameCount & 3) >> BYTE_AT(object, 99)) & 1;

    Object_SetPartAttribute(object, (u8)(bit * BYTE_AT(object, 98)));
}
