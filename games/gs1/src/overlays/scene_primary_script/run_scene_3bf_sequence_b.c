#include "types.h"

#define FieldScene_RunScene3bfSequenceB Func_0200175c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][2];
s32 Func_02003054();
void Func_02006d28();
void Func_02006d2e();
s32 Func_02006d76();
void Func_02006da2();

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

void FieldScene_RunScene3bfSequenceB(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    if (Value1(Func_02006d76, 0x225) == 0) {
        if (Value1(Func_02003054, 13) != 0) {
            if (Data_02000240_t[147][0] == 0) {
                Call1(Func_02006da2, 0x225);
                Call1(Func_02006d28, 0x200975d);
                Call1(Func_02006d2e, 0x20097bd);
                {
                    volatile u16 *target = (volatile u16 *)(((s32)p5 + 0x182));
                    s32 shown = 96;

                    *target = shown;
                }
            }
        }
    }
}
