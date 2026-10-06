#include "KABE.H"

void Func_02001658(struct KabeObject *object, s32 base)
{
    do {
        WaitFrames(1);
    } while (object->speed != 0);
    Func_02003b48(240);
    ObjectMotion_SetActionVariant(base + 21, 3);
    ObjectMotion_SetActionVariant(base + 22, 3);
    Object_SetPositionAndResetMotion(Object_GetById(base + 21), object->x, 0, object->z);
    Object_SetPositionAndResetMotion(Object_GetById(base + 22), object->x, 0, object->z + -0x100000);
    Object_SetModeById(base + 21, 1);
    Object_SetModeById(base + 22, 1);
    WaitFrames(20);
}

void Func_020016c8(void)
{
    Func_0200142c(Object_GetById(15));
}
