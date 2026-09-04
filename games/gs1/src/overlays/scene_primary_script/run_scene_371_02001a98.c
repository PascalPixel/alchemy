#include "types.h"

#define FieldScene_RunScene371_02001a98 Func_02001a98

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02004f1c();
void Func_02005c40();
void Func_02005c64();
void Func_02005ca0();
void Func_02005d80();
s32 Func_02005d9a();
void Func_02005dc6();
void Func_02005e02();
void Func_02005e20();
void Func_02005e22();
void Func_02005e2a();
void Func_02005e2e();
void Func_02005eaa();
void Func_02005ecc();
void Func_02005f28();
void Func_02005f44();
void Func_02005f6c();
void Func_02005f78();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

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

void FieldScene_RunScene371_02001a98(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005d9a, 8);
    Func_02005d80();
    Call4(Func_02005eaa, -1, -1, -1, 0);
    Func_02005c40(1);
    Func_02005e22(0, 0, 0);
    Call3(Func_02005e2e, 8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Func_02005c64(1);
    Func_02005ecc(8, 1);
    Func_02005f28();
    Call3(Func_02005e02, 8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;
    
        *(u16 *)(rec7 + 100) = shown;
    }
    if (Value0(Func_02004f1c) == 11) {
        Func_02005e20(8, 0x200d1b8);
    } else {
        Func_02005e2a(8, 0x200d158);
    }
    do {
        Func_02005ca0(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Func_02005f6c();
    Func_02005f78();
    Call1(Func_02005dc6, 0x927);
    Func_02005f44(106);
    Func_02005e20();
}
