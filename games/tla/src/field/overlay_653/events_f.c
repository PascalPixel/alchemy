#include "local.h"

void Func_02000b5c(void)
{
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 1);
}

void Func_02000b78(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    *((u8 *)object + 98) = 1;
    object->priority_flags |= 2;
    Object_SetModeById(id, 1);
    ObjectMotion_SetActionVariant(id, 3);
    Func_020052c0(id);
}

void Func_02000bac(s32 id)
{
    *((u8 *)Object_GetById(id) + 98) = 1;
    ObjectMotion_SetActionVariant(id, 2);
    Func_020052c0(id);
}

void Func_02000bcc(void)
{
    Func_02005348(133, 1);
    Func_02005350(12, 10);
    Func_02005368();
    Field_DispatchTypeHandler(1);
    Func_02005358();
    Func_02005360();
}

void Func_02000bf4(void)
{
    s32 position[3];

    if (Data_02006298 != 0 && gFrameCount % 9 == 0) {
        position[0] = 0x1280000;
        position[1] = 0;
        position[2] = 0xb00000;
        Func_0200098c(position, 0x20000);
    }
}

void Func_02000c34(void)
{
    s32 position[3];

    if (Data_0200629c != 0 && gFrameCount % 5 == 0) {
        position[0] = 0x1580000;
        position[1] = 0;
        position[2] = 0xa00000;
        Func_0200098c(position, 0x40000);
    }
}

void Func_02000c74(void)
{
    if (gFrameCount % 5 == 0)
        Func_02000adc();
}

void Func_02000c90(s32 id)
{
    Object_GetById(id)->scale_y = 0x800;
    Object_SetPositionAndResetMotion_(Object_GetById(id), 0xd80000, 0, 0xd80000);
}

void Func_02000cb4(s32 id)
{
    Object_GetById(id)->scale_y = 0x800;
    Object_SetPositionAndResetMotion_(Object_GetById(id), 0x1780000, 0, 0xf80000);
}

void Func_02000cd8(s32 id, s32 dy, s32 scale)
{
    struct ObjectRuntime *object = Object_GetById(id);

    object->scale_y += scale;
    object->y += dy;
    object->z -= dy;
}

void Func_02000cf8(s32 id, s32 dy, s32 scale)
{
    struct ObjectRuntime *object = Object_GetById(id);

    object->scale_y -= scale;
    object->y -= dy;
    object->z += dy;
}

s32 Func_02000d18(void)
{
    if (GameFlag_Test(0x20a) || GameFlag_Test(0x300))
        return 1;
    return 0;
}

s32 Func_02000d3c(void)
{
    if (GameFlag_Test(0x20a) || GameFlag_Test(0x301))
        return 1;
    return 0;
}
