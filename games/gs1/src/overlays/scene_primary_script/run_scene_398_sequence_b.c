#include "types.h"

#define FieldScene_RunScene398SequenceB Func_02000304

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000c7c();
s32 Func_02000c84();
s32 Func_02000c8c();
void Func_02000c9c();
s32 Func_02000ca4();
void Func_02000cae();
s32 Func_02000cb0();
s32 Func_02000cba();
void Func_02000cd2();
void Func_02000ce0();
void Func_02000ce2();
void Func_02000ce4();
s32 Func_02000d0e();
void Func_02000d1e();
void Func_02000d26();
void Func_02000d28();
void Func_02000d2e();
void Func_02000d34();
void Func_02000d36();
void Func_02000d38();
void Func_02000d6a();
void Func_02000d72();
void Func_02000d7c();
void Func_02000d82();

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

void FieldScene_RunScene398SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 v5;

    rec7 = Value1(Func_02000c84, 11);
    rec8 = Func_02000c8c(12);
    if ((*(volatile s32 *)(rec7 + 8) >> 20) == 35) {
        if ((*(volatile s32 *)(rec7 + 16) >> 20) != 23) {
            goto L_02000330;
        }
        Call1(Func_02000c7c, 0x303);
    } else {
        L_02000330:;
        Call1(Func_02000c8c, 0x303);
    }
    if ((*(volatile s32 *)(rec8 + 8) >> 20) == 35) {
        if ((*(volatile s32 *)(rec8 + 16) >> 20) != 23) {
            goto L_02000350;
        }
        Call1(Func_02000c9c, 0x304);
    } else {
        L_02000350:;
        Call1(Func_02000cae, 0x304);
    }
    if (Value1(Func_02000ca4, 0x303) == 0) {
        record = Value1(Func_02000cb0, 0x304);
        if (record == 0) {
            goto L_020003c2;
        }
    }
    if (Value1(Func_02000cba, 0x302) == 0) {
        Func_02000ce2();
        Func_02000ce0(40);
        Func_02000d2e(210);
        v5 = 36;
        Func_02000d1e(17, 6);
        Call6(Func_02000cd2, 0, 1, 1, 1, v5, 22);
        Call6(Func_02000ce4, 0, 2, 1, 1, v5, 24);
        Func_02000d28();
    }
    Call1(Func_02000d0e, 0x302);
    goto L_02000414;
    L_020003c2:;
    if (Value1(Func_02000d0e, 0x302) != 0) {
        Func_02000d36();
        Func_02000d34(40);
        Func_02000d82(220);
        v5 = 36;
        Func_02000d72(17, 2);
        Call6(Func_02000d26, 1, 1, 1, 1, v5, 22);
        Call6(Func_02000d38, 1, 2, 1, 1, v5, 24);
        Func_02000d7c();
    }
    Call1(Func_02000d6a, 0x302);
    L_02000414:;
}
