#include "types.h"

#define FieldScene_RunScene3bf_020021c4 Func_020021c4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000240d[];
extern u8 Data_03001ebc[];
void Func_020077f8();
void Func_0200783a();
void Func_02007862();
void Func_02007872();
void Func_0200788a();
void Func_0200788c();
void Func_020078da();
void Func_020078e4();
void Func_02007904();
void Func_0200790c();
void Func_02007918();
void Func_0200791c();
void Func_02007920();
void Func_02007938();
void Func_0200794a();
void Func_02007990();
void Func_020079a8();
void Func_020079bc();

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

void FieldScene_RunScene3bf_020021c4(void)
{
    u32 i;
    s32 record;
    s32 base5_240d;

    Func_020077f8();
    Func_02007862(0, 0, 0);
    Func_0200783a(0, 1);
    Func_0200788a(0, 1);
    Func_02007990(113);
    Call3(Func_0200790c, 21, 0x100, 0);
    Call3(Func_02007918, 13, 0x100, 60);
    Func_020078da(21, 0, 0);
    Func_020078e4(13, 0, 0);
    base5_240d = (s32)Data_0000240d;
    Func_02007904(base5_240d);
    Func_0200791c(13, 0);
    Call3(Func_0200794a, 0, 0x102, 30);
    Func_02007920((base5_240d + 1));
    Func_02007938(13, 0);
    Func_020079bc();
    Func_02007872(60);
    Func_020079a8(60);
    Func_0200788c();
    Call1(Func_02007872, 0x225);
}
