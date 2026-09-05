#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02004690();
void Func_020079a0();
void Func_020079b0();
s32 Func_020079b4();
void Func_020079b6();
void Func_020079e4();
void Func_020079fe();
void Func_02007a0e();
void Func_02007a14();
void Func_02007b5e();

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
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

void RunSecondaryMapInteraction(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    if (*(s16 *)((*(u8 *volatile *)Data_03001ebc + 0xcb8)) != 0) {
        if (Value1(Func_020079b4, 0x948) == 0) {
            Call2(Func_020079b0, 0x1528, 1);
            Func_02007b5e(188);
            v5 = 3;
            v6 = 55;
            Func_020079e4(1);
            Call6(Func_020079a0, 6, 77, 1, 2, v5, v6);
            Func_020079fe(5);
            Call6(Func_020079b6, 7, 77, 1, 2, v5, v6);
            Func_02007a14(1);
            Func_02004690();
            Call1(Func_02007a0e, 0x948);
        }
    }
}
