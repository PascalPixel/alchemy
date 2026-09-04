#include "types.h"

#define FieldScene_RunScene3b4_02002334 Func_02002334

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003d38();
void Func_02003e3c();
void Func_02003fa0();
void Func_02004074();
void Func_0200419c();
void Func_020041d4();
void Func_020041f8();
void Func_020047ba();
s32 Func_020047c4();
s32 Func_0200486c();
s32 Func_0200487a();
s32 Func_020048b4();
void Func_020048c4();
s32 Func_020048ca();
s32 Func_020048d2();
s32 Func_020048d4();
s32 Func_020048dc();
void Func_020048f0();
s32 Func_020048fa();
s32 Func_02004924();
s32 Func_02004926();
void Func_0200492c();
void Func_0200493c();
void Func_02004946();
s32 Func_0200494a();
void Func_02004958();
void Func_0200497c();
void Func_020049a6();
void Func_020049b6();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

void FieldScene_RunScene3b4_02002334(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    *(u8 *)(Func_0200487a(14) + 85) = 0;
    Call2(Func_020047ba, 0x2009e95, 0xc80);
    Value2(Func_020047c4, 0x2009edd, 0xc80);
    Func_02004946(107, 0, 0);
    if (Value1(Func_0200486c, 0xed9) != 0) {
        Func_020048f0(14, 2);
    }
    Func_02003e3c();
    Func_02003fa0();
    Func_02004074();
    Func_020041d4();
    Func_020041f8();
    Func_0200492c(8, 3);
    *(u8 *)(Func_020048d2(11) + 85) = 0;
    *(u8 *)(Func_020048dc(12) + 85) = 0;
    Func_0200419c();
    if (Value1(Func_020048b4, 0x200) != 0) {
        Func_02003d38();
        Func_0200493c(13, 5);
    }
    if (Value1(Func_020048ca, 0x109) == 0) {
        rec7 = Value1(Func_020048d4, 0x9ca);
        if (rec7 != 0) {
            Call3(Func_02004958, 15, 0x3580000, 0x3380000);
            record = Func_02004926(15);
            *(volatile s32 *)(record + 108) = 0x2008aa9;
        } else {
            if (Value1(Func_020048fa, 0x9c9) != 0) {
                Call3(Func_0200497c, 15, 0x3780000, 0x2980000);
                record = Value1(Func_0200494a, 15);
                *(volatile u16 *)(*(volatile s32 *)(record + 80) + 30) = rec7;
                Func_020048c4(record, 16);
            } else {
                if (Value1(Func_02004924, 0x9c8) != 0) {
                    Call3(Func_020049a6, 15, 0x2480000, 0x2a80000);
                } else {
                    Call3(Func_020049b6, 15, 0x2480000, 0x2980000);
                }
            }
        }
    }
}
