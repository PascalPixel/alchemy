#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_000010b6[];
extern u8 Data_0000200a[];
extern u8 Data_03001ebc[];
void Func_02005ff6();
void Func_02005ffe();
void Func_02006054();
void Func_020060be();
void Func_020060d8();
void Func_02006104();
void Func_02006156();
void Func_02006174();
void Func_02006184();
void Func_02006196();
void Func_020061f8();
void Func_020062de();
void Func_020062f4();
void Func_0200662a();
void Func_020066aa();
s32 Func_020066e4();
void Func_02006716();
void Func_0200673a();
void Func_0200673c();
void Func_02006742();
void Func_02006748();
void Func_02006764();
void Func_02006772();
void Func_0200677a();
void Func_02006780();
void Func_0200679e();
void Func_020067a4();
void Func_020067a8();
void Func_020067b0();
void Func_020067c6();
void Func_020067c8();
void Func_020067d0();
void Func_020067de();
void Func_020067e4();
s32 Func_020067e6();
void Func_020067e8();
void Func_020067ec();
void Func_020067f0();
void Func_020067f4();
void Func_020067f6();
void Func_020067fc();
void Func_0200683e();
void Func_02006860();
void Func_02006874();
void Func_02006880();
s32 Func_02006882();
void Func_0200688a();
s32 Func_0200688e();
void Func_0200689c();
void Func_020068b8();
void Func_020068c2();
void Func_02006904();
void Func_0200690c();
void Func_02006928();
void Func_0200692a();
void Func_02006932();
void Func_02006934();
void Func_02006960();
void Func_0200699c();
void Func_020069a0();
void Func_020069a4();
void Func_020069a6();
s32 Func_020069b2();
void Func_020069bc();
void Func_020069d2();
void Func_020069e0();
void Func_020069e4();
void Func_020069ec();
void Func_020069ee();
void Func_020069f6();
void Func_02006a0e();
s32 Func_02006a14();
void Func_02006a1c();
void Func_02006a22();
s32 Func_02006a24();
void Func_02006a34();
void Func_02006a38();
void Func_02006a5e();
void Func_02006a60();
void Func_02006a64();
s32 Func_02006a68();
void Func_02006a6a();
void Func_02006a6c();
void Func_02006a7a();
void Func_02006a7e();
void Func_02006a8a();
void Func_02006a8e();
void Func_02006a90();
void Func_02006a96();
s32 Func_02006a9a();
void Func_02006aa8();
void Func_02006ab2();
void Func_02006ab4();
s32 Func_02006ac4();
void Func_02006acc();
s32 Func_02006ad6();
void Func_02006ae2();
void Func_02006af8();
void Func_02006b1c();
void Func_02006b24();
void Func_02006b3a();
void Func_02006b50();
void Func_02006b54();
void Func_02006b66();
void Func_02006b6a();
void Func_02006b7e();
void Func_02006b80();
void Func_02006b8e();
void Func_02006b92();
void Func_02006b96();
void Func_02006b9e();
void Func_02006ba4();
void Func_02006baa();
void Func_02006bac();
void Func_02006bc2();
void Func_02006bd2();
void Func_02006bd8();
s32 Func_02006bda();
void Func_02006bde();
void Func_02006c06();
void Func_02006c0c();
void Func_02006c0e();
void Func_02006c1e();
void Func_02006c20();
void Func_02006c24();
void Func_02006c2e();
void Func_02006c32();
void Func_02006c44();
void Func_02006c52();
void Func_02006c6c();
void Func_02006c7e();
void Func_02006caa();

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

void FieldScene_RunActorFourteenGuestScene(void)
{
    u32 i;
    u8 *rec;
    s32 record;
    s32 base6_10b6;
    s32 base5_200a;
    s32 none;
    s32 v5;

    Func_0200673c(1, 3);
    Func_020066aa(10);
    Call3(Func_0200679e, 1, 0x3000, 0);
    record = Func_020066e4(14);
    Func_0200662a(record, 0);
    Func_0200677a(14, 15);
    Call3(Func_02006748, 14, 0x1880000, 0x1c60000);
    Func_02006104();
    Call3(Func_020067d0, 1, 0xd000, 10);
    Func_02006780(1, 2);
    Call3(Func_020067f4, 1, 0x100, 40);
    Call3(Func_020067f0, 14, 0x5000, 10);
    Func_020067a8(14, 2);
    Func_02006716(20);
    base6_10b6 = (s32)Data_000010b6;
    Func_020067de(base6_10b6);
    Func_020067f6(14, 0);
    Call3(Func_020067a4, 10, 0x1d50000, 0x15c0000);
    base5_200a = 0x200a;
    Func_0200673a(20);
    Func_02005ff6(base5_200a, 10);
    Func_02005ffe(base5_200a, 40);
    Call3(Func_020067c6, 10, 0x1fb0000, 0x15c0000);
    Func_020067f6(1, 2);
    Func_02006764(40);
    Func_020067ec(1, 3);
    Func_02006772(40);
    Call3(Func_020067b0, 1, 0x8000, 0x4000);
    Call3(Func_020067ec, 1, 0x185, 0x1d4);
    Call3(Func_02006880, 1, 0xd000, 60);
    Func_02006054(1, 20);
    Func_02006742((base6_10b6 + 4), 1, 10);
    Call3(Func_020067e8, 1, 0x8000, 0x4000);
    rec = Value1(Func_020067e6, 1);
    rec[90] &= 254;
    none = 0;
    Call3(Func_0200683e, 1, 0x178, 0x1d6);
    Func_020067e4(30);
    {
        u8 value = *(volatile u8 *)&rec[90];
    
        rec[90] = (u8)(value | 1);
    }
    Func_02006874(14, 4);
    Func_020067fc(10);
    Func_020068c2((base6_10b6 + 5));
    Func_020060be(14, 20);
    Call3(Func_0200690c, 1, 0x101, 60);
    Func_0200689c(14, 3);
    Func_020060d8(14, 20);
    Call3(Func_02006928, 1, 0x102, 60);
    Func_020068b8(14, 3);
    Func_0200683e(20);
    Call3(Func_02006932, 14, 0xc000, 20);
    Call2(Func_02006904, 14, 0x100);
    record = Func_02006882(14);
    Func_020067c8(record, 0);
    rec = Value1(Func_0200688e, 14);
    rec[85] = none;
    v5 = 0;
    Func_020069f6(220);
    for (i = 0; i != 30; i++) {
        *(volatile s32 *)((s32)rec + 12) += 0x10000;
        Func_0200688a(1);
        v5 = i;
    }
    rec[85] = 5;
    Func_0200692a(1, 2);
    Func_02006156(1, 10);
    Call3(Func_020069a4, 14, 0x101, 60);
    Call3(Func_020069a0, 14, 0x5000, 10);
    Func_02006174(1, 20);
    Func_02006960(14, 1);
    Func_02006184(14, 20);
    Call3(Func_020069d2, 1, 0x103, 20);
    Func_02006196(1, 30);
    Call3(Func_020069e4, 14, 0x105, 80);
    Call3(Func_020069e0, 14, 0xd000, 40);
    Call3(Func_020069ec, 10, 0x5000, 10);
    Func_02006a60();
    Func_02006a6c();
    Call4(Func_02006a34, 0x1dd0000, -1, 0x14e0000, 0);
    Func_02006860();
    Func_020067f6(1);
    Func_02006a7a();
    Func_02006a8e();
    Func_02006934(20);
    Func_020069bc(10, 4);
    Func_020061f8(10, 10);
    Value2(Func_02006a14, 11, 0);
    Call2(Func_02006a64, 0x66666, 0xcccc);
    Call4(Func_02006a7e, 0x1760000, -1, 0x1d60000, 1);
    Func_02006a8a();
    Call3(Func_02006a5e, 14, 0x5000, 0);
    Call3(Func_02006a6a, 1, 0xe000, 10);
    Func_02006a22(1, 2);
    if (Value2(Func_020069b2, 1, 0) != 0) {
        s32 code;

        Func_0200699c(10);
        Value2(Func_02006a24, 14, 4);
        do {
            code = 0x10c3;
        } while (0);
        for (;;) {
            Call1(Func_02006ab4, code);
            Value2(Func_02006ac4, 14, 0);
            if (Value2(Func_02006a24, 1, 0) != 0)
                break;
            Func_020069e0(20);
            Value2(Func_02006a68, 14, 4);
            Func_020069ee(10);
            code = 0x10c6;
        }
    }
    Func_02006a0e(30);
    Func_02006a96(14, 3);
    Func_02006a1c(20);
    Call1(Func_02006ae2, 0x10c4);
    Func_020062de(14, 30);
    Func_02006ab2(14, 3);
    Func_02006a38(10);
    Func_020062f4(14, 30);
    rec[85] = 0;
    Call3(Func_02006a7e, 14, 0x26666, 0x13333);
    Call4(Func_020069a6, (s32)rec, 0x1cc0000, 0, 0x1680000);
    Func_02006acc(14);
    Func_02006b1c(14, 0);
    record = Func_02006a9a(14);
    Func_020069e0(record, 1);
    Func_02006a7e(30);
    Func_02006b8e(1, 1);
    Func_02006baa();
    Func_02006a90(40);
    Call3(Func_02006b92, 1, 0x103, 40);
    Func_02006b3a(1, 3);
    Func_02006aa8(20);
    rec = Value1(Func_02006ad6, 1);
    {
        u8 value = *(volatile u8 *)&rec[90];
    
        rec[90] = (u8)(value | 1);
    }
    *(volatile s32 *)((s32)rec + 48) = 0x30000;
    *(volatile s32 *)((s32)rec + 52) = 0x20000;
    Func_02006c52(153);
    *(volatile s32 *)((s32)rec + 40) = 0x60000;
    Func_02006b54(1, 7);
    Call3(Func_02006b3a, 1, 0x156, 0x1d6);
    Func_02006b6a(1, 1);
    Func_02006af8(30);
    Func_02006c7e(153);
    *(volatile s32 *)((s32)rec + 40) = 0x60000;
    Func_02006b80(1, 7);
    Call3(Func_02006b66, 1, 0x138, 0x1d6);
    Func_02006b96(1, 1);
    Func_02006b24(30);
    Func_02006caa(153);
    *(volatile s32 *)((s32)rec + 40) = 0x60000;
    Func_02006bac(1, 7);
    Call3(Func_02006b92, 1, 0x116, 0x1e0);
    Func_02006bc2(1, 1);
    Func_02006b50(30);
    Call2(Func_02006c6c, 0x8000, 0x1000);
    Func_02006c6c(0, 1);
    Call3(Func_02006b9e, 1, 0x19999, 0xcccc);
    Func_02006c20(0, 1, 0);
    Func_02006b7e(30);
    Func_02006c06(1, 3);
    Func_02006c0e(0, 4);
    Func_02006c2e(1, 2);
    Func_02006c1e(0, 3);
    Func_02006ba4(20);
    Func_02006c24(1, 2);
    {
        s32 slot = Value1(Func_02006bda, 0);

        if (slot != 0) {
            Func_02006c0c(1, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
        }
    }
    Func_02006c32(1);
    Func_02006c44(1, 0, 0);
    Func_02006bd2(220);
    Func_02006bd8(221);
    Func_02006bde(223);
}
