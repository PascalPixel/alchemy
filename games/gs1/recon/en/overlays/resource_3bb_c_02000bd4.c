#include "types.h"

#define FieldScene_RunScene3bbSequenceB Func_02000bd4

extern u8 Data_02000240[];
extern u8 Data_02000240_t[][2];
void Func_020014d0();
s32 Func_02004092();
s32 Func_0200409a();
void Func_02004a78();
void Func_02004a8e();
void Func_02004aa2();
void Func_02004ae2();
void Func_02004af4();
void Func_02004b10();
void Func_02004b24();
s32 Func_02004b40();
void Func_02004b6c();
u8 *Func_02004b78();
void Func_02004b92();
s32 Func_02004b9a();
u8 *Func_02004bb2();
s32 Func_02004bb4();
void Func_02004bd0();
s32 Func_02004bd2();
u8 *Func_02004bf4();
s32 Func_02004bf8();
s32 Func_02004c08();
void Func_02004c0e();
s32 Func_02004c36();
void Func_02004c3a();
void Func_02004c66();
s32 Func_02004c6a();
s32 Func_02004c76();
void Func_02004c8e();
s32 Func_02004ca8();
s32 Func_02004ca8_a();
s32 Func_02004cb4();
void Func_02004ccc();
s32 Func_02004cdc();
s32 Func_02004cde();
s32 Func_02004ce6();
void Func_02004ce8();
s32 Func_02004d08();
void Func_02004d0a();
void Func_02004d1e();
s32 Func_02004d28();
s32 Func_02004d28_a();
u8 *Func_02004d44();
s32 Func_02004d68();
s32 Func_02004d68_a();
void Func_02004d72();
void Func_02004d76();
u8 *Func_02004d86();
s32 Func_02004d9c();
u8 *Func_02004dc6();
s32 Func_02004e16();
void Func_02004e44();
s32 Func_02004e60();
void Func_02004e90();
void Func_02004ede();
void Func_02004f32();
void Func_02004f9e();

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 Func_02000bd4(void)
{
    u32 i;
    u8 *rec7;
    s32 rec8;
    s32 record;
    s32 r6;
    s32 base6_1a;
    s32 none;
    s32 base6_16;
    s32 base7_3a;
    s32 base5_2008715;
    s32 v7;
    s32 v3;

    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0;
    Call1(Func_02004ae2, 0x144);
    Call6(Func_02004a8e, 14, 11, 12, 4, 100, 11);
    Call6(Func_02004aa2, 48, 10, 5, 6, 120, 10);
    base6_1a = 26;
    do {
        rec7 = Func_02004b78(base6_1a);
        Func_02004a78((s32)rec7, 4);
        rec7[85] = 0;
        base6_1a = (base6_1a + 1);
        *(s32 *)((s32)rec7 + 12) = 0;
        *(u8 *)((((s32)rec7 + 85) - 50)) = 2;
    } while (base6_1a <= 30);
    *(u8 *)(Func_02004b9a(18) + 35) = 2;
    if (Value1(Func_02004b40, 0x330) != 0) {
        rec7 = Func_02004bb2(30);
        *(s32 *)((s32)rec7 + 8) = 0x1500000;
        *(s32 *)((s32)rec7 + 12) = -0x80000;
        *(s32 *)((s32)rec7 + 16) = 0x1080000;
        Call6(Func_02004b10, 19, 16, 1, 1, 20, 16);
        Call6(Func_02004b24, 20, 80, 1, 1, 21, 80);
    } else {
        rec7 = Func_02004bf4(30);
        Func_02004af4((s32)rec7, 3);
        *(s32 *)((s32)rec7 + 12) = 0x100000;
    }
    *(u8 *)(Func_02004c08(11) + 35) = 2;
    none = 0;
    if (Value1(Func_02004bb4, 0x335) != 0) {
        Call6(Func_02004b6c, 35, 78, 1, 1, 35, 77);
    }
    if (Value1(Func_02004bd2, 0x333) != 0) {
        Func_02004c8e(19, 4);
        Call6(Func_02004b92, 32, 37, 1, 4, 32, 77);
    }
    if (Value1(Func_02004bf8, 0x331) != 0) {
        *(u8 *)(Func_02004c6a(20) + 85) = none;
        *(u8 *)(Func_02004c76(20) + 35) = 2;
        Func_02004ccc(20, 5);
        Call6(Func_02004bd0, 46, 17, 1, 1, 44, 17);
    }
    if (Value1(Func_02004c36, 0x332) != 0) {
        *(u8 *)(Func_02004ca8(21) + 85) = none;
        *(u8 *)(Func_02004cb4(21) + 35) = 2;
        Func_02004d0a(21, 5);
        Call6(Func_02004c0e, 46, 17, 1, 1, 50, 17);
    }
    rec7 = Value1(Func_02004cdc, 32);
    rec7[85] = none;
    *(u8 *)((((s32)rec7 + 85) - 50)) = 2;
    Call6(Func_02004c3a, 52, 28, 1, 3, (*(s32 *)((s32)rec7 + 8) >> 20), 10);
    rec7 = Value1(Func_02004d08, 33);
    rec7[85] = none;
    *(u8 *)((((s32)rec7 + 85) - 50)) = 2;
    Call6(Func_02004c66, 52, 28, 1, 3, (*(s32 *)((s32)rec7 + 8) >> 20), 13);
    rec8 = Value1(Func_02004ce6, 0x340);
    if (rec8 == 0) {
    }
    rec7 = Func_02004d44(12);
    *(s32 *)((s32)rec7 + 8) = ((r6 << 20) + 0x80000);
    rec7[85] = none;
    *(u8 *)((((s32)rec7 + 85) - 50)) = 2;
    Call6(Func_02004ca8_a, 71, 16, 1, 1, r6, 16);
    rec8 = Value1(Func_02004d28, 0x348);
    if (rec8 == 0) {
    }
    rec7 = Func_02004d86(13);
    *(s32 *)((s32)rec7 + 8) = ((r6 << 20) + 0x80000);
    rec7[85] = none;
    *(u8 *)((((s32)rec7 + 85) - 50)) = 2;
    Call6(Func_02004ce8, 71, 16, 1, 1, r6, 16);
    rec8 = Value1(Func_02004d68, 0x350);
    if (rec8 == 0) {
    }
    rec7 = Func_02004dc6(14);
    *(s32 *)((s32)rec7 + 8) = ((r6 << 20) + 0x80000);
    rec7[85] = none;
    *(u8 *)((((s32)rec7 + 85) - 50)) = 2;
    Call6(Func_02004d28_a, 71, 16, 1, 1, r6, 16);
    Func_020014d0();
    Func_02004e44(31, 10);
    if (Value1(Func_02004d9c, 0x334) != 0) {
        base6_16 = 22;
        base7_3a = 58;
        do {
            rec7 = Value1(Func_02004e16, base6_16);
            rec7[35] = 2;
            Func_02004d1e((s32)rec7, 4);
            base6_16 = (base6_16 + 1);
            Call6(Func_02004d72, 56, 13, 1, 1, base7_3a, 13);
            base7_3a = (base7_3a + 2);
        v7 = base7_3a;
        } while (base6_16 <= 25);
        Func_02004e90(31, 10);
        Func_02004f9e(31);
    } else {
        base6_16 = 22;
        v7 = 0x8000;
        do {
            rec7 = Value1(Func_02004e60, base6_16);
            rec7[35] = 2;
            ((void (*)())Func_02004d68_a)((s32)rec7, 4);
            base6_16 = (base6_16 + 1);
            *(s32 *)((s32)rec7 + 48) = 0x8000;
            *(s32 *)((s32)rec7 + 52) = 0x3333;
        } while (base6_16 <= 25);
        base5_2008715 = 0x2008715;
        Value2(Func_02004cde, base5_2008715, 0xc85);
        Func_02004d76(base5_2008715, 1);
    }
    Func_02004ede(8, 9);
    Data_02000240_t[249][0] = 0;
    Value2(Func_02004092, 41, 89);
    Value2(Func_0200409a, 40, 77);
    Func_02004f32(8, 1);
    if ((u32)(*(s16 *)(0x2000402) - 1) > 4) {
        v3 = (*(s16 *)(0x2000402) - 1);
    } else {
        v3 = *(s32 *)((v3 << 2) + 33591176);
    }
    return 0;
}
