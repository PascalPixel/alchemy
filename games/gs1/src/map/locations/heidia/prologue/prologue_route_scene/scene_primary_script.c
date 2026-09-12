#include "types.h"

#define FieldScene_RunScene3b2SequenceA Func_02001214
#define FieldScene_RunScene3b2_02001494 Func_02001494
#define FieldScene_RunScene3b2_0200167c Func_0200167c
typedef union {
    s32 w;
    s16 h[2];
} RecWord;

extern u8 Data_00000071[];
extern u8 Data_0000007b[];
extern u8 Data_0000007d[];
extern u8 Data_0000007e[];
extern u8 Data_00000086[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];

void Func_02001914();
void Func_0200191a();
void Func_020023cc();
void Func_0200265e();
void Func_020026be();
void Func_020028b4();
void Func_02003fe6();
void Func_02003ff8();
s32 Func_02003ffe();
void Func_02004044_a();
void Func_02004044_b();
s32 Func_02004054();
void Func_02004068();
void Func_02004078();
void Func_020040b4();
s32 Func_020040ba();
void Func_020040c0();
s32 Func_020040d0();
void Func_020040e0();
void Func_020040e6();
void Func_020040f0();
s32 Func_020040f4();
s32 Func_020040f6();
void Func_020040fc();
void Func_0200411c();
void Func_0200412c();
s32 Func_02004130();
s32 Func_02004132_a();
s32 Func_02004132_b();
void Func_02004138();
void Func_02004158();
s32 Func_0200416c();
s32 Func_0200416e();
void Func_02004174();
void Func_02004194();
void Func_020041a4();
s32 Func_020041a8();
s32 Func_020041ba_a();
s32 Func_020041ba_b();
s32 Func_020041d2();
void Func_020042a8();
s32 Func_0200428e();
void Func_02004290();
u8 *Func_020042a4();
void Func_020042b0();
s32 Func_020042da();
void Func_02004300();
s32 Func_02004318();
void Func_02004330();
void Func_020044f2();
void Func_02004508();
void Func_02004522();
void Func_02004530();
void Func_02004532();
void Func_02004560();
void Func_0200457e();
void Func_02004596();
void Func_020045a2();
void Func_020045bc();
void Func_020046c4();
s32 Func_020046dc();
s32 Func_02004714();
s32 Func_02004738();
s32 Func_0200473a();
s32 Func_0200475c();
s32 Func_0200475e();
void Func_02004762();
s32 Func_02004782();
void Func_02004786();
s32 Func_020047a6();
void Func_020047aa();
void Func_020047ce();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02001214(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02001494(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_0200167c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

s32 Func_02000f70(void)
{
    u8 *rec7;
    s32 scene;
    s32 cur;
    s32 lo;

    ((RecWord *)(*(u8 *volatile *)Data_03001ebc + 0x1c0))->w = 0x204;
    scene = Data_02000240_t[224][0];
    if (scene == (s32)Data_0000007b) {
        Func_020028b4();
        return 0;
    }
    if (scene == (s32)Data_0000007d) {
        if (Value1(Func_02003ffe, 0xef7) == 0) {
            Call6(Func_02003fe6, 0, 3, 1, 1, 13, 40);
            Call6(Func_02003ff8, 0, 2, 1, 1, 15, 40);
            Call3(Func_020040e6, 101, 0xd80000, 0x2880000);
        }
        if (Data_02000240_t[224][0] == scene) {
            if (Data_02000240_t[225][0] != 5) {
                if (Value1(Func_02004054, 0x8d1) == 0) {
                    return 0;
                }
            }
            Call1(Func_02004068, 0x8d1);
            Call6(Func_02004044_a, 0, 1, 1, 1, 13, 30);
            Call3(Func_02004132_a, 100, 0xd80000, 0x1e80000);
            return 0;
        }
    }
    cur = Data_02000240_t[224][0];
    if (cur == (s32)Data_00000071) {
        s32 rec;

        Func_020026be();
        rec = Func_020040d0(8);
        *(volatile s32 *)(rec + 56) = 0x810000;
        Func_02001914(9);
        Func_0200191a(10);
        if (Value1(Func_020040ba, 0x240) != 0) {
            rec7 = Value1(Func_020040f4, 11);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Func_02004078((s32)rec7, 4);
                Func_020040c0((s32)rec7, 0);
            }
            Call4(Func_020040e0, 0, 0x1300000, 0x1700000, 253);
        }
        if (Value1(Func_020040f6, 0x241) != 0) {
            rec7 = Value1(Func_02004130, 12);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Func_020040b4((s32)rec7, 4);
                Func_020040fc((s32)rec7, 0);
            }
            Call4(Func_0200411c, 0, 0x500000, 0x1700000, 253);
        }
        if (Value1(Func_02004132_b, 0x242) != 0) {
            rec7 = Value1(Func_0200416c, 13);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Func_020040f0((s32)rec7, 4);
                Func_02004138((s32)rec7, 0);
            }
            Call4(Func_02004158, 0, 0x600000, 0x1500000, 253);
        }
        if (Value1(Func_0200416e, 0x243) != 0) {
            rec7 = Value1(Func_020041a8, 14);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Func_0200412c((s32)rec7, 4);
                Func_02004174((s32)rec7, 0);
            }
            Call4(Func_02004194, 0, 0x900000, 0x1400000, 253);
            Call4(Func_020041a4, 0, 0x2f00000, 0x1400000, 253);
        }
        if (Value1(Func_020041ba_a, 0xfd7) != 0) {
            return 0;
        }
        Func_02004044_b(8);
        return 0;
    }
    lo = (s32)Data_0000007e;
    if (cur == lo) {
        if (Value1(Func_020041d2, 0xef4) == 0) {
            Call6(Func_020041ba_b, 0, 0, 1, 1, 37, 10);
            Call3(Func_020042a8, 100, 0x2580000, 0xa80000);
        }
    }
    cur = Data_02000240_t[224][0];
    if (cur >= lo) {
        if (cur <= (s32)Data_00000086) {
            Func_020023cc();
            if (Data_02000240_t[225][0] == 5) {
                Func_0200265e();
            }
        }
    }
    return 0;
}

void FieldScene_RunScene3b2SequenceA(void)
{
    u32 i;
    s32 rec7;
    u8 *rec8;
    s32 record;

    rec8 = Func_020042a4(8);
    rec7 = Value1_02001214(Func_0200428e, (Data_02000240_t[224][0] + (0x8d2 - (s32)Data_0000007e)));
    if (rec7 != 0) {
        Call3(Func_02004300, 8, 0x28a0000, 0xa80000);
        *(volatile s32 *)((s32)rec8 + 12) = -0x200000;
        record = Func_020042da(8);
        Func_02004290(record, 0);
        Func_02004330(8, 3);
        rec8[85] = 0;
        {
            u8 value = *(volatile u8 *)&rec8[35];

            rec8[35] = (u8)(value | 2);
        }
        Call6(Func_020042b0, 42, 10, 1, 1, 40, 10);
    } else {
        *(u8 *)(Func_02004318(8) + 85) = rec7;
    }
}

void FieldScene_RunScene3b2_02001494(void)
{
    u32 i;
    s32 record;

    Func_02004508();
    Call3_02001494(Func_02004532, 0, 0x6666, 0x3333);
    Func_02004596();
    Func_020045a2();
    Func_02004522((Data_02000240_t[224][0] + (0x8c8 - (s32)Data_0000007e)));
    Func_02004530(30);
    Call3_02001494(Func_020044f2, 0x200b2bc, 44, 7);
    Call3_02001494(Func_0200457e, 0, 3, -16);
    Func_020045bc(3);
    Func_02004560();
}

void FieldScene_RunScene3b2_0200167c(void)
{
    u32 i;
    s32 record;

    if (Value1_0200167c(Func_020046dc, 0x8c4) != 0) {
        Call6(Func_020046c4, 0, 0, 1, 1, 8, 21);
        Call3(Func_02004762, 15, 0x3c80000, 0x3c80000);
    } else {
        record = Func_0200473a(15);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
    if (Value1_0200167c(Func_02004714, 0x8c5) != 0) {
        Call3(Func_02004786, 16, 0x3c80000, 0x3c80000);
    } else {
        record = Func_0200475e(16);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
    if (Value1_0200167c(Func_02004738, 0x8c6) != 0) {
        Call3(Func_020047aa, 17, 0x3c80000, 0x3c80000);
    } else {
        record = Func_02004782(17);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
    if (Value1_0200167c(Func_0200475c, 0x8c7) != 0) {
        Call3(Func_020047ce, 18, 0x3c80000, 0x3c80000);
    } else {
        record = Func_020047a6(18);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
}
