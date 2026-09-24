#include "TYPES.H"

#define KorosseoKawa_RunStageStart Func_02000840

extern u8 Data_0000008f[];
extern u8 Data_02000240[];
void Func_020037d4();
void Func_020037f6();
s32 Func_020038fc();
void Func_020044e8();
void Func_02004530();
void Func_0200453a();
void Func_02004548();
void Func_02004554();
void Func_0200455e();
void Func_02004580();
void Func_0200459c();
void Func_020045a0();
void Func_020045a8();
void Func_020045aa();
void Func_020045b8();
void Func_020045c8();
void Func_020045ce();
void Func_020045d2();
void Func_020045e2();
void Func_02004602();
void Func_0200460a();
s32 Func_020046ac();
s32 Func_020046c8();
void Func_020046d8();

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
void Func_0200455e_a();
void Func_0200455e_b();
void Func_020045a0_a();
void Func_020045a0_b();

void KorosseoKawa_RunStageStart(void)
{
    s32 rec8;
    s32 record;
    s32 v5;
    s32 base;
    s32 base3_2000240;

    base = 0;
    Func_020037d4();
    Func_020044e8();
    rec8 = Value2(Func_020038fc, 120, 127);
    Func_020037f6();
    v5 = 9;
    do {
        Func_02004530(8);
        v5 = (v5 - 1);
    } while (v5 >= 0);
    Call3(Func_0200453a, 8, 0x10000, 0x8000);
    Call3(Func_0200455e_a, 8, 0x528, 192);
    Call3(Func_02004554, 0, 0x10000, 0x8000);
    Call3(Func_02004580, 0, 0x508, 192);
    Func_020045a0_a(8, 1);
    Func_020045d2(0, 8, 0);
    Func_02004548(10);
    Func_020045b8(8, 3);
    Func_020045c8(0, 3);
    Func_0200455e_b(20);
    Call3(Func_0200459c, 0, 0x20000, 0x10000);
    Call3(Func_020045aa, 8, 0x20000, 0x10000);
    Call3(Func_020045ce, 0, 0x510, 192);
    Call3(Func_020045e2, 8, 0x520, 192);
    Func_02004602(0, 16);
    Func_0200460a(8, 9);
    Func_020045a8(10);
    /* FAKEMATCH: base is 0 from the top of the function, so 0 - rec8 + 1
     * is not folded into 1 - rec8. */
    Value2(Func_020046ac, 72, base - rec8 + 1);
    base3_2000240 = (s32)Data_02000240;
    /* FAKEMATCH: the do/while keeps the stage flag store ahead of the
     * 0x8f pool load, which comes through a link symbol. */
    do {
        *(u8 *)((base3_2000240 + 0x22b)) = 3;
    } while (0);
    Value2(Func_020046c8, (s32)Data_0000008f, 4);
    Func_020046d8((s32)Data_0000008f, 5);
    Call1(Func_020045a0_b, 0x11a);
}
