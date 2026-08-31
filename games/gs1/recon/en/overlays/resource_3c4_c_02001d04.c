#include "types.h"

#define FieldScene_RunMiddleSequence Func_02001d04

/* Audited retained field-scene body at 0x02001d04.
 * The complete production body preserves 48 decoded calls, 1 loop(s), and 37 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02001e40();
void Func_02002d82();
void Func_020039d4();
void Func_02003aee();
void *Func_02004e0a();
s32 Func_02004e10();
void *Func_02004e30();
void Func_02004e36();
void *Func_02004e52();
void Func_02004e6e();
void Func_02004e7a();
void Func_02004e86();
void Func_02004e90();
void *Func_02004e92();
void *Func_02004e9e();
void *Func_02004ea8();
void *Func_02004eb0();
void *Func_02004eba();
void *Func_02004ec2();
void *Func_02004ecc();
void Func_02004ed0();
void *Func_02004ed8();
void *Func_02004ee4();
void Func_02004ef4();
void *Func_02004f16();
void Func_02004f1c();
void *Func_02004f1e();
void *Func_02004f26();
void *Func_02004f32();
void *Func_02004f3c();
void *Func_02004f6a();
void *Func_02004f74();
void *Func_02004f7e();
void *Func_02004f8a();
void *Func_02004f96();
void Func_02004fd4();
void *Func_02004fe2();
void Func_02004ff0();
void *Func_02004ffe();
void *Func_0200500c();
void Func_0200500e();
void Func_02005010();
void *Func_0200501a();
void *Func_0200502a();
void *Func_0200502c();
void Func_02005034();
void Func_0200503e();
void Func_02005040();

void Func_02001d04(void)
{
    void *p1;
    void *p11;
    void *p12;
    void *p13;
    void *p14;
    void *p15;
    void *p16;
    void *p17;
    void *p18;
    void *p19;
    void *p2;
    void *p22;
    void *p23;
    void *p24;
    void *p25;
    void *p26;
    void *p29;
    void *p30;
    void *p31;
    void *p32;
    void *p33;
    void *p35;
    void *p4;
    void *p40;
    void *p42;
    void *p43;
    void *p44;
    void *p46;
    s32 i1;

    p1 = Func_02004e0a();
    for (i1 = 0; i1 < 2; i1++) {
        p2 = Func_02004e30(12);
        if (Func_02004e10(512) == 0) {
            p4 = Func_02004e52(12);
            Func_02002d82();
            Func_02004e90(12, 0, 0);
            Func_02004e36(512);
            Func_02004e6e(12);
            Func_02004e7a(12);
            Func_02004e86(12);
            p11 = Func_02004e92(12);
            p12 = Func_02004e9e();
            *(s32 *)(p11 + 8) = *(s32 *)(p12 + 8);
            p13 = Func_02004ea8(12);
            p14 = Func_02004eb0();
            *(s32 *)(p13 + 12) = *(s32 *)(p14 + 12);
            p15 = Func_02004eba(12);
            p16 = Func_02004ec2();
            *(s32 *)(p15 + 16) = *(s32 *)(p16 + 16);
            p17 = Func_02004ecc();
            *(s32 *)(p17 + 8) = 0;
            p18 = Func_02004ed8();
            *(s32 *)(p18 + 12) = 0;
            p19 = Func_02004ee4();
            *(s32 *)(p19 + 16) = 0;
        } else {
            Func_02004ef4(12);
            Func_02004ed0();
            p22 = Func_02004f16(12);
            *(s32 *)(p22 + 20) = 0;
            p23 = Func_02004f1e(12);
            *(s32 *)(p23 + 40) = 0;
            p24 = Func_02004f26(12);
            *(s32 *)(p24 + 60) = 2147483648;
            p25 = Func_02004f32(12);
            *(u8 *)((u8 *)(p25) + 85) = 0;
            p26 = Func_02004f3c(12);
            *(u16 *)((u8 *)(p26) + 100) = 0;
            Func_02004f1c(512);
            Func_02001e40();
        }
        p29 = Func_02004f6a();
        *(s32 *)(p29 + 20) = 0;
        p30 = Func_02004f74();
        *(s32 *)(p30 + 40) = 0;
        p31 = Func_02004f7e();
        *(s32 *)(p31 + 60) = 2147483648;
        p32 = Func_02004f8a();
        *(u8 *)((u8 *)(p32) + 85) = 0;
        p33 = Func_02004f96();
        *(u16 *)((u8 *)(p33) + 100) = 0;
        Func_02005010(196608, 24576);
        p35 = Func_0200502c();
        *(u8 *)((u8 *)(p35) + 85) = 0;
        Func_02005034(11010048, 524288, 12058624, 1);
        Func_02005040();
        Func_02003aee();
        Func_02004fd4();
        p40 = Func_02004fe2(10);
        *(u16 *)((u8 *)(p40) + 100) = *(u16 *)((u8 *)(p40) + 100);
        Func_02004ff0(11);
        p42 = Func_02004ffe(10);
        *(u16 *)((u8 *)(p42) + 100) = *(u16 *)((u8 *)(p42) + 100);
        p43 = Func_0200500c(11);
        *(u16 *)((u8 *)(p43) + 100) = *(u16 *)((u8 *)(p43) + 100);
        p44 = Func_0200501a();
        *(s32 *)(p44 + 108) = 33594009;
        Func_020039d4(40);
        p46 = Func_0200502a();
        *(u8 *)((u8 *)(p46) + 35) |= 0x2;
        Func_0200500e(512);
    }
    Func_0200503e();
}
