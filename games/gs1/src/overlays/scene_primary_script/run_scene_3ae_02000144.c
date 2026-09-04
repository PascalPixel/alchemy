#include "types.h"

#define FieldScene_RunScene3ae_02000144 Func_02000144

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020016a6();
void Func_020016c8();
void Func_020016fe();
void Func_0200170e();
void Func_02001712();
void Func_02001724();
void Func_0200173c();
void Func_020017c0();

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

void FieldScene_RunScene3ae_02000144(void)
{
    u32 i;
    s32 record;

    Func_020016c8();
    Call1(Func_020016a6, 0x8aa);
    Call3(Func_02001724, 0, 0x188, 0x128);
    Call3(Func_0200170e, 8, 0x13333, 0x9999);
    Call3(Func_0200173c, 8, 0x198, 0x128);
    Call3(Func_020017c0, 8, 0x8000, 0);
    Func_020016fe(20);
    Func_02001712();
}
