#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200608c();
void Func_020060c0();
s32 Func_020060c2();
void Func_020060de();
void Func_020060ec();
void Func_020060ee();
void Func_02006130();
void Func_02006138();
s32 Func_02006146();
void Func_02006150();
void Func_0200618a();
void Func_02006190();
void Func_020061a0();
void Func_020061a6();
void Func_020061a8();
void Func_020061b6();
void Func_020061be();
void Func_020061c4();
void Func_020061c8();
void Func_020061f0();
void Func_02006200();

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

void FieldScene_RunScene372SequenceD(void)
{
    u32 i;
    s32 record;

    Func_0200608c();
    record = Value1(Func_020060c2, 0);
    if (record != 0) {
        Func_02006130(22, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Call3(Func_020060ee, 22, 0x10000, 0x8000);
    Call3(Func_02006138, 22, 0x119, 0x1fb);
    Func_0200618a(22, 0, 0);
    Func_020060c0(30);
    Call1(Func_020061a6, 0xe7b);
    Func_020061be(22, 0);
    Func_020061a0(0, 22, 0);
    Func_020060de(10);
    Func_020061a6(0, 1);
    Func_020060ec(20);
    Call3(Func_02006200, 22, 0x4000, 0);
    Func_020061f0(22, 0);
    Func_020061a8(22, 2);
    record = Value1(Func_02006146, 0);
    if (record != 0) {
        Func_02006190(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020061b6(22);
    Func_020061c8(22, 0, 0);
    Call3(Func_020061c4, 0, 0x100, 0x205);
    Func_02006150();
}
