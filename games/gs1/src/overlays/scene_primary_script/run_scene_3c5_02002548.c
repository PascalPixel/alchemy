#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02005368();
void Func_02005374();
void Func_02005382();
void Func_0200539c();
s32 Func_020053be();
void Func_020053d0();
void Func_020053d8();
void Func_020053dc();
void Func_020053e0();
void Func_02005402();
void Func_02005408();
void Func_020054f0();

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

void FieldScene_RunScene3c5_02002548(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02005368, 0x985) != 0) {
        Call1(Func_02005382, 0x985);
        Func_020054f0(157);
        Func_0200539c();
        Call3(Func_020053d0, 8, 0x128, 240);
        Call3(Func_020053dc, 9, 0x138, 240);
        Func_02005402(8);
        Func_02005408(9);
        Call6(Func_02005374, 0, 14, 4, 1, 17, 14);
        Func_020053e0();
        if (Value1(Func_020053be, 0x301) != 0) {
            Call1(Func_020053d8, 0x301);
        } else {
            Call1(Func_020053d8, 0x301);
        }
    }
}
