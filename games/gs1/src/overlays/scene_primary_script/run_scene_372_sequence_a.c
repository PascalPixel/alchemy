#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000e67[];
extern u8 Data_03001ebc[];
void Func_02005c0c();
void Func_02005c72();
void Func_02005c7e();
void Func_02005c98();
void Func_02005c9c();
void Func_02005ca8();
void Func_02005cb4();
void Func_02005cbc();
void Func_02005cd4();
void Func_02005ce0();
void Func_02005cf0();
void Func_02005cf4();
void Func_02005d0e();
void Func_02005d2e();
void Func_02005d38();
void Func_02005d3a();
void Func_02005d6e();
void Func_02005d86();
void Func_02005d94();
void Func_02005d9e();
s32 Func_02005da4();
void Func_02005db6();
void Func_02005e24();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

void FieldScene_RunScene372SequenceA(void)
{
    u32 i;
    s32 record;
    s32 base5_e67;

    Func_02005c0c();
    Call3(Func_02005c98, 0, 0x106, 0x32a);
    Call3(Func_02005cb4, 20, 0x1060000, 0x3250000);
    Call3(Func_02005ca8, 20, 0x106, 0x339);
    Call3(Func_02005c7e, 0, 0x20000, 0x10000);
    Func_02005cf0(0, 2, 0);
    Call3(Func_02005cd4, 0, 0x11a, 0x357);
    Func_02005cf4(20, 1);
    Func_02005d0e(0, 4, 0);
    Func_02005d38(0, 20, 0);
    Func_02005e24();
    Func_02005c72(30);
    Func_02005d3a(0, 2);
    Call3(Func_02005d9e, 20, 0x100, 20);
    base5_e67 = (s32)Data_00000e67;
    Func_02005d6e(base5_e67);
    Func_02005d86(20, 0);
    Func_02005c9c(20);
    Value2(Func_02005da4, 20, 0);
    Func_02005d6e(20, 2);
    Func_02005d94((base5_e67 + 4));
    Func_02005db6(20, 0, 20);
    Call2(Func_02005d2e, 20, 0x200c8c0);
    Call1(Func_02005cbc, 0x835);
    Func_02005ce0();
}
