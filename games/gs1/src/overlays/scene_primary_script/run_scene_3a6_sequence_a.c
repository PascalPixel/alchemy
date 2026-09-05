#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02002e34();
void Func_02002e48();
void Func_02002e64();
void Func_02002e7a();
void Func_02002e9e();
void Func_02002ecc();
s32 Func_02002edc();
void Func_02002ede();
void Func_02002ef0();
void Func_02002ef6();
void Func_02002ef8();
void Func_02002f04();
void Func_02002f0c();
void Func_02002f10();
void Func_02002f5a();
void Func_02002f76();

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

void FieldScene_RunScene3a6SequenceA(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02002e34, 0x200) == 0) {
        Call1(Func_02002e48, 0x200);
        Func_02002e64();
        Call2(Func_02002ef8, 0x10000, 0x2000);
        Func_02002ef8(8, 1);
        Func_02002f0c();
        Func_02002e7a(60);
        Call3(Func_02002ef6, 8, 0xc000, 20);
        Call2(Func_02002f10, 8, 0x102);
        Func_02002ef8(8, 2);
        Func_02002e9e(20);
        Call3(Func_02002ecc, 8, 0x10000, 0x8000);
        Call3(Func_02002ef0, 8, 0x318, 248);
        Func_02002f76(152);
        record = Func_02002edc(8);
        *(volatile s32 *)(record + 40) = 0x80000;
        Call3(Func_02002f10, 8, 0x318, 0x118);
        Func_02002ede(20);
        Call3(Func_02002f5a, 8, 0xc000, 20);
        Func_02002ef0(30);
        Func_02002f04();
    }
}
