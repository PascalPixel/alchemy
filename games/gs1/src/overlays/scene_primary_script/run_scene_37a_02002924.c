#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020052e2();
void Func_020052fc();
void Func_02005336();
void Func_0200538c();
void Func_02005394();
void Func_020053b6();
void Func_020053ca();

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

void FieldScene_RunScene37a_02002924(void)
{
    u32 i;
    s32 record;

    Func_020052fc();
    if (Value1(Func_020052e2, 0x896) != 0) {
        Call1(Func_0200538c, 0xffd);
    } else {
        Call1(Func_02005394, 0xfff);
    }
    Func_020053b6(16, 0, 10);
    Call3(Func_020053ca, 16, 0xc000, 10);
    Func_02005336();
}
