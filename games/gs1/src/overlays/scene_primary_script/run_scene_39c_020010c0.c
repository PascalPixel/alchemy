#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02006b38();
s32 Func_02006b4c();
s32 Func_02006b56();
s32 Func_02006b60();
void Func_02006b6e();
void Func_02006b72();
void Func_02006b7a();
void Func_02006b98();
void Func_02006bb4();
void Func_02006bd8();
void Func_02006d08();
void Func_02006d16();
void Func_02006d22();
void Func_02006d66();
void Func_02006d88();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene39c_020010c0(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02006b4c, 0x310) != 0) {
        record = Value1(Func_02006b56, 0x311);
        if (record == 0) {
            goto L_02001140;
        }
        record = Value1(Func_02006b60, 0x312);
        if (record == 0) {
            goto L_02001140;
        }
        Call1(Func_02006b72, 0x876);
        Func_02006b98(30);
        Call3(Func_02006b38, 0x10000, 0x10000, 0x10000);
        Func_02006d66(141);
        Func_02006bb4(60);
        *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
        Func_02006d16();
        Func_02006d22();
        Call1(Func_02006d88, 0x121);
        Call3(Func_02006b6e, -1, -1, 0xe666);
        Func_02006b7a();
        Func_02006d08(13);
    } else {
        L_02001140:;
        Call1(Func_02006bd8, 0x876);
    }
}
