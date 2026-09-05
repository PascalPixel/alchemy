#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000068[];
extern u8 Data_0000009f[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_020020bc();
s32 Func_020020cc();

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

s32 Func_020006e4(void)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[224][0] == (s32)Data_0000009f) {
        Call1(Func_020020bc, 0x941);
        return 0x200a3b4;
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_00000068) {
            record = Value1(Func_020020cc, 0x941);
            if (record == 0) {
                goto L_02000716;
            }
            return 0x200a1bc;
        } else {
            L_02000716:;
        }
    }
    return 0x2009fc4;
}
