#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200430a();
s32 Func_0200432a();
void Func_02004332();
s32 Func_02004364();
s32 Func_0200436c();
s32 Func_02004378();
void Func_0200437c();
void Func_020043a6();
void Func_020043b8();
void Func_020043fc();
void Func_02004434();
void Func_0200443e();
void Func_020044c2();
void Func_020044e4();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void FieldScene_RunScene381_02000e30(s32 a0)
{
    s32 p10;
    u8 *rec7;
    s32 recA;
    s32 rec2;
    s32 base6_0;

    p10 = a0;
    recA = Func_0200432a(8);
    *(s32 *)(recA + 24) = 0x10000;
    *(s32 *)(recA + 28) = 0x10000;
    Value3(Func_02004378, p10, 0x1d7, 0x122);
    Call3(Func_020043fc, p10, 0xc000, 0);
    Func_02004332(10);
    Call3(Func_020043a6, 8, 0x1d70000, 0x1220000);
    rec7 = Value1(Func_02004364, p10);
    rec2 = Func_0200436c(p10);
    Func_0200430a(rec2, 0);
    Call2(Func_020043fc, p10, 0x100);
    rec7[85] = 0;
    Func_020044c2(201);
    base6_0 = 0;
    do {
        *(volatile s32 *)((s32)rec7 + 12) += 0x8000;
        Func_0200437c(1);
        base6_0 = ((u32)((base6_0 + 1) << 24) >> 24);
    } while (base6_0 != 60);
    Func_020044e4(190);
    base6_0 = 0;
    do {
        *(volatile s32 *)((s32)rec7 + 12) += 0x1999;
        *(volatile s32 *)((s32)rec7 + 24) += -0x28f;
        *(volatile s32 *)((s32)rec7 + 28) += -0x28f;
        *(volatile s32 *)(recA + 24) += -0x28f;
        *(volatile s32 *)(recA + 28) += -0x28f;
        Func_020043b8(1);
        base6_0 = ((u32)((base6_0 + 1) << 24) >> 24);
    } while (base6_0 != 90);
    Func_02004434(p10, 0, 0);
    Func_0200443e(8, 0, 0);
}
