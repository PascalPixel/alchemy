#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020025c4();
void Func_020025d6();
void Func_020025f6();
void Func_02002620();
void Func_02002688();
void Func_020026ac();
void Func_020026b0();
void Func_020026c8();
void Func_020026ce();
void Func_020026e8();
void Func_02002704();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

void FieldScene_RunScene38d_02000150(void)
{
    u32 i;
    s32 record;

    Func_020025c4();
    Call3(Func_020026c8, 14, 0x102, 0);
    Func_02002688(14, 2);
    Func_020025d6(40);
    Call1(Func_020026ac, 0x1764);
    Func_020026ce(14, 0, 20);
    Func_020026b0(14, 0, 0);
    Func_020025f6(20);
    Func_020026e8(14, 0, 10);
    Call3(Func_02002704, 14, 0xb000, 10);
    Func_02002620();
}
