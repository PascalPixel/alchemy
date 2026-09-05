#include "types.h"

#define FieldScene_RunPrimarySequence Func_02000f70

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
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
void Func_02004044();
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
s32 Func_02004132();
void Func_02004138();
void Func_02004158();
s32 Func_0200416c();
s32 Func_0200416e();
void Func_02004174();
void Func_02004194();
void Func_020041a4();
s32 Func_020041a8();
s32 Func_020041ba();
s32 Func_020041d2();
void Func_020042a8();

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

typedef union {
    s32 w;
    s16 h[2];
} RecWord;

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
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
            Call6(Func_02004044, 0, 1, 1, 1, 13, 30);
            Call3(Func_02004132, 100, 0xd80000, 0x1e80000);
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
        if (Value1(Func_02004132, 0x242) != 0) {
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
        if (Value1(Func_020041ba, 0xfd7) != 0) {
            return 0;
        }
        Func_02004044(8);
        return 0;
    }
    lo = (s32)Data_0000007e;
    if (cur == lo) {
        if (Value1(Func_020041d2, 0xef4) == 0) {
            Call6(Func_020041ba, 0, 0, 1, 1, 37, 10);
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
