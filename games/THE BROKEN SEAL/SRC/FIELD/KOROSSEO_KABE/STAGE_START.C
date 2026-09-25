#include "TYPES.H"

extern u8 Data_00000090[];
extern u8 Data_02000240[];
void Func_02003c48();
void Func_02003c6a();
s32 Func_02003d70();
void Func_0200495c();
void Func_020049a4();
void Func_020049ae();
void Func_020049bc();
void Func_020049c8();
void Func_020049d2();
void Func_020049d2_a();
void Func_020049f4();
void Func_02004a0c();
void Func_02004a10();
void Func_02004a14();
void Func_02004a1c();
void Func_02004a1e();
void Func_02004a2c();
void Func_02004a3c();
void Func_02004a42();
void Func_02004a46();
void Func_02004a56();
void Func_02004a76();
void Func_02004a7e();
s32 Func_02004b30();
s32 Func_02004b4c();
void Func_02004b5c();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void KorosseoKabe_RunStageStart(void)
{
    s32 rec8;
    s32 record;
    s32 v5;
    s32 base;
    s32 base3_2000240;

    base = 2;
    Func_02003c48();
    Func_0200495c();
    rec8 = Value2(Func_02003d70, 77, 89);
    Func_02003c6a();
    v5 = 9;
    do {
        Func_020049a4(8);
        v5 = (v5 - 1);
    } while (v5 >= 0);
    Call3(Func_020049ae, 8, 0x10000, 0x8000);
    Call3(Func_020049d2, 8, 88, 0x100);
    Call3(Func_020049c8, 0, 0x10000, 0x8000);
    Call3(Func_020049f4, 0, 120, 0x100);
    Func_02004a14(8, 1);
    Func_02004a46(0, 8, 0);
    Func_020049bc(10);
    Func_02004a2c(8, 3);
    Func_02004a3c(0, 3);
    Func_020049d2_a(20);
    Call3(Func_02004a10, 0, 0x20000, 0x10000);
    Call3(Func_02004a1e, 8, 0x20000, 0x10000);
    Call3(Func_02004a42, 0, 112, 0x100);
    Call3(Func_02004a56, 8, 96, 0x100);
    Func_02004a76(0, 16);
    Func_02004a7e(8, 9);
    Func_02004a1c(10);
    /* FAKEMATCH: base is set at the top of the function, so 2 - rec8 + 1
     * is not folded into 3 - rec8. */
    Value2(Func_02004b30, 72, base - rec8 + 1);
    base3_2000240 = (s32)Data_02000240;
    /* FAKEMATCH: the do/while keeps the stage flag store ahead of the
     * pool load that follows it. */
    do {
        *(u8 *)((base3_2000240 + 0x22b)) = 3;
    } while (0);
    Value2(Func_02004b4c, (s32)Data_00000090, 4);
    Func_02004b5c((s32)Data_00000090, 5);
    Call1(Func_02004a0c, 0x11a);
}
