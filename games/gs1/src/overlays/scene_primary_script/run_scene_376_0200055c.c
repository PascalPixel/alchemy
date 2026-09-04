#include "types.h"

#define FieldScene_RunScene376_0200055c Func_0200055c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02001706();
s32 Func_02001718();
s32 Func_02001726();
void Func_02001756();
void Func_020017d0();
void Func_020017e2();
void Func_020017ea();
void Func_02001802();
void Func_02001862();

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

void FieldScene_RunScene376_0200055c(void)
{
    u32 i;
    s32 record;

    record = Func_02001726(0);
    if ((u32)(*(volatile u16 *)(record + 6) + -0xa001) <= 0x3ffe) {
        Func_02001862(2, 22);
    } else {
        ((void (*)())Func_02001718)();
        if (Value1(Func_02001706, 0x87a) != 0) {
            Call1(Func_020017d0, 0x1c09);
        } else {
            if (Value1(Func_02001718, 0x815) != 0) {
                Call1(Func_020017e2, 0x11a3);
            } else {
                Call1(Func_020017ea, 0xf54);
            }
        }
        Func_02001802(22, 0);
        Func_02001756();
    }
}
