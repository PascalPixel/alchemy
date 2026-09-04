#include "types.h"

#define FieldScene_RunScene382_02000614 Func_02000614

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001d64();
void Func_02001fd0();
s32 Func_02002010();
void Func_02002024();
void Func_0200206e();
s32 Func_02002088();
void Func_020020b4();
void Func_020020d0();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene382_02000614(void)
{
    u32 i;
    s32 record;

    Func_02001fd0();
    Call1(Func_0200206e, 0x1223);
    Func_02001d64(9, 0, 2);
    Value2(Func_02002088, 9, 0);
    if (Value2(Func_02002010, 0, 0) != 0) {
        bump_step(1);
    }
    Func_020020b4(9, 0);
    Call3(Func_020020d0, 9, 0x5000, 0);
    Func_02002024();
}
