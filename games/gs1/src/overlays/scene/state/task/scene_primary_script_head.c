#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000714
#define FieldScene_RunScene3bbSequenceA Func_020008ec
#define FieldScene_RunScene3bb_02000b38 Func_02000b38
#define FieldScene_RunSecondActorInteraction Func_02001424
extern u8 Data_02000240[];
extern u8 Data_0200c834[];
extern u8 Data_0200c838[];
extern u8 Data_03001ebc[];

void Func_02004650();
s32 Func_0200465c();
void Func_02004666();
s32 Func_02004674();
void Func_02004678();
s32 Func_0200467e();
void Func_02004692();
void Func_020046a6();
void Func_020046ba();
void Func_020046ce();
void Func_020046ec();
void Func_02004700();
void Func_02004714();
void Func_02004728();
s32 Func_0200475c();
s32 Func_02004770();
s32 Func_02004784();
s32 Func_02004798();
s32 Func_020047b2();
s32 Func_020047b6();
s32 Func_020047ca();
s32 Func_020047de();
s32 Func_020047f2();
void Func_020047f6();
void Func_02004850();
void Func_020046ac();
void Func_0200489a();
void Func_020048a8();
void Func_020048b6();
void Func_020048c4();
void Func_020048d4();
void Func_020036a2();
void Func_02004950();
void Func_02004a74();
void Func_02004a88();
void Func_02004a98();
void Func_02004a9e();
void Func_02004aee();
void Func_02004afc();
void Func_02004b0a();
void Func_02004b28();
void Func_02004b30();
void Func_02004b30_a();
void Func_02004b3a();
void Func_02004b3c();
void Func_02004b42();
void Func_02004be0();
void Func_02001d44();
void Func_02001dd8();
void Func_02001e3c();
void Func_0200322e();
s32 Func_02003444();
void Func_020035b8();
s32 Func_02003632();
void Func_02004046();
void Func_02004274();
s32 Func_020045a4();
s32 Func_020045b0();
void Func_02005378();
void Func_020053d4();
void Func_02005412();
void Func_02005424();
s32 Func_02005462();
s32 Func_02005462_a();
void Func_02005470();
void Func_0200547c();
void Func_02005496();
void Func_020054a2();
void Func_020054ca();
void Func_020054dc();
void Func_020054f4();
void Func_02005502();
void Func_02005508();
void Func_02005520();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern s32 Data_02000240_t[][1];

    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern s32 Data_02000240_t[][1];

    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern s32 Data_02000240_t[][1];

    f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000b38(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern s16 Data_02000240_t[][1];

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern s16 Data_02000240_t[][1];

    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern s16 Data_02000240_t[][1];

    return f(a0, a1, a2);
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    extern s32 Data_02000240_t[][1];

    s32 i;
    s32 rec8;
    s32 rec7;
    s32 xa;
    s32 xb;
    s32 xd;
    s32 ya;
    s32 yb;
    s32 yd;
    s32 flag;
    s32 count;
    s32 record;
    s32 base5;

    rec8 = Func_02004674(Data_02000240_t[125][0]);
    for (i = 22; i <= 25; i++) {
        rec7 = Value1(Func_0200467e, i);
        *(u8 *)(rec7 + 91) = 0;
        xa = *(s32 *)(rec7 + 8);
        xb = *(s32 *)(rec8 + 8);
        xd = xa - xb;
        if (xd >= 0) {
            if (xd > 0x9ffff) {
                continue;
            }
        } else {
            xb = xb - xa;
            if (xb > 0x9ffff) {
                continue;
            }
        }
        ya = *(s32 *)(rec7 + 16);
        yb = *(s32 *)(rec8 + 16);
        yd = ya - yb;
        if (yd >= 0) {
            if (yd > 0x9ffff) {
                continue;
            }
        } else {
            yb = yb - ya;
            if (yb > 0x9ffff) {
                continue;
            }
        }
        if (Value1(Func_0200465c, 0x104) != 0) {
            yb = *(s32 *)(rec7 + 16);
        } else {
            yb = *(s32 *)(rec8 + 16);
            yd = *(s32 *)(rec7 + 44);
            yb = yb + yd;
        }
        *(s32 *)(rec8 + 16) = yb;
    }
    if (*(volatile s32 *)Data_0200c838 != 0
        && *(s32 *)(rec7 + 56) == (s32)0x80000000) {
        if (*(volatile s32 *)Data_0200c834 == 0) {
            Call6(Func_02004650, 58, 28, 7, 1, 58, 13);
        } else {
            Call6(Func_02004650, 58, 10, 1, 1, 58, 11);
        }
    } else {
        Call6(Func_02004666, 57, 11, 1, 1, 58, 11);
        Call6(Func_02004678, 58, 14, 7, 1, 58, 13);
    }
    base5 = (s32)Data_0200c838;
    if (*(volatile s32 *)base5 == 0) {
        flag = *(volatile s32 *)Data_0200c834 ^ 1;
        *(volatile s32 *)Data_0200c834 = flag;
        if (flag != 0) {
            record = Func_0200475c(22);
            Call4(Func_02004692, record, 0x3a80000, 0, 0xb80000);
            record = Func_02004770(23);
            Call4(Func_020046a6, record, 0x3c80000, 0, 0xf80000);
            record = Func_02004784(24);
            Call4(Func_020046ba, record, 0x3e80000, 0, 0xb80000);
            record = Func_02004798(25);
            Call4(Func_020046ce, record, 0x4080000, 0, 0xf80000);
            Func_020047f6(31, 11);
        } else {
            record = Func_020047b6(22);
            Call4(Func_020046ec, record, 0x3a80000, 0, 0xd80000);
            record = Func_020047ca(23);
            Call4(Func_02004700, record, 0x3c80000, 0, 0xd80000);
            record = Func_020047de(24);
            Call4(Func_02004714, record, 0x3e80000, 0, 0xd80000);
            record = Func_020047f2(25);
            Call4(Func_02004728, record, 0x4080000, 0, 0xd80000);
            Func_02004850(31, 10);
        }
    }
    count = *(volatile s32 *)base5 + 1;
    *(volatile s32 *)base5 = count;
    if ((u32)count > 119) {
        record = Value1(Func_020047b2, 0x104);
        if (record == 0) {
            *(volatile s32 *)base5 = record;
        }
    }
}

void FieldScene_RunScene3bbSequenceA(void)
{
    u32 i;
    s32 record;

    *(volatile s32 *)Data_0200c838 = 0;
    *(volatile s32 *)Data_0200c834 = 0;
    Call1(Func_020046ac, 0x2008715);
    Call3(Func_0200489a, 22, 0x3a80000, 0xd80000);
    Call3(Func_020048a8, 23, 0x3c80000, 0xd80000);
    Call3(Func_020048b6, 24, 0x3e80000, 0xd80000);
    Call3(Func_020048c4, 25, 0x4080000, 0xd80000);
    Func_020048d4(31, 10);
}

void FieldScene_RunScene3bb_02000b38(s32 a0)
{
    u32 i;
    s32 record;

    Func_02004a98(40);
    Func_02004a9e(41);
    Func_02004a74(1);
    Func_02004a88();
    Call3_02000b38(Func_02004aee, 8, 0x580000, 0x1000000);
    Call3_02000b38(Func_02004afc, 0, 0x780000, 0x1000000);
    Call3_02000b38(Func_02004b30, 8, 0x4000, 0);
    Call3_02000b38(Func_02004b3c, 0, 0x4000, 0);
    if (a0 < 0) {
        Func_02004b28(8, 10);
        Func_02004b30_a(0, 35);
    } else {
        Func_02004b3a(8, 8);
        Func_02004b42(0, 28);
    }
    Func_02004950(1);
    Call4(Func_02004be0, 0x680000, 0, 0xc00000, 0);
    Func_020036a2(a0);
    Func_02004b0a();
}

void FieldScene_RunSecondActorInteraction(s32 a0)
{
    extern s16 Data_02000240_t[][1];

    u32 i;
    s32 rec;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_0200322e();
    } else {
        Func_02005378();
        rec = Value2(Func_02003444, a0, 2);
        if (rec == 0) {
            Call1(Func_02005424, 0x20a2);
            Func_02001d44();
            Call2(Func_0200547c, 0x30000, 0x6000);
            Call4(Func_02005496, 0x3d80000, -1, 0xe80000, 1);
            Func_020054a2();
            Value2(Func_02005462, a0, 0);
            Func_02001dd8();
            Func_02005470(a0, 0);
            Func_02004046(0, 0x438, 0x108);
            Func_020053d4(15);
            Call3(Func_02005412, 0, 0x18000, 0xc000);
            Value3(Func_020045a4, 0, 0x438, 216);
            Value3(Func_020045b0, 0, 0x428, 216);
            Func_02001e3c();
            Func_02005520();
            Call4(Func_02005502, -1, -1, -1, 0);
            Func_020054ca(a0, 0);
            Func_02004274(0);
            Func_02005508(0, 0);
            Func_020035b8(a0, 2);
        } else {
            if (rec == 1) {
                Call1(Func_020054dc, 0x20a1);
                Func_020054f4(a0, 0);
            }
        }
        Value3(Func_02003632, rec, a0, 2);
        ((void (*)())Func_02005462_a)();
    }
}
