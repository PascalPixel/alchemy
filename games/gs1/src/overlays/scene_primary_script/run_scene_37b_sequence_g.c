#include "types.h"

#define FieldScene_RunScene37bSequenceF Func_020016dc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02003b1a();
void Func_02003b2a();
u8 *Func_02003b50();
s32 Func_02003b54();
void Func_02003b68();
s32 Func_02003b82();
s32 Func_02003b8e();
s32 Func_02003b96();
void Func_02003ba2();
s32 Func_02003bae();
void Func_02003bd0();
s32 Func_02003bfa();
s32 Func_02003c1a();
void Func_02003c28();
void Func_02003c44();
void Func_02003c5a();
void Func_02003c5e();
void Func_02003c70();
void Func_02003c7a();
void Func_02003c80();
void Func_02003c86();
void Func_02003c90();
void Func_02003c9c();
void Func_02003ca0();
void Func_02003ca6();
void Func_02003cac();
void Func_02003cb2();
void Func_02003cb8();
void Func_02003cbc();
void Func_02003ccc();
void Func_02003cd2();
void Func_02003cee();
s32 Func_02003cf6();
s32 Func_02003d00();
void Func_02003d02();
s32 Func_02003d0a();
s32 Func_02003d14();
void Func_02003d18();
void Func_02003d20();
void Func_02003d24();
void Func_02003d26();
void Func_02003d2a();
void Func_02003d3c();
void Func_02003d7c();
void Func_02003d7e();
void Func_02003df0();
void Func_02003e20();

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene37bSequenceF(void)
{
    u32 i;
    u8 *rec8;
    s32 record;
    u8 *p5;

    rec8 = Func_02003b50(17);
    Call4(Func_02003b1a, 2, 0x1100000, 0x800000, 0);
    Call4(Func_02003b2a, 2, 0x1200000, 0x800000, 0);
    if (rec8 == 0) {
    } else {
        p5 = *(s32 *)(rec8 + 16);
        Func_02003b68();
        if (((s32)p5 >> 20) != 8) {
        } else {
            if (Value1(Func_02003b54, 0x207) == 0) {
                record = Value1(Func_02003b96, 0);
                if ((u32)(*(s32 *)(record + 16) >> 19) <= 17) {
                    Call3(Func_02003bd0, 0, 0x121, 158);
                    record = Func_02003bae(0);
                    {
                        s32 shown = 0xc000;
                    
                        *(u16 *)(record + 6) = shown;
                    }
                }
            }
            if (Value1(Func_02003b82, 0x816) == 0) {
            } else {
                if (Value1(Func_02003b8e, 0x817) == 0) {
                } else {
                    Call1(Func_02003ba2, 0x818);
                    Call2(Func_02003c86, 0x20000, 0x4000);
                    Call4(Func_02003ca0, 0x11e0000, -1, 0x920000, 1);
                    Func_02003cac();
                    *(u8 *)(Func_02003bfa(17) + 90) &= 254;
                    Value3(Func_02003c1a, 17, 0x30000, 0x10000);
                    {
                        s32 z = 0;

                        rec8[85] = (u8)z;
                        Func_02003cb2(17, 3);
                        Func_02003d20(189);
                        Call3(Func_02003c44, 17, 0x120, 178);
                        Call1((void (*)())Func_02003c1a, 8);
                        Call3(Func_02003c80, 18, 0x1200000, 0xb20000);
                        *(s32 *)(rec8 + 56) = -0x80000000;
                        *(s32 *)(rec8 + 60) = -0x80000000;
                        *(s32 *)(rec8 + 64) = -0x80000000;
                        *(s32 *)(rec8 + 8) = z;
                        *(s32 *)(rec8 + 12) = z;
                        *(s32 *)(rec8 + 16) = z;
                        *(s32 *)(rec8 + 36) = z;
                        *(s32 *)(rec8 + 40) = z;
                        *(s32 *)(rec8 + 44) = z;
                        Func_02003ca0(17, 0, 0);
                    }
                    Call3(Func_02003c28, 0x10000, 0x10000, 0x10000);
                    Func_02003c5e(10);
                    Func_02003d7c(141);
                    Call3(Func_02003c44, 0x30000, 0x30000, 0x10000);
                    Func_02003c7a(10);
                    Call3(Func_02003c5a, 0x50000, 0x50000, 0x10000);
                    Func_02003c90(35);
                    Call3(Func_02003c70, 0x40000, 0x40000, 0x10000);
                    Func_02003ca6(20);
                    Call3(Func_02003c86, 0x30000, 0x30000, 0x10000);
                    Func_02003cbc(30);
                    Call3(Func_02003c9c, 0x20000, 0x20000, 0x10000);
                    Func_02003cd2(40);
                    Call1(Func_02003df0, 0x121);
                    Call3(Func_02003cb8, 0x10000, 0x10000, 0x10000);
                    Func_02003cee(10);
                    Call3(Func_02003ccc, -1, -1, 0xe666);
                    Func_02003d02(60);
                    Func_02003e20(188);
                    if (Value1(Func_02003cf6, 0x80b) != 0) {
                        if (Value1(Func_02003d00, 0x80c) != 0) {
                            if (Value1(Func_02003d0a, 0x80d) != 0) {
                                if (Value1(Func_02003d14, 0x80e) != 0) {
                                    Call1(Func_02003d26, 0x80f);
                                }
                            }
                        }
                    }
                    Func_02003d3c(40);
                    Call2(Func_02003d24, 0x1038, 1);
                    Call6(Func_02003d18, 0, 1, 2, 1, 17, 8);
                    Call6(Func_02003d2a, 17, 9, 2, 1, 17, 7);
                }
            }
        }
        Func_02003d7e();
    }
}
