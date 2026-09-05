#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00002424[];
extern u8 Data_03001ebc[];
void Func_02007330();
void Func_020073e2();
void Func_020073f0();
void Func_020073fa();
void Func_02007400();
void Func_02007416();
void Func_02007418();
void Func_0200742a();
void Func_0200742e();
void Func_02007446();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3bf_02001cf0(s32 a0)
{
    u32 i;
    s32 record;
    s32 base6_2424;

    base6_2424 = (s32)Data_00002424;
    Func_020073e2(base6_2424);
    Func_020073fa(a0, 0);
    Func_02007330(120);
    Call3(Func_0200742a, a0, 0x101, 60);
    Func_02007400((base6_2424 + 1));
    Func_02007418(a0, 0);
    Func_020073f0(a0, 1);
    Func_02007416((base6_2424 + 2));
    Func_0200742e(a0, 0);
    Func_020073f0(a0, 4);
    Func_0200742e((base6_2424 + 3));
    Func_02007446(a0, 0);
}
