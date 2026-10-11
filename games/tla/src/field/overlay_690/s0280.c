#include "local.h"

void Func_02000280(void)
{
    Func_02002158(10, 1, 14);
}

void Func_02000290(void)
{
    Func_02002158(11, 2, 14);
}

void Func_020002a0(void)
{
    Func_02002158(23, 3, 19);
}

s32 Func_020002b0(struct ObjectRuntime *object)
{
    ObjectDispatch_SetSingleChildFlags(object, 0);
    return 0;
}

s32 Func_020002bc(struct ObjectRuntime *object)
{
    u8 *phase = (u8 *)object + 0x62;
    s32 step = *phase & 63;
    s16 *animation = object->animation;

    object->motion_flags = 3;
    if (step == 0)
        object->velocity_y = 0x30000;
    if (step == 16)
        object->velocity_y = 0x20000;
    if (step == 24)
        object->velocity_y = 0x30000;
    animation[9] = Math_Cosine(step << 12) / 64;
    (*phase)++;
    return -1;
}
