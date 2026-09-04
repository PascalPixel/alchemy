#include "types.h"

#define FieldScene_RunScene372_02003c48 Func_02003c48

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200828a();
void Func_020082d4();
void Func_020082e8();
void Func_02008300();
void Func_02008306();
void Func_0200830c();
void Func_02008312();
void Func_02008318();
void Func_0200831e();
void Func_0200833a();
void Func_02008356();
void Func_0200835c();
void Func_02008362();
void Func_02008368();
void Func_0200836e();
void Func_02008374();
void Func_020083a0();
void Func_020084f6();
void Func_02008500();
void Func_0200850e();
void Func_02008514();
void Func_02008522();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene372_02003c48(void)
{
    u32 i;
    s32 record;

    Func_0200828a(20);
    Call1(Func_0200833a, 0x166);
    Func_02008300(0);
    Func_02008306(1);
    Func_0200830c(2);
    Func_02008312(3);
    Func_02008318(4);
    Func_0200831e(5);
    Call2(Func_020084f6, 0x10003, 1);
    Call2(Func_02008500, 0x10000, 2);
    Func_0200850e(1);
    Func_020082d4(120);
    Func_02008514(0, 0);
    Func_02008522(60);
    Func_020082e8(60);
    Call1(Func_020083a0, 0x166);
    Func_02008356(0);
    Func_0200835c(1);
    Func_02008362(2);
    Func_02008368(3);
    Func_0200836e(4);
    Func_02008374(5);
}
