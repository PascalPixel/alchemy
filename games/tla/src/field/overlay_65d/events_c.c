#include "local.h"

void Func_020004ec(void)
{
    s32 id;

    Func_02000648();
    Func_020006d0(0);
    ObjectMotion_CommitPositionAndActivate_(4, 0, -14);
    id = 4;
    ObjectMotion_CommitPositionAndActivate(id, 16, 0);
    Func_02000438();
}

void Func_02000514(void)
{
    s32 id, x, z;

    Func_02000648();
    Func_020006d0(0);
    ObjectMotion_CommitPositionAndActivate_(4, -2, -6);
    x = 14;
    z = 8;
    id = 4;
    /* FAKEMATCH: the actor id comes between the distances and their negation. */
    __asm__ volatile("" : "+r"(id));
    ObjectMotion_CommitPositionAndActivate(id, -x, -z);
    Func_02000438();
}

void Func_02000544(void)
{
    s32 id, a;

    Func_02000648();
    Func_020006d0(0);
    a = 192;
    id = 4;
    /* FAKEMATCH: the actor id comes between the angle byte and its shift. */
    __asm__ volatile("" : "+r"(id));
    ObjectMotion_ArmCallback(id, a << 8, 0);
    Func_02000438();
}

void Func_02000564(void)
{
    s32 x = ((s16 *)&Object_GetById(4)->x)[1];

    Func_02000648();
    Func_020006d0(0);
    if (x % 16 <= 7) {
        ObjectMotion_CommitPositionAndActivate_(4, -4, -4);
        ObjectMotion_CommitPositionAndActivate_(4, -4, -8);
    } else {
        ObjectMotion_CommitPositionAndActivate_(4, 4, -4);
        ObjectMotion_CommitPositionAndActivate_(4, 4, -8);
    }
    ObjectMotion_SetPositionAndReset_(4, 424, 99);
    Func_02000438();
}

void Func_020005d4(void)
{
    Func_02000648();
    Func_020006d0(0);
    ObjectMotion_CommitPositionAndActivate_(4, 0, -4);
    ObjectMotion_SetPositionAndReset_(4, 424, 99);
    Func_02000438();
}
