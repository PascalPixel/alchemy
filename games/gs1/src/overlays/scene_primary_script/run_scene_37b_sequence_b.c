#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_0200aa50[];
extern u8 Data_03001ebc[];
extern u16 Data_02000240_t[][1];
void Func_0200157c();
void Func_02001a82();
void Func_0200252c();
void Func_020034b4();
void Func_020034b8();
s32 Func_020034ee();
u8 *Func_02003506();
s32 Func_02003512();
s32 Func_0200351c();
void Func_0200354a();
void Func_02003568();
void Func_0200357a();
void Func_0200357c();
void Func_02003586();
void Func_0200359e();
void Func_020035ac();
void Func_020035b4();
void Func_020035ba();
s32 Func_020035be();
s32 Func_020035c8();
void Func_020035cc();
void Func_020035d6();
void Func_020035f0();
void Func_020035f8();
void Func_02003610();
void Func_02003620();
void Func_0200363c();
void Func_0200364a();
void Func_02003658();
void Func_02003662();
s32 Func_0200366c();
void Func_0200367a();
void Func_0200368e();
s32 Func_02003698();
void Func_0200369e();
void Func_020036ba();
s32 Func_020036c4();
void Func_020036ca();
void Func_020036de();
void Func_020036ee();
void Func_0200370a();
s32 Func_02003714();
void Func_0200371a();
void Func_0200372e();
void Func_02003746();
void Func_0200375a();
s32 Func_02003764();
void Func_0200376a();
void Func_0200377c();
void Func_02003796();
void Func_020037a8();
s32 Func_020037b2();
void Func_020037b8();
void Func_020037ca();
void Func_020037e6();
s32 Func_02003810();
void Func_02003834();
s32 Func_02003858();
void Func_0200386e();
s32 Func_0200387a();
void Func_0200388e();
s32 Func_02003890();
void Func_02003898();
s32 Func_0200389a();
void Func_020038aa();
s32 Func_020038ae();
void Func_020038b0();
s32 Func_020038be();
s32 Func_020038e6();
s32 Func_020038e8();
void Func_020038f8();
void Func_02003902();
s32 Func_02003910();
s32 Func_02003914();
void Func_02003920();
void Func_02003926();
void Func_020039d4();
void Func_020039e0();

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

s32 Func_0200105c(void)
{
    u8 *record;

    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x204;
    Func_0200252c();
    Call1(Func_020034b8, 0x144);
    record = (u8 *)Value1(Func_020034ee, 18);
    record[89] = 0;
    record += 35;
    {
        u8 value = *(volatile u8 *)record;

        *record = (u8)(value | 2);
    }
    Func_020034b4((s32)Func_02003506(18), 0);
    *(u8 *)(Func_02003512(18) + 35) &= 254;
    Func_020035b4(18, 1);
    if ((u32)((Data_02000240_t[225][0] - 3) << 16) > 0x10000) {
        Func_0200357c(5, 0, 0);
        Func_02003586(1, 0, 0);
    }
    if (Value1(Func_0200351c, 0x818) != 0) {
        Call3(Func_0200359e, 18, 0x1200000, 0xb20000);
        Call3(Func_020035ac, 17, 0x6480000, 0x6480000);
        Call3(Func_020035ba, 10, 0xe80000, 0x780000);
        Call3(Func_020035c8, 12, 0x1580000, 0x780000);
        Call3(Func_020035d6, 10, 0xe80000, 0x780000);
        Call6(Func_0200354a, 0, 59, 15, 38, 4, 3);
        Call3(Func_020035f8, 12, 0x1580000, 0x780000);
        Call6(Func_02003568, 4, 59, 17, 38, 4, 3);
        Call6(Func_0200357a, 8, 60, 17, 39, 2, 2);
        Call6(Func_0200363c, 0, 1, 2, 1, 17, 7);
    } else if (Value1(Func_020035be, 0x816) != 0
               && Value1(Func_020035c8, 0x817) != 0) {
        Call3(Func_0200364a, 10, 0xe80000, 0x780000);
        Call3(Func_02003658, 12, 0x1580000, 0x780000);
        Call6(Func_020035cc, 0, 28, 17, 8, 2, 1);
        Call3(Func_0200367a, 10, 0xe80000, 0x780000);
        Call6(Func_020035f0, 0, 59, 15, 38, 4, 3);
        Call3(Func_0200369e, 12, 0x1580000, 0x780000);
        Call6(Func_02003610, 4, 59, 17, 38, 4, 3);
        Call6(Func_02003620, 8, 60, 17, 39, 2, 2);
        Call6(Func_0200363c, 0, 0, 2, 1, 17, 8);
    } else {
        if (Value1(Func_0200366c, 0x816) != 0) {
            Call3(Func_020036ee, 10, 0xe80000, 0x780000);
            Call6(Func_02003662, 0, 59, 15, 38, 4, 3);
        }
        if (Value1(Func_02003698, 0x817) != 0) {
            Call3(Func_0200371a, 12, 0x1580000, 0x780000);
            Call6(Func_0200368e, 4, 59, 17, 38, 4, 3);
        }
    }
    if (Value1(Func_020036c4, 0x80b) != 0) {
        Call3(Func_02003746, 9, 0x1f80000, 0x980000);
        Call6(Func_020036ba, 2, 28, 34, 10, 2, 1);
        Call6(Func_020036ca, 2, 30, 16, 10, 2, 1);
        Call6(Func_020036de, 0, 55, 32, 40, 4, 3);
    }
    if (Value1(Func_02003714, 0x80c) != 0) {
        Call3(Func_02003796, 11, 0x2880000, 0x980000);
        Call6(Func_0200370a, 4, 28, 36, 10, 2, 1);
        Call6(Func_0200371a, 4, 30, 18, 10, 2, 1);
        Call6(Func_0200372e, 4, 55, 36, 40, 4, 3);
    }
    if (Value1(Func_02003764, 0x80d) != 0) {
        Call3(Func_020037e6, 13, 0x1f80000, 0xc80000);
        Call6(Func_0200375a, 2, 29, 34, 11, 2, 1);
        Call6(Func_0200376a, 2, 31, 16, 11, 2, 1);
        Call6(Func_0200377c, 0, 58, 32, 43, 4, 1);
    }
    if (Value1(Func_020037b2, 0x80e) != 0) {
        Call3(Func_02003834, 15, 0x2880000, 0xc80000);
        Call6(Func_020037a8, 4, 29, 36, 11, 2, 1);
        Call6(Func_020037b8, 4, 31, 18, 11, 2, 1);
        Call6(Func_020037ca, 4, 58, 36, 43, 4, 1);
    }
    {
    s16 *state = (s16 *)Data_02000240_t;

    if (state[225] == 3) {
        if (Value1(Func_02003810, 0x30a) != 0) {
            Func_0200388e(1, 0, 0);
            Func_02003898(5, 0, 0);
        } else if (Value1(Func_02003858, 0x109) == 0) {
            Func_0200157c();
            Call1(Func_0200386e, 0x30a);
        }
    }
    if (state[225] == 4) {
        if (Value1(Func_0200387a, 0x30b) != 0) {
            Func_020038f8(1, 0, 0);
            Func_02003902(5, 0, 0);
        } else if (Value1(Func_0200389a, 0x109) == 0) {
            Func_02001a82();
            Call1(Func_020038b0, 0x30b);
        }
    }
    }
    if (Value1(Func_020038ae, 0x814) != 0) {
        Func_020039e0(141);
        Call3(Func_020038b0, 0x10000, 0x10000, 0x10000);
        Func_020039d4();
    }
    return 0;
}

