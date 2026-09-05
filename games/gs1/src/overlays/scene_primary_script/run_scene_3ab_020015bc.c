#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002fa0();
void Func_02002fb2();
void Func_02002fee();
void Func_0200300c();
void Func_02003022();
void Func_02003032();
void Func_02003040();
void Func_02003042();
void Func_02003048();
void Func_02003050();
void Func_02003052();
void Func_02003060();
void Func_02003064();
void Func_02003070();
void Func_02003076();
void Func_02003082();
void Func_0200308c();
void Func_02003094();
void Func_020030da();
void Func_020030e4();
void Func_020030ea();
void Func_020030fe();
void Func_02003100();

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

void FieldScene_RunScene3ab_020015bc(void)
{
    u32 i;
    s32 record;

    Func_02002fa0();
    Func_0200308c();
    Func_02002fee(0, 152, 168);
    Func_0200300c(0);
    Func_02002fb2(20);
    Func_020030da(146, 1);
    Func_020030ea(0, 0);
    Func_020030fe();
    Func_020030e4(1);
    Func_02003100();
    Func_02003022(0, 144, 184);
    Func_02003040(0);
    Func_02003032(0, 88, 184);
    Func_02003050(0);
    Func_02003042(0, 88, 200);
    Func_02003060(0);
    Func_02003052(0, 72, 200);
    Func_02003070(0);
    Call3(Func_02003064, 0, 72, 0x120);
    Func_02003082(0);
    Call3(Func_02003076, 0, 88, 0x120);
    Func_02003094(0);
    Func_02003048();
}
