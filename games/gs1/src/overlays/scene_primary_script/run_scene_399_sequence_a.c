#include "types.h"

#define FieldScene_RunScene399SequenceA Func_02001794

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002ebe();
void Func_02002ef8();
void Func_02002f30();
void Func_02002f68();
void Func_02002f9e();
s32 Func_02003906();
s32 Func_0200393c();
s32 Func_02003968();
void Func_0200396a();
s32 Func_02003976();
s32 Func_020039a2();
void Func_020039a4();
s32 Func_020039ae();
s32 Func_020039da();
void Func_020039dc();
s32 Func_020039e6();
s32 Func_02003a12();
void Func_02003a14();
s32 Func_02003a48();
void Func_02003a4a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

void FieldScene_RunScene399SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base6_3001e40;

    base6_3001e40 = 0x3001e40;
    if (Value2(Func_02003906, *(volatile s32 *)base6_3001e40, 60) == 0) {
        rec7 = Value4(Func_02003968, 222, 0x1cf0000, 0, 0x1240000);
        if (rec7 != 0) {
            Func_02002ebe();
            *(volatile s32 *)(rec7 + 104) = 60;
            *(volatile s32 *)(rec7 + 108) = 0x200974d;
            Func_0200396a(rec7, 5);
        }
    }
    if (Value2(Func_0200393c, (*(volatile s32 *)base6_3001e40 + 30), 60) == 0) {
        rec7 = Value4(Func_020039a2, 222, 0x1400000, 0x200000, 0x1640000);
        if (rec7 != 0) {
            Func_02002ef8();
            *(volatile s32 *)(rec7 + 104) = 60;
            *(volatile s32 *)(rec7 + 108) = 0x200974d;
            Func_020039a4(rec7, 5);
        }
    }
    if (Value2(Func_02003976, (*(volatile s32 *)base6_3001e40 + 10), 60) == 0) {
        rec7 = Value4(Func_020039da, 222, 0x760000, 0, 0x460000);
        if (rec7 != 0) {
            Func_02002f30();
            *(volatile s32 *)(rec7 + 104) = 60;
            *(volatile s32 *)(rec7 + 108) = 0x200974d;
            Func_020039dc(rec7, 5);
        }
    }
    if (Value2(Func_020039ae, (*(volatile s32 *)base6_3001e40 + 50), 60) == 0) {
        rec7 = Value4(Func_02003a12, 222, 0x1560000, 0, 0x7c0000);
        if (rec7 != 0) {
            Func_02002f68();
            *(volatile s32 *)(rec7 + 104) = 60;
            *(volatile s32 *)(rec7 + 108) = 0x200974d;
            Func_02003a14(rec7, 5);
        }
    }
    if (Value2(Func_020039e6, (*(volatile s32 *)base6_3001e40 + 80), 60) == 0) {
        rec7 = Value4(Func_02003a48, 222, 0x1af0000, 0, 0xab0000);
        if (rec7 != 0) {
            Func_02002f9e();
            *(volatile s32 *)(rec7 + 104) = 60;
            *(volatile s32 *)(rec7 + 108) = 0x200974d;
            Func_02003a4a(rec7, 5);
        }
    }
}
