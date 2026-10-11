#include "ov65f.h"

void Func_02000f28(void)
{
    Object_GetById(10);
    Flag_IsSet(0x203);
}

void Func_02000f3c(void)
{
    struct ObjectRuntime *object = Object_GetById(11);
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;

    if (!Flag_IsSet(0x204) && x == 15 && z == 25) {
        Func_02000e78(11, object);
        Flag_Set(0x204);
    }
}

void Func_02000f74(void)
{
    Func_0200537c();
    Func_02000eb0();
}

void Func_02000f80(void)
{
    Func_0200537c();
    Func_02000eec();
}

void Func_02000f8c(void)
{
    Func_0200537c();
    Func_02000f28();
}

void Func_02000f98(void)
{
    Func_0200537c();
    Func_02000f3c();
}

void Func_02000fa4(void)
{
    Func_0200537c();
    Func_02000eb0();
    Func_02000eec();
    Func_02000f28();
    Func_02000f3c();
}

static inline void CopyRect(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters give each stack argument its own register; a direct call reuses r3 */
    Func_02005184(a0, a1, a2, a3, a4, a5);
}

static inline void BlendRect(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters give each stack argument its own register; a direct call reuses r3 */
    Func_0200518c(a0, a1, a2, a3, a4, a5);
}

void Func_02000fbc(void)
{
    CopyRect(98, 8, 86, 8, 1, 2);
    BlendRect(34, 9, 1, 1, 22, 11);
}

void Func_02000fec(void)
{
    CopyRect(25, 47, 11, 33, 1, 3);
    BlendRect(25, 47, 1, 1, 11, 33);
}

void Func_0200101c(void)
{
    CopyRect(44, 5, 19, 5, 3, 3);
    CopyRect(108, 5, 83, 5, 3, 2);
    BlendRect(44, 5, 3, 3, 19, 5);
}

void Func_0200105c(void)
{
    CopyRect(102, 8, 86, 8, 1, 2);
    BlendRect(22, 10, 1, 1, 22, 11);
}

s32 Func_0200108c(void)
{
    return Flag_IsSet(0x310);
}

void Func_02001098(void)
{
    if (Flag_IsSet(0x310)) {
        if (Flag_IsSet(0x318))
            Func_020052cc(0x10003, 1);
        else
            Func_020052cc(0x10007, 1);
        Func_020052d4(16);
    }
}

void Func_020010d4(void)
{
    CopyRect(8, 30, 11, 33, 1, 3);
    BlendRect(8, 30, 1, 1, 11, 33);
}

void Func_02001104(void)
{
    CopyRect(39, 5, 19, 5, 3, 3);
    CopyRect(103, 5, 83, 5, 3, 2);
    BlendRect(39, 5, 3, 3, 19, 5);
}
