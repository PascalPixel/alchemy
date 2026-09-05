#include "types.h"

#define RunEventScript01 Func_020000dc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000f76[];
s32 Func_02001b4a();
void Func_02001b64();
void Func_02001b96();
s32 Func_02001bbe();
void Func_02001be8();
void Func_02001c1c();
void Func_02001c20();
void Func_02001c2e();
void Func_02001c34();
void Func_02001c48();
s32 Func_02001c56();
void Func_02001c60();
void Func_02001c68();
void Func_02001c88();
void Func_02001cac();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void RunEventScript01(void)
{
    u32 i;
    s32 record;
    s32 base5_f76;

    Func_02001b64();
    if (Value1(Func_02001b4a, 0x815) != 0) {
        Call1(Func_02001c1c, 0x11c4);
        Func_02001c34(12, 0);
    } else {
        base5_f76 = (s32)Data_00000f76;
        Func_02001c2e(base5_f76);
        Func_02001c20(12, 0, 10);
        Func_02001c20(12, 2);
        Func_02001b96(6);
        Value2(Func_02001c56, 12, 0);
        if (Value2(Func_02001bbe, 0, 0) == 0) {
            Func_02001c60((base5_f76 + 1));
        } else {
            Func_02001c68((base5_f76 + 2));
        }
        Func_02001c48(12, 3);
        Func_02001c88(12, 0);
        Call3(Func_02001cac, 12, 0xc000, 10);
    }
    Func_02001be8();
}
