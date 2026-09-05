#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_02000248

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001197[];
extern u8 Data_03001ebc[];
void Func_020025fa();
void Func_02002858();
s32 Func_020028c6();
s32 Func_020028d8();
void Func_020028dc();
void Func_020028e8();
s32 Func_020028f2();
void Func_020028f6();
s32 Func_0200290c();
void Func_02002950();
void Func_0200295a();
s32 Func_0200295e();
void Func_02002976();
void Func_02002984();
void Func_020029c2();
void Func_020029e2();
s32 Func_02002a06();
void Func_02002a0a();
void Func_02002a10();
void Func_02002a18();
void Func_02002a1e();
void Func_02002a20();
void Func_02002a2c();
void Func_02002a2e();
void Func_02002a30();
void Func_02002a60();
void Func_02002ac4();
void Func_02002ae0();
void Func_02002af8();
void Func_02002b6a();
void Func_02002ba4();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 base5_1197;
    s32 base7_0;
    u8 *p6;

    Func_020028e8();
    if (Value1(Func_020028c6, 0x815) != 0) {
        base5_1197 = (s32)Data_00001197;
        Func_020029c2(base5_1197);
        if (Value1(Func_020028d8, 2) != 0) {
            bump_step(1);
        }
        if (Value1(Func_020028f2, 3) != 0) {
            bump_step(1);
        }
        Value2(Func_02002a06, 17, 0);
        if (Value2(Func_0200295e, 0, 0) == 0) {
            Func_02002a10((base5_1197 + 3));
        } else {
            Func_02002a18((base5_1197 + 4));
        }
        Func_02002a30(17, 0);
    } else {
        p6 = *(volatile s32 *)(*(volatile s32 *)0x03001e70);
        Call1(Func_02002a2e, 0xf48);
        Func_02002a20(17, 0, 0);
        Func_02002a60(17, 0);
        Func_02002976(20);
        Func_02002a1e(17, 2);
        Func_02002984(15);
        Func_02002858();
        base7_0 = 0;
        for (i = 0; i < 40; i++) {
            Func_020025fa(((s32 (*)())Func_020029c2)(17));
            Func_020028f6(1);
        }
        Value2(Func_0200290c, 0x200a591, 0xc80);
        Func_02002b6a(107);
        for (i = 0; i != 180; i++) {
            if (Value2(Func_0200290c, i, 10) == 0) {
                if ((1 & base7_0) != 0) {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 - 0x10000;
                } else {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 + 0x10000;
                }
                base7_0 = (base7_0 + 1);
            }
            Func_020029e2(1);
        }
        Call1(Func_02002ba4, 0x121);
        Call1(Func_0200295a, 0x200a591);
        Func_02002950(1);
        Func_020028dc();
        Func_02002ac4(17, 0);
        Func_02002a0a(40);
        Call1(Func_02002ae0, 0xf4b);
        Func_02002af8(17, 0);
    }
    Func_02002a2c();
}
