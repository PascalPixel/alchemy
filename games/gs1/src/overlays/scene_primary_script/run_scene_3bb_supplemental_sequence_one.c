#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000714

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern s32 Data_02000240_t[][1];
extern u8 Data_0200c834[];
extern u8 Data_0200c838[];
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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunSupplementalSequenceOne(void)
{
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
