#include "types.h"

#define FieldScene_RunScene3af_02001b58 Func_02001b58

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003818();
void Func_02005dc4();
void Func_02005dec();
void Func_02005dfe();
void Func_02005e10();
void Func_02005e20();
void Func_02005e2e();
void Func_02005e48();
void Func_02005e58();
void Func_02005e5e();
s32 Func_02005e90();
void Func_02005edc();
void Func_02005ee2();
s32 Func_02005ee4();
s32 Func_02005efc();
void Func_02005f22();
void Func_02005f2a();
void Func_02005f34();
void Func_02005f3e();
void Func_02005f56();
void Func_02005f9c();

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

void FieldScene_RunScene3af_02001b58(void)
{
    u32 i;
    s32 record;

    Func_02005e48();
    Call1(Func_02005e5e, 0x200d160);
    Func_02005dc4(1);
    Call3(Func_02005ee2, 0, 0xe80000, 0x27c0000);
    Func_02005f22(0, 15);
    record = Func_02005e90(0);
    Func_02005e2e(record, 0);
    Func_02005dec(1);
    Func_02005f9c(0, 0);
    Func_02005e20();
    Func_02005dfe(1);
    Func_02005edc(22);
    Func_02005ee2(21);
    Func_02005e10(1);
    Func_02005f2a(22, 0, 0);
    Func_02005f34(21, 0, 0);
    Func_02005f3e(20, 0, 0);
    record = Func_02005ee4(20);
    {
        s32 shown = 0x3000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Call3(Func_02005f56, 23, 0xe80000, 0x28a0000);
    record = Func_02005efc(23);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Func_02005e58(1);
    Func_02003818(20, 23);
}
