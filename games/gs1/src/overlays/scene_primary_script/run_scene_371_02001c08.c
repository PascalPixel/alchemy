#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02005ecc();
void Func_02005eea();
void Func_02005ef0();
void Func_02005f0e();
void Func_02005f10();
void Func_02005f12();
void Func_02005f5a();
void Func_02005faa();
void Func_02005fd0();
void Func_02005fd6();
void Func_02005fe8();
void Func_0200600c();
void Func_02006024();
void Func_02006070();
void Func_0200607a();
void Func_020060a2();
void Func_020060ae();
void Func_020060e4();
void Func_020060f4();
void Func_0200611c();

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

void FieldScene_RunScene371_02001c08(void)
{
    u32 i;
    s32 record;

    Func_02006070();
    Call2(Func_020060a2, 0x10000, 6);
    Func_020060ae();
    Func_0200607a();
    Func_02005faa(8, 2);
    Call1(Func_02005fd0, 0xc66);
    Func_02005fe8(8, 0);
    Func_02005f0e(30);
    Func_020060e4(111);
    Func_02005ecc(0, 2);
    Call1(Func_02005eea, 0x16f);
    Call1(Func_02005ef0, 0x171);
    Func_020060f4();
    Func_02005fd6(8, 4, 30);
    Call1(Func_0200600c, 0xc67);
    Func_02006024(8, 0);
    Call1(Func_02005f12, 0x16f);
    Call1(Func_02005f10, 0x171);
    Func_0200611c();
    Func_02005f5a(30);
    Func_020060a2(12, 6);
}
