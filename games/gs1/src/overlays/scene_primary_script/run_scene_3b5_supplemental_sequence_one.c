#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000728

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020009e8();
void Func_0200145c();
s32 Func_020014ce();
s32 Func_020014d8();
s32 Func_020014f0();
s32 Func_020014f6();
void Func_020014f8();
s32 Func_02001500();
s32 Func_02001506();
s32 Func_02001518();
s32 Func_02001520();
void Func_0200155a();
s32 Func_02001586();
void Func_02001588();
void Func_0200159a();
void Func_020015a8();
void Func_020015b6();
void Func_020015c4();
void Func_020015d2();
void Func_020015e0();
void Func_020015ee();
void Func_02001608();
void Func_02001624();
void Func_0200167c();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

s32 Func_02000728(s32 a0)
{
    u32 i;
    s32 record;
    s32 handler;
    s32 hidden;

    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Call3(Func_020014f8, 16, 0x1600000, 0x1600000);
    Call2(Func_020014d8, 16, 0x2008ec0);
    record = Value1(Func_020014ce, 16);
    handler = 0x2008171;
    {
        volatile u16 *target = (volatile u16 *)(record + 100);
        s32 shown = 1;

        *target = shown;
    }
    *(volatile s32 *)(record + 108) = handler;
    hidden = 0;
    Value3(Func_02001520, 17, 0x1700000, 0x1400000);
    Call2(Func_02001500, 17, 0x2008f90);
    record = Value1(Func_020014f6, 17);
    *(volatile u16 *)(record + 100) = hidden;
    *(volatile s32 *)(record + 108) = handler;
    record = Func_02001506(14);
    *(volatile s32 *)(record + 108) = 0x20086e9;
    if (Value1(Func_020014d8, 0x8c1) != 0) {
        Call3(Func_0200155a, 28, 0x13c0000, 0x1480000);
    }
    if (Value1(Func_020014f0, 0x201) != 0) {
        Func_0200145c();
    }
    if (Value1(Func_02001500, 0x200) != 0) {
        Func_020009e8();
        Func_02001588(8, 4);
    }
    if (Value1(Func_02001518, 0x950) != 0) {
        Call3(Func_0200159a, 20, 0x2080000, 0x2300000);
        Call3(Func_020015a8, 21, 0x2080000, 0x2300000);
        Call3(Func_020015b6, 22, 0x2080000, 0x2300000);
        Call3(Func_020015c4, 24, 0x2080000, 0x2300000);
        Call3(Func_020015d2, 25, 0x2080000, 0x2300000);
        Call3(Func_020015e0, 26, 0x2080000, 0x2300000);
        Call3(Func_020015ee, 27, 0x2080000, 0x2300000);
    } else {
        if (Value1(Func_02001586, 0x962) != 0) {
            Call3(Func_02001608, 27, 0x1180000, 0x500000);
            Call3(Func_0200167c, 27, 0x2000, 0);
            Func_02001624(27, 1);
        }
    }
    return 0;
}
