#include "types.h"

#define FieldScene_RunScene371_020017a4 Func_020017a4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020059e0();
s32 Func_02005a5e();
void Func_02005a80();
s32 Func_02005aa6();
s32 Func_02005aae();
void Func_02005c80();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene371_020017a4(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005aa6, 15);
    record = Value1(Func_02005aae, 14);
    *(s32 *)(rec7 + 8) = *(s32 *)(record + 8);
    *(s32 *)(rec7 + 16) = *(s32 *)(record + 16);
    if (*(s32 *)(rec7 + 12) < 0xa0000) {
        *(s32 *)(rec7 + 12) = 0xa0000;
        if (Value1(Func_02005a5e, 0x200) == 0) {
            Func_02005c80(145);
            Func_020059e0(rec7, 3);
            Call1(Func_02005a80, 0x200);
            {
                u16 *target = (u16 *)(rec7 + 100);
                s32 shown = 1;

                *target = shown;
            }
        }
    }
}
