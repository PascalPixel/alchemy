#include "types.h"

#define FieldScene_RunScene3aeSequenceA Func_02000ba0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002124();
void Func_02002148();
s32 Func_0200215c();
void Func_02002170();
void Func_0200218e();
void Func_020021b6();
void Func_020021bc();
void Func_020021c0();
void Func_020021e2();
void Func_020021e8();
void Func_020021fe();
void Func_02002216();
void Func_02002230();
void Func_02002232();
void Func_02002234();
void Func_0200223e();
void Func_02002242();
void Func_02002244();
void Func_0200224a();
void Func_0200224c();
void Func_02002258();
void Func_02002266();
void Func_0200226c();
void Func_02002272();
void Func_02002278();
void Func_0200228a();
void Func_0200228c();
void Func_02002298();
void Func_0200229e();
void Func_020022aa();
void Func_020022c2();
void Func_020022dc();
void Func_020022e2();
void Func_020022e8();
void Func_020022ee();
void Func_020022f6();
void Func_020022fc();
void Func_02002306();
void Func_0200230a();
void Func_02002310();
void Func_02002316();
void Func_02002318();
void Func_02002342();
void Func_0200234a();
void Func_0200234c();
void Func_02002352();
s32 Func_02002358();
void Func_0200236c();
void Func_02002374();
void Func_02002382();
s32 Func_02002386();
void Func_02002392();
void Func_02002398();
s32 Func_0200239a();
void Func_020023a0();
s32 Func_020023a4();
void Func_020023a6();
void Func_020023aa();
void Func_020023b0();
void Func_020023b2();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3aeSequenceA(void)
{
    u32 i;
    s32 record;

    Func_02002124();
    Func_02002230();
    Call3(Func_0200218e, 8, 0x1480000, 0x5900000);
    *(u8 *)(Func_0200215c(8) + 91) = 1;
    Func_0200223e();
    Func_0200224a();
    Func_02002148(20);
    Call4(Func_02002278, 1, -16, 0, 0x8000);
    Func_020021b6(1);
    Call3(Func_02002232, 0, 0xa000, 0);
    Func_02002170(20);
    Call1(Func_02002216, 0x1f89);
    Call3(Func_0200224a, 0, 0xa000, 0);
    Call3(Func_020021bc, 1, 0x19999, 0xcccc);
    Call3(Func_020021e8, 1, 232, 0x590);
    Call3(Func_0200226c, 1, 0x8000, 0);
    Call4(Func_0200229e, 0xb80000, -1, 0x5a00000, 1);
    Func_020022aa();
    Func_020021c0(10);
    Func_02002242(1, 6, 15);
    Func_0200224c(1, 6, 40);
    Func_0200228c(1, 0);
    Func_020021e2(20);
    Call4(Func_020022dc, 0x1080000, -1, 0x5a80000, 1);
    Func_020022e8();
    Func_020021fe(20);
    Call3(Func_020022e2, 8, 0x100, 50);
    Call3(Func_02002244, 8, 0x13333, 0x9999);
    Call3(Func_02002272, 8, 0x108, 0x590);
    Call3(Func_020022f6, 8, 0x8000, 0);
    Func_02002234(10);
    Func_02002306(1, 0, 0);
    Func_02002244(20);
    Func_0200224a(10);
    Func_020022c2(8, 4);
    Func_02002258(10);
    Func_02002310(8, 0);
    Func_02002266(20);
    Call3(Func_0200234a, 1, 0x102, 40);
    Func_02002278(30);
    Call3(Func_0200234c, 1, 0x8000, 0);
    Func_0200228a(50);
    Func_02002342(1, 0);
    Func_02002298(20);
    Call3(Func_0200236c, 1, 0x2000, 0);
    Func_020022aa(30);
    Call3(Func_020022e8, 1, 0x10000, 0x8000);
    Call3(Func_02002316, 1, 0x108, 0x5b8);
    Func_02002398(1, 0, 0);
    Call3(Func_020023a4, 0, 0x8000, 0);
    Call3(Func_020023b0, 8, 0x4000, 0);
    Func_020022ee(30);
    Func_020023a6(1, 0);
    Func_020022fc(10);
    Func_02002374(0, 3);
    Func_0200230a(30);
    Func_02002382(1, 3);
    Func_02002318(30);
    Call3(Func_02002352, 1, 0x13333, 0x9999);
    Func_02002392(1, 2);
    record = Value1(Func_02002358, 0);
    if (record != 0) {
        Func_02002382(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020023a0(1);
    Func_020023b2(1, 0, 0);
    ((void (*)())Func_02002358)(20);
    *(u8 *)(Func_02002386(8) + 91) = 0;
    Value2(Func_020023a4, 8, 2);
    record = Value1(Func_0200239a, 8);
    {
        s32 shown = *(s32 *)(record + 8) / 0x10000;

        *(u16 *)(record + 100) = shown;
    }
    {
        s32 shown = *(s32 *)(record + 16) / 0x10000;

        *(u16 *)(record + 102) = shown;
    }
    Func_020023aa();
}
