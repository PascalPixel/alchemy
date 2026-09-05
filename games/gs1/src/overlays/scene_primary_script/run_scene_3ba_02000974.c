#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003246();
void Func_020044f4();
void Func_02004618();
void Func_0200462c();
void Func_0200463c();
void Func_02004642();
void Func_02004692();
void Func_020046a0();
void Func_020046ae();
void Func_020046cc();
void Func_020046d4();
void Func_020046de();
void Func_020046e0();
void Func_020046e6();
void Func_02004774();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene3ba_02000974(s32 a0)
{
    u32 i;
    s32 record;

    Func_0200463c(24);
    Func_02004642(25);
    Func_02004618(1);
    Func_0200462c();
    Call3(Func_02004692, 8, 0x5280000, 0xc00000);
    Call3(Func_020046a0, 0, 0x5080000, 0xc00000);
    Call3(Func_020046d4, 8, 0x4000, 0);
    Call3(Func_020046e0, 0, 0x4000, 0);
    if (a0 < 0) {
        Func_020046cc(8, 10);
        Func_020046d4(0, 35);
    } else {
        Func_020046de(8, 8);
        Func_020046e6(0, 28);
    }
    Func_020044f4(1);
    Call4(Func_02004774, 0x5180000, 0, 0x800000, 0);
    Func_02003246(a0);
    Func_020046ae();
}
