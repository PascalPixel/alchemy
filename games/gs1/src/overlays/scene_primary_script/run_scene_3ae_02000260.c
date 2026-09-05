#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020017ba();
void Func_020017e4();
void Func_020017ee();
s32 Func_0200181c();
void Func_0200184c();
void Func_0200188c();
s32 Func_0200189c();
void Func_020018b8();
void Func_020018d0();
void Func_020018d8();
void Func_020018e8();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3ae_02000260(void)
{
    u32 i;
    s32 record;

    Func_020017e4();
    if (Value1(Func_020017ba, 0x8a6) == 0) {
        Call1(Func_0200188c, 0x1cfd);
        Value2(Func_0200189c, 11, 0);
        if (Value2(Func_0200181c, 0, 0) == 0) {
            Func_020018b8(11, 0);
            Call1(Func_020017ee, 0x8a6);
            goto L_020002c2;
        }
        bump_step(1);
        Func_020018d8(11, 0);
    } else {
        Call1(Func_020018d0, 0x1cfe);
        Func_020018e8(11, 0);
    }
    L_020002c2:;
    Func_0200184c();
}
