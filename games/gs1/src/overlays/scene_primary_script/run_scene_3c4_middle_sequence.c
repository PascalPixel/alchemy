#include "types.h"

#define FieldScene_RunMiddleSequence Func_02001d04

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002d82();
void Func_020039d4();
void Func_02003aee();
void Func_02004e0a();
s32 Func_02004e10();
s32 Func_02004e30();
void Func_02004e36();
void Func_02004e52();
s32 Func_02004e6e();
s32 Func_02004e7a();
s32 Func_02004e86();
void Func_02004e90();
s32 Func_02004e92();
s32 Func_02004e9e();
s32 Func_02004ea8();
s32 Func_02004eb0();
s32 Func_02004eba();
s32 Func_02004ec2();
s32 Func_02004ecc();
s32 Func_02004ed0();
s32 Func_02004ed8();
s32 Func_02004ee4();
s32 Func_02004ef4();
s32 Func_02004f16();
s32 Func_02004f1c();
s32 Func_02004f1e();
s32 Func_02004f26();
s32 Func_02004f32();
s32 Func_02004f3c();
s32 Func_02004f6a();
s32 Func_02004f74();
s32 Func_02004f7e();
s32 Func_02004f8a();
s32 Func_02004f96();
s32 Func_02004fd4();
s32 Func_02004fe2();
s32 Func_02004ff0();
s32 Func_02004ffe();
s32 Func_0200500c();
void Func_0200500e();
s32 Func_02005010();
s32 Func_0200501a();
s32 Func_0200502a();
s32 Func_0200502c();
void Func_02005034();
void Func_0200503e();
void Func_02005040();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void FieldScene_RunMiddleSequence(void)
{
    s32 saved[28];
    s32 frame;
    s32 outer;
    s32 base;
    s32 zero;
    s32 found;
    s32 inner;
    s32 slot;
    s32 rec;
    s32 tmp;
    s32 dst;
    s32 pa;
    s32 pb;
    s32 v0;
    s32 va;
    s32 v4;
    s32 vb;
    s32 v3;

    Func_02004e0a();
    outer = 0;
    frame = (s32)saved;
    zero = outer;
    base = 12;
    L_02001d20:;
    rec = Value1(Func_02004e30, base);
    if ((12 & *(u8 *)(*(volatile s32 *)(rec + 80) + 9)) == 12) {
        slot = outer + 0x200;
        if (Value1(Func_02004e10, slot) == 0) {
            Func_02004e52(base);
            Func_02002d82();
            Func_02004e90(base, 0, 0);
            Func_02004e36(slot);
            goto L_02001f40;
        }
    }
    goto L_02001de4;
    L_02001d5e:;
    rec = Value1(Func_02004e6e, base);
    *(s32 *)(frame + 8) = *(volatile s32 *)(rec + 8);
    rec = Value1(Func_02004e7a, base);
    *(s32 *)(frame + 12) = *(volatile s32 *)(rec + 12);
    rec = Value1(Func_02004e86, base);
    *(s32 *)(frame + 16) = *(volatile s32 *)(rec + 16);
    dst = Func_02004e92(base);
    pa = inner + 12;
    rec = Value1(Func_02004e9e, pa);
    *(volatile s32 *)(dst + 8) = *(volatile s32 *)(rec + 8);
    dst = Value1(Func_02004ea8, base);
    rec = Value1(Func_02004eb0, pa);
    *(volatile s32 *)(dst + 12) = *(volatile s32 *)(rec + 12);
    dst = Value1(Func_02004eba, base);
    rec = Value1(Func_02004ec2, pa);
    *(volatile s32 *)(dst + 16) = *(volatile s32 *)(rec + 16);
    rec = Func_02004ecc(pa);
    *(volatile s32 *)(rec + 8) = *(s32 *)(frame + 8);
    rec = Func_02004ed8(pa);
    *(volatile s32 *)(rec + 12) = *(s32 *)(frame + 12);
    found = inner;
    rec = Func_02004ee4(pa);
    *(volatile s32 *)(rec + 16) = *(s32 *)(frame + 16);
    goto L_02001e56;
    L_02001de4:;
    rec = Value1(Func_02004ef4, base);
    if ((*(volatile s32 *)(rec + 16) >> 20) != 9) {
        goto L_02001f36;
    }
    tmp = Value1(Func_02004ed0, outer + 0x200);
    if (tmp != 0) {
        goto L_02001f36;
    }
    rec = Value1(Func_02004f16, base);
    *(volatile s32 *)(rec + 20) = tmp;
    rec = Value1(Func_02004f1e, base);
    *(volatile s32 *)(rec + 40) = tmp;
    rec = Func_02004f26(base);
    *(volatile s32 *)(rec + 60) = -0x80000000;
    *(u8 *)(Func_02004f32(base) + 85) = tmp;
    *(volatile u16 *)(Func_02004f3c(base) + 100) = tmp;
    found = outer;
    for (inner = 0; inner < outer; inner++) {
        if (Value1(Func_02004f1c, 0x200 + inner) == 0) {
            goto L_02001d5e;
        }
    }
    L_02001e56:;
    pb = found + 12;
    rec = Func_02004f6a(pb);
    *(volatile s32 *)(rec + 20) = zero;
    rec = Func_02004f74(pb);
    *(volatile s32 *)(rec + 40) = zero;
    rec = Func_02004f7e(pb);
    *(volatile s32 *)(rec + 60) = -0x80000000;
    *(u8 *)(Func_02004f8a(pb) + 85) = zero;
    *(volatile u16 *)(Func_02004f96(pb) + 100) = zero;
    Value2(Func_02005010, 0x30000, 0x6000);
    *(u8 *)(Func_0200502c() + 85) = zero;
    Call4(Func_02005034, 0xa80000, 0x80000, 0xb80000, 1);
    Func_02005040();
    Func_02003aee(pb);
    rec = Value1(Func_02004fd4, pb);
    if ((*(volatile s32 *)(rec + 8) >> 20) == 8) {
        vb = (Func_02004fe2(10) + 100);
        *(volatile u16 *)(vb) = (*(volatile u16 *)(vb) + 1);
        v0 = (Func_02004ff0(11) + 100);
        v3 = (*(volatile u16 *)(v0) - 1);
    } else {
        va = (Func_02004ffe(10) + 100);
        v4 = (*(volatile u16 *)(va) - 1);
        *(volatile u16 *)(va) = v4;
        v0 = (Func_0200500c(11) + 100);
        v3 = (*(volatile u16 *)(v0) + 1);
    }
    *(volatile u16 *)(v0) = v3;
    rec = Func_0200501a(pb);
    *(volatile s32 *)(rec + 108) = 0x2009a99;
    Func_020039d4(40);
    *(u8 *)(Func_0200502a(pb) + 35) |= 2;
    Func_0200500e(0x200 + found);
    goto L_02001f40;
    L_02001f36:;
    outer = outer + 1;
    base = base + 1;
    if (outer <= 2) {
        goto L_02001d20;
    }
    L_02001f40:;
    Func_0200503e();
}
