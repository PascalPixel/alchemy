#include "types.h"

#define FieldScene_RunPrimarySequence Func_02001804

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200609a();
void Func_020060f0();
void Func_02007cd6();
s32 Func_02007d04();
void Func_02007d0e();
void Func_02007d14();
s32 Func_02007d24();
void Func_02007d58();
void Func_02007d66();
void Func_02007d86();
void Func_02007d94();
void Func_02007da6();
void Func_02007dbc();
s32 Func_02007dcc();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunPrimarySequence(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 record;

    Func_02007cd6();
    Func_02007dbc(a1);
    Value2(Func_02007dcc, a0, 0);
    if (Value2(Func_02007d04, 0, 0) == 0) {
        Func_0200609a(a0);
        Func_02007d86(a0, 2);
        record = Value1(Func_02007d24, 0);
        if (record != 0) {
            Func_02007d66(a0, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_02007d94(a0);
        Func_02007da6(a0, 0, 0);
        Call1(Func_02007d0e, 0x300);
        Func_02007d14(a2);
    } else {
        bump_step(1);
        Func_020060f0(a0);
    }
    Func_02007d58();
}
