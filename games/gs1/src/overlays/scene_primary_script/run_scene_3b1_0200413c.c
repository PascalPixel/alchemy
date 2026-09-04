#include "types.h"

#define FieldScene_RunScene3b1_0200413c Func_0200413c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020089e2();
void Func_02008a4a();
void Func_02008a74();
void Func_0200a578();
void Func_0200a608();
void Func_0200a6ea();
void Func_0200a718();
void Func_0200a752();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b1_0200413c(void)
{
    u32 i;
    s32 record;

    Func_0200a608();
    Call4(Func_0200a752, -1, -1, -1, 0);
    Func_0200a578(1);
    Func_02008a4a(15, 1, 1);
    Func_0200a6ea(8, 1);
    Call1(Func_0200a718, 0x1e43);
    Func_020089e2(8);
    Call3(Func_0200a752, 8, 0xd000, 40);
    Func_02008a74(9, 15, 0);
}
