#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200a9f2();
void Func_0200aa06();
void Func_0200aa12();
void Func_0200aa3a();
void Func_0200ab22();
void Func_0200ab48();
s32 Func_0200ab92();
s32 Func_0200b0cc();
s32 Func_0200b0d6();
s32 Func_0200b0e0();
s32 Func_0200b0ea();
void Func_0200c610();
void Func_0200c652();
void Func_0200c68c();
void Func_0200c6fe();
void Func_0200c70c();
void Func_0200c712();
void Func_0200c71a();
void Func_0200c71c();
void Func_0200c726();
void Func_0200c728();
void Func_0200c72e();
void Func_0200c736();
void Func_0200c746();
void Func_0200c748();
void Func_0200c764();
void Func_0200c770();
void Func_0200c77c();
void Func_0200c790();
void Func_0200c7a6();
void Func_0200c7b2();
void Func_0200c7b8();
void Func_0200c7bc();
void Func_0200c7c0();
void Func_0200c7c8();
void Func_0200c7cc();
void Func_0200c80e();
void Func_0200c816();
void Func_0200c81e();
void Func_0200c826();
void Func_0200c828();
void Func_0200c830();
void Func_0200c834();
void Func_0200c836();
void Func_0200c83c();
void Func_0200c844();
void Func_0200c854();
void Func_0200c85e();
void Func_0200c860();
void Func_0200c868();
void Func_0200c86c();
void Func_0200c874();
void Func_0200c878();
void Func_0200c87a();
void Func_0200c882();
void Func_0200c896();
void Func_0200c8da();
void Func_0200c992();
void Func_0200c996();
void Func_0200c99e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b1_02006110(void)
{
    u32 i;
    s32 rec2;
    s32 rec4;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec2 = Value2(Func_0200b0cc, 0, 0);
    rec8 = Func_0200b0d6(1, 0);
    rec7 = Func_0200b0e0(2, 0);
    rec4 = Value2(Func_0200b0ea, 3, 0);
    Func_0200c610();
    Func_0200aa3a(10, 0, 0);
    Func_0200a9f2(8, 0x1d8, 144, 0x5000);
    Func_0200aa06(27, 0x198, 142, 0x3000);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x201;
    Func_0200c7b8();
    Func_0200c7cc();
    Func_0200c652(40);
    Func_0200c71a(27, 1);
    Call1(Func_0200c748, 0x1f78);
    Func_0200aa12(27);
    Func_0200c726(rec2, 2);
    Func_0200c72e(rec8, 2);
    Func_0200c736(rec7, 2);
    Func_0200c746(rec4, 2);
    Func_0200c68c(20);
    Func_0200c7a6(rec2, 0, 0);
    Call3(Func_0200c7b2, rec8, 0x8000, 0);
    Func_0200c7bc(rec7, 0, 0);
    Call3(Func_0200c7c8, rec4, 0x8000, 40);
    Call3(Func_0200c6fe, rec2, 0x10000, 0x8000);
    Call3(Func_0200c70c, rec8, 0x10000, 0x8000);
    Call3(Func_0200c71a, rec7, 0x10000, 0x8000);
    Call3(Func_0200c728, rec4, 0x10000, 0x8000);
    Call3(Func_0200c764, rec2, 0x1d6, 172);
    Call3(Func_0200c770, rec8, 0x19a, 172);
    Call3(Func_0200c77c, rec7, 0x1d6, 204);
    Call3(Func_0200c790, rec4, 0x19a, 204);
    Func_0200c7b8(rec2, 1);
    Func_0200c7c0(rec8, 1);
    Func_0200c7c8(rec7, 1);
    Call3(Func_0200c854, rec8, 0xd000, 0);
    Call3(Func_0200c860, rec2, 0xb000, 0);
    Call3(Func_0200c86c, rec4, 0xd000, 0);
    Call3(Func_0200c878, rec7, 0xb000, 20);
    Func_0200c830(27, 1);
    Func_0200ab22(27);
    Func_0200c80e(rec2, 3);
    Func_0200c816(rec8, 3);
    Func_0200c81e(rec7, 3);
    Func_0200c836(rec4, 3);
    Func_0200ab48(27);
    Func_0200c834(rec2, 3);
    Func_0200c83c(rec8, 3);
    Func_0200c844(rec7, 3);
    Func_0200c85e(rec4, 3);
    Func_0200c8da(27, 0, 0);
    Func_0200ab92(0, 0x8000);
    Func_0200c87a(0, 3);
    Func_0200c882(27, 3);
    Call3(Func_0200c826, 27, 0x10000, 0x8000);
    Func_0200c868(27, 0x198, 132);
    Call3(Func_0200c874, 27, 0x1bc, 132);
    Func_0200c896(27, 0, 0);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x202;
    Func_0200c992();
    Func_0200c99e();
    Call2(Func_0200c712, 0x92c, 0x935);
    Call2(Func_0200c71c, 0x917, 0x990);
    Call1(Func_0200c828, 0x8a0);
    Func_0200c996(10);
}
