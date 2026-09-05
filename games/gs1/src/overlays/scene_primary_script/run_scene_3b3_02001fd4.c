#include "types.h"

#define FieldScene_RunScene3b3_02001fd4 Func_02001fd4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020023ac();
s32 Func_02003244();
void Func_02003d8c();
void Func_02004ae4();
s32 Func_02004b02();
s32 Func_02004b14();
s32 Func_02004b2a();
u8 *Func_02004b3a();
void Func_02004b3e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b3_02001fd4(void)
{
    u32 i;
    u8 *record;

    Func_02004ae4();
    if (Value0(Func_02003244) == 0) {
        *(u8 *)(Func_02004b02(0) + 85) &= 254;
        *(u8 *)(Func_02004b14(0) + 35) &= 254;
        Func_020023ac();
        Func_02003d8c();
        *(u8 *)(Func_02004b2a(0) + 85) |= 1;
        {
            u8 *record = Func_02004b3a(0);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 1);
        }
    }
    Func_02004b3e();
}
