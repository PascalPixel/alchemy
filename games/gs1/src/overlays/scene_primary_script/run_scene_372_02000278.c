#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000426();
void Func_020017b6();
void Func_0200491e();
s32 Func_0200495a();
s32 Func_02004974();
s32 Func_0200497e();
void Func_02004994();
void Func_020049b8();
void Func_02004a32();
void Func_02004b3c();

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

void FieldScene_RunScene372_02000278(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_0200495a, 0x206) == 0) {
        Func_02004b3c(158);
        Call3(Func_0200491e, 0x200d7a0, 45, 39);
    }
    if (Value1(Func_02004974, 0x835) == 0) {
        record = Value1(Func_0200497e, 0x831);
        if (record != 0) {
            goto L_020002b4;
        }
        Func_020017b6();
        Call1(Func_02004994, 0x206);
    } else {
        L_020002b4:;
        Call3(Func_02004a32, 0, 0x106, 0x325);
        Func_020049b8(3);
        Func_02000426(6);
    }
}
