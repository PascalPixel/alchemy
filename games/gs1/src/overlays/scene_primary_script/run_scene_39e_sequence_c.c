#include "types.h"

#define FieldScene_RunScene39eSequenceC Func_0200071c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200486a();
void Func_020048c8();
void Func_02004902();
void Func_02004966();
void Func_02004990();
void Func_020049f4();
s32 Func_02004ae4();
void Func_02004afc();
void Func_02004b14();
void Func_02004b4c();
s32 Func_02004b56();
void Func_02004b64();
s32 Func_02004b68();
void Func_02004b6e();
void Func_02004b7a();
void Func_02004b8c();
void Func_02004bc2();
void Func_02004bc6();
s32 Func_02004bd4();
s32 Func_02004bde();
void Func_02004be4();
void Func_02004be6();
s32 Func_02004bf2();
void Func_02004c00();
void Func_02004c0e();
void Func_02004c10();
void Func_02004c18();
void Func_02004c1a();
void Func_02004c22();
void Func_02004c3c();
void Func_02004c48();
void Func_02004c50();
void Func_02004c7e();
void Func_02004c88();
void Func_02004c8e();
s32 Func_02004c92();
s32 Func_02004c9a();
void Func_02004c9c();
void Func_02004ca6();
void Func_02004ca8();
void Func_02004cc4();
void Func_02004cd2();
void Func_02004cd6();
void Func_02004cda();
void Func_02004ce4();
void Func_02004cf2();
void Func_02004d00();
void Func_02004d16();
void Func_02004d1a();
void Func_02004d24();
void Func_02004d2a();
void Func_02004d36();
s32 Func_02004d3a();
void Func_02004d46();
void Func_02004d4c();
void Func_02004d54();
void Func_02004d66();
void Func_02004d78();
void Func_02004d7a();
void Func_02004d82();
void Func_02004d86();
void Func_02004d94();
void Func_02004da6();
s32 Func_02004daa();
void Func_02004db0();
void Func_02004db2();
void Func_02004dc0();
void Func_02004dc4();
void Func_02004dda();
void Func_02004de0();
void Func_02004df6();
void Func_02004e1e();
void Func_02004e20();
void Func_02004e3e();
void Func_02004e42();
void Func_02004e48();
s32 Func_02004e52();
void Func_02004e5c();
void Func_02004e66();
void Func_02004e6a();
void Func_02004e74();
void Func_02004eb8();
void Func_02004ec6();
void Func_02004ed6();
void Func_02004edc();
void Func_02004ee8();
void Func_02004efc();
void Func_02004efe();
void Func_02004f14();
void Func_02004f1e();
void Func_02004f28();
void Func_02004f34();
void Func_02004f38();
void Func_02004f3a();
void Func_02004f50();

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
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene39eSequenceC(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Func_02004afc();
    rec7 = Value1(Func_02004ae4, 0x300);
    if (rec7 != 0) {
        Call3(Func_02004b6e, 0, 168, 0x1f8);
        Func_02004b14(5);
        Call3(Func_02004c10, 0, 0xc000, 20);
        *(u8 *)(Func_02004b56(8) + 91) = 0;
        Func_02004cda(152);
        record = Func_02004b68(8);
        *(s32 *)(record + 40) = 0x80000;
        Func_02004bc6(8, 1);
        Func_02004b4c(30);
        Call1(Func_02004c1a, 0x17ac);
    } else {
        Call1(Func_02004c22, 0x179f);
        Func_0200486a(0, 8);
        Call1((void (*)())Func_02004b68, 30);
        Func_02004c48(8, 0);
        Func_020048c8();
        Func_02004b7a(20);
        Call3(Func_02004be6, 0, 168, 0x1f8);
        Func_02004b8c(5);
        Call3(Func_02004c88, 0, 0xc000, 20);
        Func_02004d46(152);
        *(u8 *)(Func_02004bd4(8) + 91) = rec7;
        record = Func_02004bde(8);
        *(s32 *)(record + 40) = 0x80000;
        Func_02004c3c(8, 1);
        Func_02004bc2(30);
        Value2(Func_02004c9a, 8, 0);
        if (Value2(Func_02004bf2, 0, 0) == 1) {
            Func_02004c7e(8, 2);
            Func_02004be4(20);
            Func_02004cc4(8, 0);
            Call1((void (*)())Func_02004bf2, 20);
            Func_02004902(8, 0);
            Func_02004c00(30);
            Func_02004ca8(0, 2);
            Func_02004c0e(50);
            Func_02004966();
            Func_02004c18(30);
            Func_02004ca8(8, 3);
            Func_02004d00(8, 0);
        } else {
            bump_step(2);
            Func_02004d1a(8, 0);
        }
        Func_02004cd2(8, 3);
        Func_02004c50(30);
        Call3(Func_02004d54, 8, 0x100, 60);
        Call1(Func_02004d2a, 0x17a4);
        Value2(Func_02004d3a, 8, 0);
        if (Value2(Func_02004c92, 0, 0) == 1) {
            Call3(Func_02004d78, 8, 0x105, 60);
            Func_02004990(8, 0);
            Func_02004c8e(30);
            Func_02004d36(0, 2);
            Func_02004c9c(50);
            Func_020049f4();
            Func_02004ca6(30);
            Func_02004d86(8, 0);
            bump_step(1);
        } else {
            bump_step(1);
            Func_02004cd6(20);
            Func_02004d66(8, 3);
            Func_02004ce4(20);
            Func_02004dc4(8, 0);
        }
        Func_02004cf2(20);
        Func_02004d82(8, 4);
        Func_02004d00(20);
        Func_02004de0(8, 0);
        Func_02004db0(0, 2);
        Func_02004d16(20);
        Func_02004df6(8, 0);
        Func_02004d24(20);
        Call3(Func_02004e20, 8, 0xc000, 20);
        Call3(Func_02004d7a, 8, 0x4ccc, 0x2666);
        Call3(Func_02004da6, 8, 168, 0x1d0);
        Func_02004d4c(60);
        Call3(Func_02004e48, 8, 0x4000, 40);
        Func_02004e42(8, 0, 10);
        Call3(Func_02004e66, 0, 0x102, 60);
        Call3(Func_02004dda, 8, 168, 0x1d8);
    }
    Value2(Func_02004e52, 8, 0);
    if (Value2(Func_02004daa, 0, 0) == 1) {
        Call1(Func_02004e5c, 0x17ab);
        Func_02004e74(8, 0);
        Call1(Func_02004d94, 0x300);
    } else {
        Call1(Func_02004e74, 0x17ad);
        Func_02004db2(30);
        Func_02004e42(8, 3);
        Func_02004dc0(20);
        Call2(Func_02004edc, 0x8000, 0x1000);
        Func_02004efc(8, 1);
        Call2(Func_02004f1e, 0x10000, 0);
        Call2(Func_02004f1e, 0x10003, 1);
        Func_02004f34(30);
        Func_02004f50();
        Func_02004f14();
        Func_02004b64();
        Call2(Func_02004f3a, 0x10000, 0);
        Func_02004f50(30);
        Func_02004ee8(8, 0);
        Func_02004eb8(8, 2);
        Func_02004e1e(20);
        Func_02004efe(8, 0);
        Func_02004ec6(0, 1);
        Call2(Func_02004f38, 0, 0x102);
        Func_02004e3e(60);
        Func_02004f28(8, 0, 10);
        Call1(Func_02004e3e, 0x891);
    }
    Func_02004ed6(8, 5);
    Func_02004e6a();
}
