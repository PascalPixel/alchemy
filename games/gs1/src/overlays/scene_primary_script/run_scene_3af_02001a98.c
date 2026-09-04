#include "types.h"

#define FieldScene_RunScene3af_02001a98 Func_02001a98

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003760();
void Func_02005d10();
void Func_02005d88();
void Func_02005da0();
void Func_02005dc2();
s32 Func_02005e16();
void Func_02005e2a();
s32 Func_02005e2e();
void Func_02005e34();
void Func_02005e3e();
void Func_02005e48();
void Func_02005e52();
void Func_02005e5c();
void Func_02005e66();
void Func_02005e70();
void Func_02005e88();
void Func_02005ec2();
void Func_02005f4e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene3af_02001a98(void)
{
    u32 i;
    s32 record;

    Func_02005d88();
    Call4(Func_02005ec2, -1, -1, -1, 0);
    Func_02005d10(1);
    Func_02005e2a(20, 0, 0);
    Func_02005e34(22, 0, 0);
    Func_02005e3e(24, 0, 0);
    Func_02005e48(25, 0, 0);
    Func_02005e52(26, 0, 0);
    Func_02005e5c(27, 0, 0);
    Func_02005e66(0, 0, 0);
    Func_02005e70(23, 0, 0);
    record = Func_02005e16(23);
    {
        s32 shown = 0x3000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Call3(Func_02005e88, 21, 0xe80000, 0x28a0000);
    record = Func_02005e2e(21);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Call4(Func_02005f4e, 0xe80000, -1, 0x27c0000, 0);
    Func_02005dc2();
    Func_02005da0(1);
    Func_02003760(23, 21);
}
