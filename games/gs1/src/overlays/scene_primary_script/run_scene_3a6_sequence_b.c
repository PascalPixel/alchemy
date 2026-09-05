#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02002eec();
s32 Func_02002ef6();
void Func_02002f08();
void Func_02002f0e();
void Func_02002f2a();
void Func_02002f3a();
void Func_02002f68();
void Func_02002f78();
void Func_02002f8e();
void Func_02002f94();
void Func_02002f9c();
s32 Func_02002fa2();
void Func_02002faa();
void Func_02002fac();
void Func_02002ff4();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call0(void (*f)())
{
    f();
}

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

void FieldScene_RunScene3a6SequenceB(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02002eec, 0x200) != 0) {
        if (Value1(Func_02002ef6, 0x201) == 0) {
            Call1(Func_02002f08, 0x201);
            Call1(Func_02002f0e, 0x302);
            Func_02002f2a();
            Call2(Func_02002fac, 8, 0x102);
            Func_02002f94(8, 2);
            Func_02002f3a(20);
            Call3(Func_02002f68, 8, 0x20000, 0x10000);
            Call3(Func_02002f8e, 8, 0x2f8, 0x118);
            Call3(Func_02002f9c, 8, 0x2f8, 0x138);
            Call3(Func_02002faa, 8, 0x318, 0x138);
            Func_02002f78(10);
            Call3(Func_02002ff4, 8, 0xc000, 20);
            record = Func_02002fa2(8);
            *(volatile s32 *)(record + 108) = 0x2008cf9;
            Call0((void (*)())Func_02002fa2);
        }
    }
}
