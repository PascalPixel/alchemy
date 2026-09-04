#include "types.h"

#define FieldScene_RunScene3b4_02000ad0 Func_02000ad0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02002fde();
void Func_02002ff0();
void Func_0200300c();
void Func_0200303e();
void Func_02003074();
s32 Func_02003078();
void Func_0200308c();
void Func_0200309e();
void Func_020030a0();
void Func_020030ac();
void Func_020030b4();
void Func_020030b8();
void Func_020030f0();
void Func_02003132();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
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

void FieldScene_RunScene3b4_02000ad0(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02002fde, 0x9c8) == 0) {
        Call1(Func_02002ff0, 0x9c8);
        Func_0200300c();
        Call2(Func_020030a0, 0x20000, 0x4000);
        Func_020030b8(15, 1);
        Func_020030b4();
        Func_0200309e(15, 0, 20);
        Call2(Func_020030b8, 15, 0x102);
        Func_020030a0(15, 2);
        Func_0200303e(20);
        Call3(Func_02003074, 15, 0x10000, 0x8000);
        Func_02003132(152);
        record = Func_02003078(15);
        *(volatile s32 *)(record + 40) = 0x80000;
        Call3(Func_020030ac, 15, 0x248, 0x2a8);
        Call3(Func_020030f0, 15, 0x4000, 20);
        Func_0200308c();
    }
}
