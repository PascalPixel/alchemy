#include "types.h"

#define FieldScene_RunScene38e_0200045c Func_0200045c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000d58();
s32 Func_02000f1c();
void Func_02000f34();
void Func_02000f72();
void Func_02000f7a();
void Func_02000f8c();
void Func_02000fb4();
void Func_02000fc0();
void Func_02001050();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

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

void FieldScene_RunScene38e_0200045c(void)
{
    u32 i;
    s32 record;

    Func_02000f34();
    if (Value1(Func_02000f1c, 0x200) == 0) {
        Func_02000d58();
    }
    Call3(Func_02000f7a, 0, 0x8000, 0x4000);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Func_02000fc0(0, 2);
    Call3(Func_02000fb4, 0, 2, -16);
    Func_02000f72(16);
    Func_02001050(2);
    Func_02000f8c();
}
