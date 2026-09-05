#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_020013e4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002c18();
void Func_02002c36();
s32 Func_02002c98();
s32 Func_02002c9e();
void Func_02002cc4();
void Func_02002cce();
void Func_02002cf8();
void Func_02002d3c();
void Func_02002d4a();
void Func_02002d5e();
void Func_02002d62();
void Func_02002d6c();
void Func_02002d70();
void Func_02002d82();
void Func_02002d90();
void Func_02002de4();
void Func_02002e06();
void Func_02002e22();
void Func_02002e28();
void Func_02002e34();
void Func_02002e3e();
void Func_02002e40();
void Func_02002e9e();
void Func_02002ecc();
void Func_02002efe();
void Func_02002f08();
void Func_02002f5e();
void Func_02002f84();
void Func_02002f90();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    u32 i;
    u8 *rec2;
    s32 rec7;
    s32 record;
    u8 *p6;

    Func_02002cf8(a0);
    Call4(Func_02002e22, -1, -1, -1, 0);
    Func_02002c18(1);
    Func_02002d82(0, 0, 0);
    Call3(Func_02002d90, 18, 0x1e00000, 0xca0000);
    Func_02002c36(1);
    Func_02002e3e(18, 1);
    rec7 = 0;
    rec2 = Value4(Func_02002c98, 22, 0x1480000, 0x20000, 0xc30000);
    rec2[85] = rec7;
    p6 = *(volatile s32 *)((s32)rec2 + 80);
    *(volatile s32 *)((s32)rec2 + 12) = 0x50000;
    p6[39] = rec7;
    {
        s32 mask = -33;
        p6[5] &= mask;
    }
    p6[9] &= 15;
    rec7 = Value2(Func_02002c9e, 17, 0x608);
    Func_02002d5e(224);
    Func_02002cce(p6[28], 128, (rec7 + 0x400));
    Func_02002cc4(17);
    {
        s32 off = 0x1c0;
        u8 *base = *(u8 *volatile *)Data_03001ebc;
        s32 *slot = (s32 *)(base + off);
        off += 66;
        *slot = off;
    }
    Func_02002efe();
    Call3(Func_02002de4, 18, 0x10000, 0x8000);
    Call3(Func_02002e28, 18, 0x1e0, 176);
    Call3(Func_02002e34, 18, 0x1a4, 164);
    Call3(Func_02002e40, 18, 0x146, 185);
    Call3(Func_02002ecc, 18, 0x4000, 10);
    Func_02002d3c((s32)rec2, 0x2009eac);
    Func_02002d62((s32)rec2);
    Call2(Func_02002d4a, (s32)rec2, 0x2009ecc);
    Func_02002d70((s32)rec2);
    Func_02002e06(20);
    Func_02002d6c((s32)rec2);
    Func_02002e9e(18, 2, 20);
    Func_02002f08(18, 0, 40);
    Func_02002f84();
    Func_02002f90();
    Func_02002f5e(22);
}
