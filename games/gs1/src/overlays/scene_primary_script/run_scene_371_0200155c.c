#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020056f6();
void Func_0200570a();
void Func_0200571e();
void Func_02005732();
void Func_02005746();
void Func_0200575c();
void Func_02005772();
void Func_02005786();
void Func_0200579a();
void Func_020057ae();
void Func_020057c2();
void Func_020057d8();
void Func_02005960();
void Func_02005974();
void Func_02005988();
void Func_0200599c();
void Func_020059b0();
void Func_020059c6();
void Func_020059dc();
void Func_020059f0();
void Func_02005a04();
void Func_02005a18();
void Func_02005a2c();
void Func_02005a42();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene371_0200155c(void)
{
    u32 i;
    s32 record;

    Call4(Func_02005960, 0x160c0000, -1, 0x6f80000, 1);
    Func_020056f6(4);
    Call4(Func_02005974, 0x16040000, -1, 0x6fc0000, 1);
    Func_0200570a(4);
    Call4(Func_02005988, 0x160c0000, -1, 0x6f40000, 1);
    Func_0200571e(4);
    Call4(Func_0200599c, 0x160c0000, -1, 0x6fc0000, 1);
    Func_02005732(4);
    Call4(Func_020059b0, 0x16040000, -1, 0x6f40000, 1);
    Func_02005746(4);
    Call4(Func_020059c6, 0x16080000, -1, 0x6f80000, 1);
    Func_0200575c(4);
    Call4(Func_020059dc, 0x160a0000, -1, 0x6f80000, 1);
    Func_02005772(4);
    Call4(Func_020059f0, 0x16060000, -1, 0x6fa0000, 1);
    Func_02005786(4);
    Call4(Func_02005a04, 0x160a0000, -1, 0x6f60000, 1);
    Func_0200579a(4);
    Call4(Func_02005a18, 0x160a0000, -1, 0x6fa0000, 1);
    Func_020057ae(4);
    Call4(Func_02005a2c, 0x16060000, -1, 0x6f60000, 1);
    Func_020057c2(4);
    Call4(Func_02005a42, 0x16080000, -1, 0x6f80000, 1);
    Func_020057d8(4);
}
