#include "types.h"

extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern void Func_080000d0(s32 callback, s32 delay);
extern void Func_080000d8(s32 callback);
extern void Func_08009180(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                          s32 arg4, s32 arg5);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                          s32 arg4, s32 arg5);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_0808a098(s32 actorId, s32 mode);
extern void Func_0808a100(s32 actorId, s32 mode);
extern void Func_0808a118(s32 actorId);
extern void Func_0808a158(s32 actorId, s32 mode);
extern void Func_0808a208(s32 distance, s32 step);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 mode);
extern void Func_0808a218(void);

/*
 * Complete 276-byte flag-0x203 scene owner through alignment and seven-word
 * pool.  Its 28 static calls preserve both branches in machine order.
 */
void Func_02002384(void)
{
    if (Func_080770c0(0x203) != 0)
        return;

    Func_080770c8(0x202);
    Func_0808a018();
    Func_0808a208(0x9999, 0x1333);
    Func_0808a210(0x01380000, -1, 0x00b80000, 1);
    Func_0808a218();
    Func_0808a010(20);
    Func_08009180(73, 10, 60, 10, 1, 2);
    Func_0808a010(20);
    Func_080000d0(0x0200a231, 3200);
    Func_0808a010(40);

    if (Func_080770c0(0x201) != 0) {
        u8 *actor12 = Func_0808a080(12);
        *(s32 *)(actor12 + 108) = 0x0200a2d9;
        Func_0808a100(12, 6);
        Func_0808a118(12);
        actor12 = Func_0808a080(12);
        *(s32 *)(actor12 + 108) = 0;
        Func_080091c0(17, 13, 1, 1, 18, 13);
        Func_080770d0(0x201);
        Func_0808a158(12, 0);
        Func_0808a098(12, 1);
    } else {
        Func_0808a010(60);
    }

    Func_080000d8(0x0200a231);
    Func_0808a010(20);
    Func_08009180(72, 10, 60, 10, 1, 2);
    Func_0808a010(20);
    Func_0808a020();
}
