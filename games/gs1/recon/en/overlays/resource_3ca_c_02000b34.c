#include "types.h"

/* AUDITED GENERATED PRESENTATION FINALE for FieldScene_ClosePresentationSequence:
 * 35 calls, palette ramps, blend-register setup, and runtime handoff. */

#define FieldScene_ClosePresentationSequence Func_02000b34

void Func_02000c44();
void Func_020011a8();
void Func_02001ef8();
void Func_02001f02();
void Func_02001f1c();
void Func_02001f38();
void Func_02001f4a();
void Func_02001f60();
void Func_02001f62();
void Func_02001f6c();
void Func_02001f74();
void Func_02001f90();
void *Func_02001f92();
void Func_02001f96();
void Func_02001fa8();
void Func_02001fae();
void Func_02001fbe();
void *Func_02001fc6();
void Func_0200200a();
void Func_02002010();
void Func_02002040();
void Func_02002080();
void Func_02002084();
void Func_020020c2();
void Func_020020cc();
void Func_02002106();
void Func_02002124();
void Func_02002138();
void Func_0200218a();
void Func_02002196();
void Func_020021fe();
void Func_02002202();

void Func_02000b34(void)
{
    u8 *runtime;
    s32 base;
    void *p2;
    void *p22;
    s32 i1;
    s32 i2;
    s32 i3;
    s32 i4;
    s32 phase;

    runtime = *(u8 **)0x03001e70;

    Func_02001f02();
    p2 = Func_02001fc6();
    Func_02001f4a(0, 312, 232);
    Func_02001f96(0, 49152, 0);
    Func_02001f1c(40);
    Func_0200200a(140);
    for (i1 = 0; i1 <= 15; i1++) {
        *(volatile u16 *)0x05000000 = (i1 << 11) | (i1 << 5);
        Func_02001f38(10);
    }
    *(volatile u16 *)0x05000000 = 0x7e00;
    for (i2 = 2; i2 >= 0; i2--) {
        Func_02002040(212);
        *(volatile u16 *)0x04000050 = 3;
        Func_02001f62(3);
        *(volatile u16 *)0x04000050 = 0x810;
        Func_02001f6c(65);
    }
    *(u32 *)0x020097e8 = 1;
    *(u32 *)0x020097ec = 0;
    Func_02001ef8(33587605, 3200, 0, 33593324);
    *(u32 *)0x02009804 = 1;
    Func_02001f92(20);
    Func_02002080(163);
    Func_02001f90(65536, 65536, 65536);
    Func_02001fae(60);
    Func_02001fa8(131072, 131072, 65536);
    Func_02001fc6(60);
    Func_02001fbe(196608, 196608, 65536);
    *(u32 *)0x02009808 = 0;
    Func_02001f60(33587561, 3200, 0, 33593332);
    phase = 0;
    do {
        *(s32 *)(runtime + 320) += 0x3333;
        *(s32 *)(runtime + 368) += 0x3333;
        phase += 0x3333;
        Func_02001f74(1);
    } while (phase <= 0xe666);
    p22 = Func_02001f92(33587561);
    *(u32 *)0x020097f8 = 0;
    *(volatile u16 *)0x04000052 = (*(volatile u16 *)0x04000052 & 0xfffc) | 0x0a;
    *(volatile u16 *)0x04000050 = (*(volatile u16 *)0x04000050 & 0xfffc) | 0x0a;
    *(volatile u16 *)0x0400000e = (*(volatile u16 *)0x0400000e & 0xfffc) | 0x0400;
    *(u32 *)0x020097f4 = 0;
    Func_0200218a(288);
    Func_02002010(1);
    Func_02002196(145);
    *(volatile u16 *)0x04000054 = 191;
    for (i3 = 0; i3 <= 16; i3++) {
        *(volatile u16 *)0x05000000 = i3;
        Func_020020c2(1);
    }
    Func_020020cc(40);
    Func_020020c2(-1, -1, 58982);
    *(u32 *)0x02009804 = *(u32 *)(runtime + 320);
    *(u32 *)0x02009808 = *(u32 *)(runtime + 368);
    *(u32 *)0x020097fc = 1;
    for (i4 = 16; i4 >= 0; i4--) {
        *(volatile u16 *)0x05000000 = i4;
        Func_02002106(8);
    }
    Func_02002084(33587377, 3200);
    Func_02002202(80);
    Func_020021fe();
    Func_02002124(20);
    Func_02002138();
    Func_020011a8();
}
