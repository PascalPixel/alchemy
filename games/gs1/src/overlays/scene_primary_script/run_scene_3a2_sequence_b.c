#include "types.h"

#define FieldScene_RunScene3a2SequenceB Func_02000ac0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02001fb4();
s32 Func_02001fe4();
void Func_02001fe6();
void Func_02001ff4();
void Func_02002012();
void Func_0200204a();
s32 Func_02002064();
void Func_02002066();
void Func_0200208c();
void Func_020020ae();

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
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3a2SequenceB(void)
{
    u32 i;
    s32 record;

    Func_02001fb4();
    Call1(Func_0200204a, 0x18b9);
    Call3(Func_0200208c, 10, 0x105, 60);
    Value2(Func_02002064, 10, 0);
    if (Value2(Func_02001fe4, 0, 0) == 1) {
        bump_step(1);
    }
    Func_02001fe6(20);
    Func_02002066(10, 4);
    Func_02001ff4(20);
    Func_020020ae(10, 0, 20);
    Func_02002012();
}
