#include "types.h"

/* Absolute dialogue ID shared by the prompt and its completion flag. */
extern u8 Data_00002644[];


#define Function Func_020028e8

void Func_02005422();
void Func_02006b5c();
void Func_02006b7e();
void Func_02006b8c();
void Func_02006b98();
void Func_02006ba6();
void Func_02006baa();
u8 *Func_02006bb4();
void Func_02006bb8();
void Func_02006bd6();
u8 *Func_02006bd8();
void Func_02006bda();
u8 *Func_02006bf0();
void Func_02006bfc();
void Func_02006c36();
void Func_02006c7c();
void Func_02006c84();
void Func_02006c86();
void Func_02006c96();
void Func_02006cf8();
void Func_02006cf8_a();
void Func_02006cfe();
void Func_02006d02();
void Func_02006d14();
void Func_02006d1c();
void Func_02006d2c();
void Func_02006d2e();
u8 *Func_02006d32();
void Func_02006d3a();
void Func_02006d3e();
void Func_02006d4c();
void Func_02006d54();
void Func_02006d62();
void Func_02006d62_a();
void Func_02006d92();
void Func_02006da4();
void Func_02006db2();
void Func_02006dcc();
void Func_02006dd8();
void Func_02006dda();
void Func_02006dde();
u8 *Func_02006df8();
void Func_02006dfa();
void Func_02006dfe();
void Func_02006dfe_a();
void Func_02006e02();
void Func_02006e20();
void Func_02006e24();
void Func_02006e24_a();
void Func_02006e30();
void Func_02006e50();
void Func_02006e5a();
u8 *Func_02006e5c();
void Func_02006e64();
void Func_02006e66();
void Func_02006e68();
void Func_02006e70();
void Func_02006e78();
void Func_02006e8c();
void Func_02006e8c_a();
void Func_02006e9e();
void Func_02006ea0();
void Func_02006ea2();
void Func_02006eb0();
void Func_02006ec0();
void Func_02006ed6();
void Func_02006eee();
void Func_02006ef0();
void Func_02006ef2();
void Func_02006ef8();
void Func_02006f00();
void Func_02006f08();
u8 *Func_02006f08_a();
void Func_02006f16();
void Func_02006f1a();
void Func_02006f42();
void Func_02006f48();
void Func_02006f4a();
void Func_02006f50();
void Func_02006f62();
void Func_02006f6a();
void Func_02006f6a_a();
void Func_02006f78();
void Func_02006f8a();
void Func_02006faa();
void Func_02007096();
void Func_020070c8();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ u8 *Pointer1(u8 *(*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ u8 *Pointer2(u8 *(*f)(), s32 a0, s32 a1)
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

static __inline__ u8 *Pointer4(u8 *(*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void Function(void)
{
    u8 *rec;
    u8 *rec7;
    u8 *record;
    s32 base6_200e79c;
    s32 base10_2644;
    u8 *p6;
    u8 *tiles;

    rec7 = Pointer1(Func_02006bf0, 0);
    Func_02006bd6();
    Func_02006d62();
    Call2(Func_02006d92, 0x16666, 6);
    Call2(Func_02006cfe, 0x30000, 0x6000);
    Call4(Func_02006d14, 0x17880000, -1, 0xd680000, 1);
    Call3(Func_02006c36, 0, 0xcccc, 0x6666);
    rec = 0;
    Func_02006c96(0, 2);
    rec7[91] = 0;
    Func_02006b5c((s32)rec7);
    if (*(s32 *)((s32)rec7 + 16) > 0xd680000) {
        if (*(s32 *)((s32)rec7 + 8) <= 0x176e0000) {
            goto L_0200297c;
        }
        Call4(Func_02006b7e, (s32)rec7, 0x176e0000, *(s32 *)((s32)rec7 + 12), 0xd7d0000);
        Func_02006b8c((s32)rec7);
    } else {
        if (*(s32 *)((s32)rec7 + 8) > 0x177a0000) {
            Call4(Func_02006b98, (s32)rec7, 0x177a0000, *(s32 *)(rec + 12), 0xd480000);
            Func_02006ba6((s32)rec7);
        }
    }
    L_0200297c:;
    Call4(Func_02006baa, (s32)rec7, 0x17690000, 0, 0xd680000);
    Func_02006bb8((s32)rec7);
    Func_02006cf8(0, 1);
    Func_02006d62_a(0, 0, 40);
    Func_02006dfe();
    Func_02006d2e(0, 2);
    Func_02006c84(20);
    Func_02006d1c(0, 28);
    rec = Pointer4(Func_02006bd8, 22, (*(s32 *)((s32)rec7 + 8) + 0x20000), 0x260000, *(s32 *)((s32)rec7 + 16));
    if ((s32)rec != 0) {
        rec[85] = 0;
        p6 = *(u8 **)(rec + 80);
        p6[38] = 0;
        *(u8 *)(((s32)p6 + 38) + 1) = 0;
        p6[5] &= -33;
        p6[9] &= 15;
        tiles = Pointer2(Func_02006bb4, 17, 0x608);
        Func_02006c7c(242);
        tiles += 0x400;
        Func_02006bfc(p6[28], 128, tiles);
        Func_02006bda(17);
        Func_02006cf8_a(20);
        *(s32 *)((s32)rec + 108) = 0x200813d;
        Func_02006d02(80);
    }
    base6_200e79c = 0x200e79c;
    record = Func_02006d32(*(s32 *)base6_200e79c);
    {
        s32 shown = 0x3000;

        *(u16 *)(record + 6) = shown;
    }
    Call3(Func_02006e20, *(s32 *)base6_200e79c, 0x100, 0);
    Func_02006dd8(*(s32 *)base6_200e79c, 2);
    base10_2644 = (s32)Data_00002644;
    Func_02006e02(base10_2644);
    Func_02006e24(*(s32 *)base6_200e79c, 0, 80);
    if ((s32)rec != 0) {
        Func_02006c86((s32)rec);
    }
    Func_02006dde(0, 1);
    Func_02006d54(40);
    Func_02006dfe_a(*(s32 *)base6_200e79c, 6, 40);
    Func_02006e50(*(s32 *)base6_200e79c, 0, 20);
    Call3(Func_02006e64, 0, 0xe000, 0);
    Call3(Func_02006e70, *(s32 *)base6_200e79c, 0xd000, 20);
    Func_02006e78((*(s32 *)base6_200e79c | 0x9000), 0, 40);
    Func_02006e30(*(s32 *)base6_200e79c, 4);
    Func_02006e8c((*(s32 *)base6_200e79c | 0x9000), 0, 20);
    Func_02006e9e(*(s32 *)base6_200e79c, 0x3000, 20);
    Func_02006ea0(*(s32 *)base6_200e79c, 0, 10);
    Call3(Func_02006dfa, *(s32 *)base6_200e79c, 0xcccc, 0x6666);
    Func_02006e5a(*(s32 *)base6_200e79c, 2);
    rec = Pointer1(Func_02006df8, 55);
    Call4(Func_02006d2c, (s32)rec, 0x177a0000, *(s32 *)((s32)rec + 12), 0xd480000);
    Func_02006d3a((s32)rec);
    Call4(Func_02006d3e, (s32)rec, 0x17710000, 0, 0xd580000);
    Func_02006d4c((s32)rec);
    Func_02006e8c_a(55, 1);
    Call3(Func_02006ef8, *(s32 *)base6_200e79c, 0x5000, 10);
    Func_02006ec0(*(s32 *)base6_200e79c, 1);
    Func_02006f08((*(s32 *)base6_200e79c | 0x1000), 0, 20);
    Call3(Func_02006e66, *(s32 *)base6_200e79c, 0x10000, 0x8000);
    *(u8 *)(Func_02006e5c(55) + 90) &= 254;
    Func_02006ed6(55, 2);
    Call4(Func_02006da4, (s32)rec, 0x176d0000, 0, 0xd600000);
    Func_02006db2((s32)rec);
    Func_02006ef2(55, 1);
    Func_02006e68(10);
    Func_02006f00(55, 2);
    Call4(Func_02006dcc, (s32)rec, 0x17710000, 0, 0xd580000);
    Func_02006dda((s32)rec);
    Func_02006f1a(55, 1);
    Func_02006e24_a((base10_2644 + 6), 1);
    *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    Func_02006ea2(242);
    Func_02006eb0(20);
    Func_02006f48(*(s32 *)base6_200e79c, 4);
    Func_02006faa(*(s32 *)base6_200e79c, 0, 10);
    Func_02006f62(0, 3);
    Func_02006f6a(*(s32 *)base6_200e79c, 3);
    Func_02006f6a_a(*(s32 *)base6_200e79c, 2);
    record = Pointer1(Func_02006f08_a, 0);
    if (record != 0) {
        Func_02006f4a(*(s32 *)base6_200e79c, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02006f78(*(s32 *)base6_200e79c);
    Func_02006f8a(*(s32 *)base6_200e79c, 0, 0);
    Func_02007096();
    Call2(Func_020070c8, 0x10000, 6);
    Func_02006f16(20);
    Func_02005422();
    Func_02006f50(*(s32 *)base6_200e79c);
    Call1(Func_02006ef0, 0x234);
    Call1(Func_02006eee, 0x85d);
    Func_02006f42();
}
