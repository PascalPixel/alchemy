#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000808();
s32 Func_0200177a();
void Func_0200179a();
s32 Func_0200179c();
s32 Func_0200179e();
void Func_020017b4();
void Func_020017c2();
void Func_020017dc();
void Func_02001848();
void Func_02001860();
void Func_02001870();
void Func_02001888();
void Func_020018da();

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

void FieldScene_RunScene376_020005d4(void)
{
    u32 i;
    s32 record;

    record = Func_0200179e(0);
    if ((u32)(*(volatile u16 *)(record + 6) + -0xa001) <= 0x3ffe) {
        Func_020018da(3, 20);
    } else {
        if (Value1(Func_0200177a, 0x87a) != 0) {
            Func_0200179a();
            Call1(Func_02001848, 0x1c0a);
            Func_02001860(20, 0);
            Func_020017b4();
        } else {
            if (Value1(Func_0200179c, 0x815) != 0) {
                Func_02000808();
            } else {
                Func_020017c2();
                Call1(Func_02001870, 0xf55);
                Func_02001888(20, 0);
                Func_020017dc();
            }
        }
    }
}
