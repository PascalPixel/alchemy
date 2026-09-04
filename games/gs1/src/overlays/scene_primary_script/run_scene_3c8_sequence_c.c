#include "types.h"

#define FieldScene_RunScene3c8SequenceC Func_02001150

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02005ff0();
s32 Func_02006002();
void Func_02006020();
void Func_0200602e();
void Func_02006036();
s32 Func_0200603c();
void Func_0200604e();
s32 Func_02006050();
void Func_02006064();
u8 *Func_02006074();
void Func_02006098();
void Func_020060a4();
void Func_020060a6();
void Func_020060fc();
void Func_0200610c();
void Func_0200612c();
void Func_02006154();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene3c8SequenceC(void)
{
    u32 i;
    s32 rec7;
    u8 *record;
    s32 v2;

    rec7 = Value1(Func_02006002, 0);
    Func_02005ff0();
    Call2(Func_02006020, 0, 0x200d21c);
    Func_0200602e(0);
    Func_020060a6(0, 6);
    *(volatile s32 *)(rec7 + 40) = 0x40000;
    Call3(Func_02006036, 0, 0x40000, 0x20000);
    if ((*(volatile s32 *)(rec7 + 16) >> 20) <= 54) {
        *(u8 *)(Func_0200603c(0) + 90) &= 254;
        v2 = 210;
    } else {
        *(u8 *)(Func_02006050(0) + 90) &= 254;
        v2 = 238;
    }
    Func_02006098(0, *(s16 *)(rec7 + 10), (v2 << 2));
    Func_0200604e(1);
    {
        u8 *record = Func_02006074(0);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    Func_02006064(20);
    *(volatile s32 *)(rec7 + 108) = 0x20085e5;
    Call3(Func_02006154, 0, 0x102, 60);
    Func_020060fc(0, 4);
    Func_0200612c(0, 0);
    Func_0200610c(0, 4);
    *(volatile s32 *)(rec7 + 108) = 0;
    Func_020060a4();
}
