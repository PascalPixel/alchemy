#include "TYPES.H"

#define HaidiaArashi_RunCallOutSequence Func_0200173c

extern u8 Data_00000e70[];
void Func_02005e66();
s32 Func_02005e7c();
void Func_02005e90();
void Func_02005ea0();
void Func_02005ef0();
s32 Func_02005f08();
void Func_02005f12();
void Func_02005f1a();
s32 Func_02005f20();
void Func_02005f22();
void Func_02005f2a();
void Func_02005f36();
void Func_02005f40();
void Func_02005f50();
void Func_02005f52();
void Func_02005f62();
void Func_02005f68();
void Func_02005f6a();
void Func_02005f78();
void Func_02005f8a();
void Func_02005f92();
void Func_02005f9a();
void Func_02005fb2();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

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
void Func_02005f9a_a();
void Func_02005f9a_b();

void HaidiaArashi_RunCallOutSequence(void)
{
    s32 record;
    s32 v5;

    Value2(Func_02005f20, 22, 0);
    Func_02005f12(0, 22, 0);
    v5 = 0;
    if (Value2(Func_02005e7c, 0, 0) == 0) {
        Call1(Func_02005f36, 0xee5);
        v5 = 1;
    } else {
        Call1(Func_02005f40, 0xee6);
    }
    Func_02005e66(20);
    Func_02005f68(22, 0, 40);
    Call2(Func_02005f9a_a, 22, 0x100);
    Func_02005f22(21, 3);
    Func_02005f2a(22, 1);
    Func_02005e90(40);
    Func_02005f62(22, 0, 0);
    Func_02005ea0(20);
    Func_02005f50(22, 3);
    if (v5 != 0) {
        /* FAKEMATCH: 0xe70 comes from the literal pool through a link symbol. */
        Func_02005f92((s32)Data_00000e70);
    } else {
        Call1(Func_02005f9a_b, 0xee7);
    }
    Func_02005fb2(22, 0);
    Func_02005f6a(22, 2);
    record = Value1(Func_02005f08, 0);
    if (record != 0) {
        Func_02005f52(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02005f78(22);
    Func_02005f8a(22, 0, 0);
    Func_02005f1a(1, 1);
    Call1(Func_02005ef0, 0x837);
}
