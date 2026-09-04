#include "types.h"

#define FieldScene_RunScene3bbSequenceA Func_020008ec

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200c834[];
extern u8 Data_0200c838[];
extern u8 Data_03001ebc[];
void Func_020046ac();
void Func_0200489a();
void Func_020048a8();
void Func_020048b6();
void Func_020048c4();
void Func_020048d4();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3bbSequenceA(void)
{
    u32 i;
    s32 record;

    *(volatile s32 *)Data_0200c838 = 0;
    *(volatile s32 *)Data_0200c834 = 0;
    Call1(Func_020046ac, 0x2008715);
    Call3(Func_0200489a, 22, 0x3a80000, 0xd80000);
    Call3(Func_020048a8, 23, 0x3c80000, 0xd80000);
    Call3(Func_020048b6, 24, 0x3e80000, 0xd80000);
    Call3(Func_020048c4, 25, 0x4080000, 0xd80000);
    Func_020048d4(31, 10);
}
