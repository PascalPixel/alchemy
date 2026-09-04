#include "types.h"

#define FieldScene_RunScene3c6_02000078 Func_02000078

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020015ce();
void Func_020015d4();
s32 Func_020015da();
void Func_02001614();
void Func_0200165a();
void Func_02001672();
void Func_02001686();

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

void FieldScene_RunScene3c6_02000078(s32 a0)
{
    u32 i;
    s32 record;

    record = Func_020015ce();
    *(volatile s32 *)(record + 24) = 0x10000;
    record = Value1(Func_020015da, a0);
    *(volatile s32 *)(record + 28) = 0x10000;
    Call1(Func_0200165a, 0x26af);
    Func_02001672(a0, 0);
    Call3(Func_02001686, a0, 0xc000, 0);
    Func_020015d4(20);
    Func_02001614(a0, 0x2009638);
}
