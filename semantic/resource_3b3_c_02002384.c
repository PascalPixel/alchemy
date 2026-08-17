#include "types.h"

extern s32 Func_02004e78();
extern void Func_02004e8c(s32 flagId);
extern void Func_02004ea8(void);
extern void Func_02004f20(s32 distance, s32 step);
extern void Func_02004f3a(s32 x, s32 y, s32 z, s32 mode);
extern void Func_02004f46(void);
extern void Func_02004ec4(s32 frames);
extern void Func_02004ede(s32 frames);
extern void Func_02004e20(s32 callback, s32 delay);
extern void Func_02004eee(s32 frames);
extern s32 Func_02004edc(s32 flagId);
extern u8 *Func_02004f16(s32 actorId);
extern void Func_02004f5a(s32 actorId, s32 mode);
extern void Func_02004f68(s32 actorId);
extern u8 *Func_02004f2e(s32 actorId);
extern void Func_02004eec(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                          s32 arg4, s32 arg5);
extern void Func_02004f2a(s32 flagId);
extern void Func_02004f9a(s32 actorId, s32 mode);
extern void Func_02004f6a(s32 actorId, s32 mode);
extern void Func_02004f4a(s32 frames);
extern void Func_02004e90(s32 callback);
extern void Func_02004f56(s32 frames);
extern void Func_02004f0a();
extern void Func_02004f70(s32 frames);
extern void Func_02004f84(void);

void Func_02002384(void)
{
    if (Func_02004e78(0x203) != 0)
        return;

    Func_02004e8c(0x202);
    Func_02004ea8();
    Func_02004f20(0x9999, 0x1333);
    Func_02004f3a(0x01380000, -1, 0x00b80000, 1);
    Func_02004f46();
    Func_02004ec4(20);
    {
        s32 arg4 = 1;
        s32 arg5 = 2;
        Func_02004e78(73, 10, 60, 10, arg4, arg5);
    }
    Func_02004ede(20);
    Func_02004e20(0x0200a231, 3200);
    Func_02004eee(40);

    if (Func_02004edc(0x201) != 0) {
        u8 *actor12 = Func_02004f16(12);
        *(s32 *)(actor12 + 108) = 0x0200a2d9;
        Func_02004f5a(12, 6);
        Func_02004f68(12);
        actor12 = Func_02004f2e(12);
        *(s32 *)(actor12 + 108) = 0;
        {
            s32 arg4 = 18;
            s32 arg5 = 13;
            Func_02004eec(17, 13, 1, 1, arg4, arg5);
        }
        Func_02004f2a(0x201);
        Func_02004f9a(12, 0);
        Func_02004f6a(12, 1);
    } else {
        Func_02004f4a(60);
    }

    Func_02004e90(0x0200a231);
    Func_02004f56(20);
    {
        s32 arg4 = 1;
        s32 arg5 = 2;
        Func_02004f0a(72, 10, 60, 10, arg4, arg5);
    }
    Func_02004f70(20);
    Func_02004f84();
}
