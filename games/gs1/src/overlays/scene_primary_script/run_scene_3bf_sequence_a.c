#include "types.h"

#define FieldScene_RunScene3bfSequenceA Func_0200169c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][2];
s32 Func_02002d10();
s32 Func_02002fae();
s32 Func_02006cb8();
void Func_02006cec();
s32 Func_02006cf4();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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

void FieldScene_RunScene3bfSequenceA(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    if (Value1(Func_02006cb8, 0x214) == 0) {
        if (Value0(Func_02002d10) == 0) {
            if (Data_02000240_t[147][0] == 0) {
                if (Value1(Func_02002fae, 17) != 0) {
                    Call1(Func_02006cec, 0x215);
                    Call1(Func_02006cf4, 0x214);
                }
            }
            if (Value1(Func_02006cf4, 0x214) != 0) {
                {
                    volatile u16 *target = (volatile u16 *)(((s32)p5 + 0x182));
                    s32 shown = 94;

                    *target = shown;
                }
            }
        }
    }
}
