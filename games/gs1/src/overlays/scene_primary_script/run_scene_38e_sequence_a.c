#include "types.h"

#define FieldScene_RunScene38eSequenceA Func_020005dc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001720[];
void Func_0200105a();
void Func_02001094();
void Func_020010b4();
void Func_02001140();
void Func_0200115e();
void Func_0200118e();
void Func_020011a0();
void Func_020011a2();
void Func_020011bc();
void Func_020011cc();
void Func_020011d2();
void Func_020011d6();
void Func_020011da();
void Func_020011ee();
void Func_020011f6();
void Func_0200120a();
void Func_02001216();
void Func_02001218();
void Func_02001226();
void Func_02001228();
s32 Func_0200122c();
void Func_02001234();
void Func_0200123a();
void Func_02001240();
void Func_02001244();
void Func_0200124c();
void Func_02001258();
s32 Func_0200125a();
void Func_0200126c();
void Func_02001272();
void Func_02001286();
s32 Func_0200128c();
void Func_02001294();
void Func_02001296();
void Func_0200129a();
void Func_0200129c();
void Func_0200129e();
void Func_020012a6();
void Func_020012b8();
void Func_020012c4();
void Func_020012d2();
void Func_020012d8();
void Func_020012da();
void Func_020012de();
s32 Func_020012e4();
void Func_020012e6();
void Func_020012fc();
u8 *Func_02001306();
void Func_02001312();
void Func_02001322();
void Func_0200132a();
void Func_0200132c();
void Func_0200132e();
void Func_02001334();
void Func_02001342();
void Func_02001358();
void Func_0200136e();
void Func_02001380();
void Func_02001382();
void Func_0200138c();
void Func_020013a2();
void Func_020013a4();
void Func_020013b4();
void Func_02001450();
void Func_02001456();
void Func_02001462();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene38eSequenceA(void)
{
    u32 i;
    u8 *record;

    Func_020010b4();
    Call4(Func_0200118e, -1, -1, -1, 0);
    Call4(Func_020011a0, 0x1400000, -1, 0x1400000, 0);
    Func_02001094();
    Func_0200105a(1);
    Call3(Func_02001140, 0, 0x1400000, 0x1740000);
    Func_020011cc();
    Call2(Func_020011bc, 0x3333, 0x666);
    Call4(Func_020011d6, 0x1400000, -1, 0x1220000, 1);
    Call3(Func_02001140, 0, 0x9999, 0x4ccc);
    Call3(Func_0200115e, 0, 0x140, 0x136);
    Call3(Func_020011da, 11, 0x3000, 10);
    Func_020011a2(11, 2);
    Call3(Func_020011f6, 11, 0x100, 60);
    Func_020011cc((s32)Data_00001720);
    Func_020011ee(11, 0, 10);
    Call3(Func_0200120a, 12, 0x5000, 10);
    Func_020011d2(12, 2);
    Call3(Func_02001226, 12, 0x100, 60);
    Func_02001218(12, 0, 20);
    Call3(Func_02001234, 11, 0x1000, 0);
    Call3(Func_02001240, 12, 0x7000, 40);
    Call3(Func_0200124c, 11, 0x3000, 0);
    Call3(Func_02001258, 12, 0x5000, 10);
    Func_02001228(11, 1);
    Call3((void (*)())Func_0200125a, 11, 0, 10);
    Func_0200123a(12, 1);
    Value2(Func_0200125a, 12, 0);
    Call3(Func_02001286, 0, 0xe000, 0);
    while (Value2(Func_0200122c, 0, 0) != 0) {
        Call3(Func_0200129c, 12, 0x100, 60);
        Call1(Func_02001272, 0x1724);
        Call3(Func_02001294, 12, 0, 10);
        Func_0200126c(12, 2);
        Func_02001294(12, 0);
    }
    Func_02001216(10);
    Call3(Func_020012d2, 11, 0x3000, 0);
    Call3(Func_020012de, 12, 0x5000, 20);
    Func_02001296(11, 3);
    Func_020012a6(12, 3);
    Func_02001244(20);
    Func_020012c4(11, 1);
    Call1(Func_020012da, 0x1726);
    Func_020012fc(11, 0, 10);
    Call3(Func_0200129a, 11, 0x10000, 0x8000);
    Call3(Func_020012b8, 11, 0x13a, 0x118);
    Call3(Func_02001334, 0, 0xc000, 20);
    Func_0200132e(11, 0, 40);
    Call2(Func_02001358, 0, 0x102);
    Func_0200129e(60);
    if (Value1(Func_0200128c, 0x84a) == 0) {
        Call3(Func_020012e6, 12, 0x10000, 0x8000);
        *(u8 *)(Func_020012e4(12) + 90) &= 254;
        Call3(Func_02001312, 12, 0x15a, 0x107);
        Func_020012d8(1);
        {
            u8 *record = Func_02001306(12);
            u8 value = *(volatile u8 *)&record[90];
        
            record[90] = (u8)(value | 1);
        }
    }
    Call3(Func_02001322, 11, 0xcccc, 0x6666);
    Call3(Func_0200132c, 0, 0xcccc, 0x6666);
    Call3(Func_02001342, 11, 0x148, 0x106);
    Call3(Func_02001358, 0, 0x148, 0x116);
    Func_02001380(11, 1);
    Func_02001140();
    Func_0200132a(40);
    Call3(Func_0200136e, 0, 0x148, 242);
    Call3(Func_02001382, 11, 0x148, 242);
    Func_020013a4(11, 0, 0);
    Func_020013a2(0);
    Func_020013b4(0, 0, 0);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x201;
    Func_02001456();
    Func_02001462();
    Func_02001450(10);
    Func_0200138c();
}
