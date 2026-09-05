#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200475e();
s32 Func_02004768();
s32 Func_02004772();
s32 Func_020047d6();
s32 Func_020047e2();
s32 Func_020047ec();
s32 Func_020047f6();
s32 Func_02004800();
s32 Func_0200480a();
s32 Func_02004812();
s32 Func_0200481a();
s32 Func_02004822();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void FieldScene_RunScene3b4_02002290(void)
{
    s32 record;

    *(u8 *)(Func_020047d6(8) + 89) = 1;
    *(u8 *)(Func_020047e2(9) + 89) = 1;
    *(u8 *)(Func_020047ec(10) + 89) = 1;
    *(u8 *)(Func_020047f6(11) + 89) = 1;
    record = Func_02004800(8);
    *(volatile s32 *)(record + 24) = 0xb333;
    record = Value1(Func_0200480a, 9);
    *(volatile s32 *)(record + 24) = 0xb333;
    record = Value1(Func_02004812, 10);
    *(volatile s32 *)(record + 24) = 0xb333;
    record = Value1(Func_0200481a, 11);
    *(volatile s32 *)(record + 24) = 0xb333;
    record = Func_02004822(12);
    *(volatile s32 *)(record + 24) = 0xb333;
    Call2(Func_0200475e, 0x20097ad, 0xc80);
    Value2(Func_02004768, 0x200941d, 0xc80);
    Value2(Func_02004772, 0x2009309, 0xc80);
    {
        u16 t;
        t = 0x3f42;
        *(volatile u16 *)0x04000050 = t;
        t = 0x607;
        *(volatile u16 *)0x04000052 = t;
    }
}
