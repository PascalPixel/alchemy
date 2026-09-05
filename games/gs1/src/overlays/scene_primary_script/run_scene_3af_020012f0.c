#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001f0a();
void Func_020055c2();
void Func_020055fa();
void Func_02005618();
void Func_02005668();
void Func_02005684();
void Func_020056ae();
void Func_020056b2();
void Func_020056e4();
void Func_020056fc();
void Func_02005706();
void Func_0200571a();
void Func_02005720();
void Func_02005722();
void Func_0200572c();
void Func_02005740();
void Func_02005762();
void Func_02005768();
s32 Func_02005796();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3af_020012f0(void)
{
    u32 i;
    s32 record;

    Call2(Func_02005706, 0x19999, 0x3333);
    Call4(Func_02005720, 0xd80000, -1, 0x3380000, 1);
    Func_0200572c();
    Func_020055fa(20);
    Func_02001f0a();
    Call6(Func_020055c2, 30, 108, 13, 108, 1, 2);
    Func_02005618(10);
    Call3(Func_020056ae, 20, 0xd80000, 0x3200000);
    Call3(Func_02005668, 20, 0x13333, 0x9999);
    Call3(Func_020056b2, 20, 216, 0x32e);
    Func_020056fc(0, 20, 10);
    Func_020056e4(20, 4);
    Func_020056fc(20, 2);
    Call3(Func_02005768, 20, 0x100, 20);
    Func_02005722(20, 0, 20);
    Func_0200571a(20, 2);
    Call1(Func_02005740, 0x1d8d);
    Func_02005762(20, 0, 20);
    Value3(Func_02005796, 20, 0x102, 0);
    Call1(Func_02005684, 0x923);
}
