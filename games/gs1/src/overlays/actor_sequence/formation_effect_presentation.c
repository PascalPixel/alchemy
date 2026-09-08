#include "types.h"

#define FieldScene_RunFormationAndEffectPresentation Func_020052f4

void Func_02009ba2();
void Func_02009bb2();
void Func_02009bc0();
void Func_02009bd2();
void Func_02009be2();
void Func_02009bf6();
void Func_02009c2e();
void Func_02009c42();
void Func_02009cb2();
void Func_02009cce();
void Func_02009cde();
void Func_02009cfe();
void Func_02009d64();
void Func_02009d6e();
void Func_02009d84();
void Func_02009d90();
void Func_02009dc8();
void Func_02009dd6();
void Func_02009df2();
void Func_02009df6();
void Func_02009e16();
void Func_02009e7c();
void Func_02009e9c();
void Func_02009f16();
void Func_0200b850();
void Func_0200b8c0();
void Func_0200b8fe();
void Func_0200b942();
void Func_0200b94e();
void Func_0200b95a();
void Func_0200b96c();
void Func_0200b978();
void Func_0200b97c();
void Func_0200b984();
void Func_0200b98a();
void Func_0200b990();
void Func_0200b9b6();
void Func_0200b9ca();
void Func_0200b9d8();
void Func_0200b9e4();
void Func_0200b9e6();
void Func_0200b9ec();
void Func_0200b9ee();
void Func_0200b9f0();
void Func_0200b9fc();
void Func_0200b9fe();
void Func_0200ba0c();
void Func_0200ba12();
void Func_0200ba2a();
void Func_0200ba34();
s32 Func_0200ba38();
void Func_0200ba3a();
void Func_0200ba40();
void Func_0200ba6e();
void Func_0200ba72();
void Func_0200ba82();
void Func_0200ba98();
void Func_0200baaa();
void Func_0200babe();
void Func_0200bac0();
void Func_0200bac4();
void Func_0200bac6();
void Func_0200bad4();
void Func_0200bad6();
void Func_0200bade();
void Func_0200bae2();
void Func_0200bae4();
void Func_0200baea();
void Func_0200baee();
void Func_0200baf4();
s32 Func_0200bb00();
void Func_0200bb1c();
void Func_0200bb34();
void Func_0200bb38();
void Func_0200bbce();
void Func_0200bbe8();
void Func_0200bc58();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Func_0200b978_a();
void Func_0200b9fc_a();
void Func_0200ba34_a();

extern u8 Data_0200e7f0[];

void FieldScene_RunFormationAndEffectPresentation(void)
{
    s32 action;

    Func_02009ba2(0, 0x1bc, 0x12c, 0);
    Func_02009bb2(1, 0x1ca, 0x136, 0);
    Func_02009bc0(2, 0x1bc, 0x14a, 0);
    Func_02009bd2(3, 0x1b0, 0x136, 0);
    Func_02009be2(27, 0x1b8, 134, 0x8000);
    Func_02009bf6(10, 0x1c6, 248, 0x3000);
    Func_0200b8c0(10, 6);
    Func_02009c2e(0x1b80000, -1, 0x1340000, 0x1000001);
    Func_0200b9b6();
    Func_0200b9ca();
    Func_0200b850(20);
    Call3(Func_0200b96c, 0, 0x2000, 0);
    Call3(Func_0200b978, 1, 0xa000, 0);
    Call3(Func_0200b984, 2, 0xa000, 0);
    Call3(Func_0200b990, 3, 0x2000, 40);
    Func_02009cb2(2, 1, 20);
    Call1(Func_0200b978_a, 0x1e6e);
    Func_02009c42(27);
    Func_02009cce(1, 0xc000, 0);
    Call2(Func_0200b9e6, 0x26666, 0x4ccc);
    Call4(Func_0200b9fc, 0x1b80000, -1, 0xb00000, 1);
    Call3(Func_0200b8fe, 27, 0x19999, 0xcccc);
    Call3(Func_0200b942, 27, 0x198, 134);
    Call3(Func_0200b94e, 27, 0x198, 152);
    Call3(Func_0200b95a, 27, 0x1a8, 164);
    Call2(Func_0200ba2a, 0x19999, 0x3333);
    Call4(Func_0200ba40, 0x1b80000, -1, 0x12c0000, 1);
    Call3(Func_0200b97c, 27, 0x1a8, 222);
    Call3(Func_0200b98a, 27, 0x1a8, 0x106);
    Func_0200ba34(27, 0x3000, 20);
    Func_0200b9ec(27, 1);
    Func_02009cde(27);
    Func_02009d64(2, 1, 20);
    Func_0200b9e4(27, 3);
    Func_0200ba0c(27, 1);
    Func_02009cfe(27);
    Func_02009d84(3, 2, 60);
    Func_02009d90(1, 0xe000, 60);
    Func_0200ba82(27, 0, 40);
    Func_0200ba3a(27, 1);
    Func_0200ba12(27, 2);
    Call3(Func_0200b9ee, 27, 0x1b0, 0x10c);
    Call3(Func_0200b9fc_a, 27, 0x1c4, 0x10c);
    Func_0200ba34_a(27, 1);
    Func_02009d6e(27, 0xd000);
    Func_0200ba6e(27, 2);
    Func_0200bac0(27, 0, 20);
    Func_02009df2(1, 0xc000, 20);
    Func_0200ba72(27, 4);
    Func_0200b9d8(40);
    Func_0200bae2(27, 0, 80);
    Func_0200baaa(27, 1);
    Func_0200b9f0(20);
    Func_0200ba98(27, 3);
    Func_0200b9fe(10);
    Func_02009dc8(27, 0x5000);
    Value2(Func_0200bb00, 27, 0);
    if (Value2(Func_0200ba38, 0, 0) == 0) {
        Func_0200bac4(27, 3);
        Func_02009dd6(27);
    } else {
        Func_0200bad4(27, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        Func_02009df6(27);
        Func_02009e7c(3, 2, 40);
        Func_0200bb1c(27, 1);
        Func_0200baf4(27, 3);
        Func_02009e16(27);
    }
    Func_02009e9c(2, 1, 20);
    action = (s32)Data_0200e7f0;
    Func_0200babe(1, action);
    Func_0200bac6(2, action);
    Func_0200bade(3, action);
    Call2(Func_0200bbce, 0x9999, 0x1333);
    Call4(Func_0200bbe8, 0x1b80000, -1, 0xb00000, 1);
    Call3(Func_0200baee, 0, 0x10000, 0x8000);
    Call3(Func_0200bb34, 0, 0x1a8, 0x110);
    Call3(Func_0200bb38, 0, 0x1a8, 164);
    Func_0200bad6(60);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x209;
    Func_02009f16(9, 0, 0);
    Call1(Func_0200bae4, 0x301);
    Call1(Func_0200baea, 0x927);
    Func_0200bc58(4);
}
