#include "types.h"

#define FieldScene_RunPrimarySequence Func_02000af8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_020015b8();
void Func_020015be();
void Func_020015c4();
void Func_020015ca();
void Func_020015d0();
void Func_020015d6();
void Func_020015dc();
void Func_02001cca();
void Func_02001cd6();
void Func_02001ce2();
void Func_02001d04();
void Func_02001d10();
s32 Func_02001d3e();
void Func_02001d40();
void Func_02001d4c();
void Func_02001d88();
void Func_02001d8a();
void Func_02001d96();
void Func_02001da4();
void Func_02001ddc();
void Func_02001de2();
void Func_02001de8();
void Func_02001dee();
void Func_02001df4();
void Func_02001dfa();
void Func_02001dfe();
void Func_02001e00();
void Func_02001e0c();
void Func_02001e1a();
void Func_02001e28();
void Func_02001e36();
void Func_02001e44();
void Func_02001e52();
void Func_02001e5e();
void Func_02001e72();
void Func_02001e74();
void Func_02001e7e();
void Func_02001e82();
void Func_02001e88();
void Func_02001e90();
void Func_02001e92();
void Func_02001e9c();
void Func_02001e9e();
void Func_02001ea8();
void Func_02001eb4();
void Func_02001ece();
void Func_02001ed8();
void Func_02001ee4();
void Func_02001eee();
void Func_02001ef8();
s32 Func_02001efc();
void Func_02001f02();
void Func_02001f0c();
void Func_02001f10();
void Func_02001f16();
void Func_02001f1c();
void Func_02001f20();
void Func_02001f30();
void Func_02001f42();
void Func_02001f6e();
s32 Func_02001f98();
void Func_02001f9a();
void Func_02001f9e();
void Func_02001fa2();
void Func_02001fae();
void Func_02001fb4();
void Func_02001fb6();
void Func_02001fc2();
void Func_02001fde();
void Func_02001ff4();
void Func_02001ffa();
void Func_02002008();
void Func_0200200a();
void Func_02002010();
void Func_02002030();
void Func_02002040();
s32 Func_02002042();
void Func_0200204c();
void Func_02002060();
void Func_02002062();
void Func_0200206c();
void Func_020020ae();
void Func_020020e6();
void Func_02002114();
void Func_0200212a();
void Func_02002136();

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

void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Func_02001d10();
    Func_02001d88(0, 15);
    record = Func_02001d3e(0);
    Func_02001d04(record, 0);
    Func_02001cca(1);
    Call1(Func_02001d40, 0x200976c);
    Func_02001cd6(1);
    Call1(Func_02001d4c, 0x2009844);
    Func_02001ce2(1);
    Func_020015b8(9);
    Func_020015be(10);
    Func_020015c4(11);
    Func_020015ca(12);
    Func_020015d0(13);
    Func_020015d6(14);
    Func_020015dc(15);
    Call2(Func_02001da4, 8, 0x200939c);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x203;
    Func_02001e5e();
    Func_02001e72();
    Call1(Func_02001d8a, 0x12c);
    Func_02001e90(147);
    Func_02001d96(100);
    Func_02001ddc(9);
    Func_02001de2(10);
    Func_02001de8(11);
    Func_02001dee(12);
    Func_02001df4(13);
    Func_02001dfa(14);
    Func_02001e00(15);
    Call3(Func_02001dfe, 9, 0x30000, 0x18000);
    Call3(Func_02001e0c, 10, 0x30000, 0x18000);
    Call3(Func_02001e1a, 11, 0x30000, 0x18000);
    Call3(Func_02001e28, 12, 0x30000, 0x18000);
    Call3(Func_02001e36, 13, 0x30000, 0x18000);
    Call3(Func_02001e44, 14, 0x30000, 0x18000);
    Call3(Func_02001e52, 15, 0x30000, 0x18000);
    Func_02001e74(9, 0, 100);
    Func_02001e7e(10, 60, 100);
    Func_02001e88(11, 120, 100);
    Func_02001e92(12, 180, 100);
    Func_02001e9c(13, 240, 100);
    Call3(Func_02001ea8, 14, 0x140, 100);
    Call3(Func_02001eb4, 15, 0x17c, 100);
    Func_02001e72(40);
    Call3(Func_02001f1c, 8, 0x101, 0);
    Func_02001e82(20);
    Func_02001ee4(9, 0, 0);
    Func_02001eee(10, 0, 0);
    Func_02001ef8(11, 0, 0);
    Func_02001f02(12, 0, 0);
    Func_02001f0c(13, 0, 0);
    Func_02001f16(14, 0, 0);
    Func_02001f20(15, 0, 0);
    Func_02001ece(100);
    rec7 = Func_02001efc(18);
    *(s32 *)(rec7 + 24) = 0x1999;
    *(s32 *)(rec7 + 28) = 0x1999;
    Call3(Func_02001f42, 18, 0xac0000, 0x1540000);
    Func_02001f30(8);
    Func_02001e9e(1);
    Func_02001f6e(8, 1);
    Call3(Func_02001f9a, 8, 0x3000, 0);
    Func_02002010(29);
    Call1(Func_02001f10, 0x8f0);
    for (i = 0; i < 32; i++) {
        *(s32 *)(rec7 + 24) += 0xccc;
        *(s32 *)(rec7 + 28) += 0xccc;
        Func_02001ed8(1);
    }
    Call3(Func_02001fde, 8, 0x101, 60);
    Func_02001fb6(8, 2);
    Call3(Func_02001fa2, 8, 168, 0x154);
    Call3(Func_02001fae, 8, 200, 0x154);
    Call3(Func_02001ffa, 8, 0x8000, 0);
    rec7 = Func_02001f98(17);
    *(s32 *)(rec7 + 24) = 0x12666;
    *(s32 *)(rec7 + 28) = 0x12666;
    *(s32 *)(rec7 + 8) = 0xac0000;
    *(s32 *)(rec7 + 12) = 0xa00000;
    *(s32 *)(rec7 + 16) = 0x1540000;
    {
        s32 shown = 0;
    
        *(u16 *)(rec7 + 6) = shown;
    }
    *(s32 *)(rec7 + 68) = 0x6666;
    *(s32 *)(rec7 + 72) = 0x30000;
    Func_02001f9e(20);
    Func_02002008(8, 6, 20);
    Func_020020ae(147);
    Func_02001fb4(20);
    Call2(Func_02001ff4, 8, 0x20093ac);
    Func_02001fc2(80);
    Func_02002062(17, 1);
    Call3(Func_02002008, 17, 0x10000, 0x8000);
    *(s32 *)(rec7 + 68) = 0x1999;
    *(s32 *)(rec7 + 72) = 0xb333;
    Func_020020e6(153);
    *(s32 *)(rec7 + 40) = 0x80000;
    Call3(Func_02002040, 17, 132, 0x168);
    Call3(Func_0200204c, 18, 132, 0x168);
    Func_0200200a(40);
    Func_0200206c(17, 0, 0);
    rec7 = Func_02002042(8);
    *(s32 *)(rec7 + 24) = 0x10000;
    *(s32 *)(rec7 + 28) = 0x10000;
    {
        s32 shown = 0x5000;
    
        *(u16 *)(rec7 + 6) = shown;
    }
    Func_02002030(40);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x202;
    Func_0200212a();
    Func_02002136();
    Func_02002114(13);
    Func_02002060();
}
