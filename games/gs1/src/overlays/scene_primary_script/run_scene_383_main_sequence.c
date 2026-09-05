#include "types.h"

#define FieldScene_RunMainSequence Func_02000de4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000125f[];
void Func_02005416();
void Func_0200542e();
void Func_020054a8();
void Func_020054de();
void Func_0200550e();
void Func_02005526();
void Func_02005536();
void Func_0200553e();
void Func_020055d2();
void Func_020055dc();
void Func_0200569e();
void Func_020056a0();
void Func_020056b2();
void Func_020056d6();
void Func_020056e0();
void Func_020056f4();
void Func_020056fa();
void Func_02005716();
void Func_02005740();
void Func_0200574e();
void Func_020057a8();
void Func_020057c8();
void Func_02005800();
void Func_02005808();
void Func_0200580a();
void Func_02005824();
void Func_0200583c();
void Func_0200585e();
void Func_02005878();
void Func_02005888();
void Func_0200589c();
void Func_020058c8();
void Func_020058d2();
void Func_020058dc();
void Func_020058ea();
void Func_02005908();
void Func_0200592a();
void Func_0200593c();
void Func_0200594e();
void Func_02005b2a();
void Func_02005b46();
void Func_02005b56();
void Func_02005b6e();
s32 Func_02005b70();
void Func_02005b92();
void Func_02005bc4();
void Func_02005bda();
s32 Func_02005be4();
void Func_02005bf0();
void Func_02005bfc();
u8 *Func_02005c0a();
void Func_02005c1e();
void Func_02005c28();
void Func_02005c2c();
void Func_02005c36();
void Func_02005c4c();
void Func_02005c5e();
void Func_02005c64();
void Func_02005c68();
void Func_02005c8c();
void Func_02005c9c();
void Func_02005c9e();
void Func_02005ca8();
void Func_02005cc8();
void Func_02005cca();
void Func_02005ce6();
void Func_02005cf4();
void Func_02005cfa();
void Func_02005cfe();
void Func_02005d06();
void Func_02005d18();
void Func_02005d1e();
void Func_02005d26();
void Func_02005d48();
void Func_02005d58();
void Func_02005d5c();
s32 Func_02005d6c();
void Func_02005d70();
void Func_02005d78();
void Func_02005d7a();
void Func_02005d82();
s32 Func_02005d86();
u8 *Func_02005d8e();
void Func_02005d98();
void Func_02005dba();
void Func_02005dcc();
void Func_02005dd0();
void Func_02005dd2();
void Func_02005dda();
void Func_02005de6();
void Func_02005df6();
void Func_02005e16();
void Func_02005e18();
void Func_02005e44();
void Func_02005e50();
void Func_02005e66();
void Func_02005e86();
void Func_02005e8a();
void Func_02005e90();
void Func_02005ea4();
void Func_02005ea6();
void Func_02005eb0();
void Func_02005ec2();
void Func_02005ed0();
void Func_02005ed4();
void Func_02005edc();
void Func_02005ee8();
void Func_02005eea();
void Func_02005f32();
void Func_02005f34();
void Func_02005f3e();
void Func_02005f60();
void Func_02005f6c();
void Func_02005f7a();
void Func_02005f84();
void Func_02005f8e();
void Func_02005f90();
void Func_02005f92();
void Func_02005f9e();
void Func_02005fb8();
void Func_02005fce();
void Func_02005fd6();
void Func_02005fea();
void Func_02005fec();
void Func_02005ffe();
void Func_02006016();
void Func_02006020();
void Func_02006024();
void Func_0200605a();
void Func_02006060();
void Func_02006064();
void Func_02006066();
void Func_0200606e();
void Func_02006076();
void Func_020060a8();
void Func_020060b2();
void Func_020060dc();
void Func_020060ee();
void Func_020060f8();
void Func_0200611a();
void Func_02006128();
void Func_02006138();
void Func_02006158();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunMainSequence(void)
{
    u32 i;
    u8 *record;
    s32 base8_125f;

    Func_02005b2a(20);
    *(u8 *)(Func_02005b70(2) + 91) = 0;
    Func_02005bf0(2, 4, 0);
    Func_02005b46(40);
    base8_125f = (s32)Data_0000125f;
    Func_02005c28(base8_125f);
    Func_02005b56(20);
    Func_02005416(2, 20);
    Call3(Func_02005c68, 2, 0x101, 0);
    Func_02005b6e(50);
    Func_0200542e(2, 30);
    Call3(Func_02005bfc, 2, 0x178, 0x188);
    Func_020054a8(2, 0);
    Func_02005b92(40);
    Func_020054de();
    Func_02005c5e(0, 1);
    *(u8 *)(Func_02005be4(0) + 90) &= 254;
    Call3(Func_02005c36, 0, 0x180, 0x1a8);
    Func_02005bc4(1);
    {
        u8 *record = Func_02005c0a(0);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    Func_02005bda(30);
    Func_02005cca(2, 0);
    Call2(Func_02005cf4, 0, 0x102);
    Call2(Func_02005cfe, 1, 0x102);
    Func_02005bfc(60);
    Func_02005c9c(0, 3);
    Func_0200550e(1, 3, 30);
    Call3(Func_02005d18, 2, 0x101, 0);
    Func_02005c1e(60);
    Func_02005ce6(2, 1);
    Func_02005c2c(10);
    Func_02005536(2, 3, 20);
    Func_02005d26(2, 0);
    Func_02005d06(1, 2);
    Func_02005c4c(10);
    Func_02005526(1, 0, 30);
    Func_02005d1e(0, 2);
    Func_02005c64(10);
    Func_0200553e(0, 1, 40);
    Call3(Func_02005d7a, 0, 0x102, 0);
    Call3(Func_02005d86, 1, 0x102, 0);
    Func_02005c8c(60);
    Call3(Func_02005d98, 2, 0x102, 0);
    Func_02005c9e(60);
    Value2(Func_02005d86, 2, 0);
    Func_02005d78(0, 2, 0);
    Func_02005d82(1, 2, 0);
    Func_02005cfa(0, 0);
    Func_02005cc8(30);
    Func_020055d2(2, 3, 10);
    Call3(Func_02005d58, 2, 0x180, 0x198);
    Func_02005ce6(10);
    Func_02005ca8((base8_125f + 5), 1);
    Func_02005dd0((base8_125f + 6));
    Func_02005dd2(2, 1, 0);
    Func_020055dc(0, 1, 20);
    Call3(Func_02005e18, 1, 0x102, 0);
    Func_02005d1e(60);
    Func_02005de6(1, 1);
    *(u8 *)(Func_02005d6c(1) + 90) &= 254;
    Call3(Func_02005dba, 1, 0x160, 0x198);
    Func_02005d48(1);
    {
        u8 *record = Func_02005d8e(1);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    Func_02005d5c(10);
    Call3(Func_02005dda, 2, 0x170, 0x198);
    Func_02005d70(20);
    Call3(Func_02005df6, 0, 0x170, 0x1a8);
    Func_02005e50(0, 1, 0);
    Func_02005e16(2);
    Func_020056b2(2, 1);
    Func_020056a0(1, 4, 10);
    Func_02005e90(1, 0);
    Func_020056f4();
    Func_0200569e(2, 0, 30);
    Func_02005ea6(2, 0);
    Func_02005e86(0, 1);
    Func_02005dcc(30);
    Func_020056d6(0, 3, 30);
    Func_020056e0(2, 3, 10);
    Func_02005ed0(2, 0);
    Func_02005eb0(1, 2);
    Func_02005ec2(1, 0, 0);
    Func_02005eea(1, 0);
    Func_02005ed4(0, 1, 0);
    Func_02005716(0, 3, 40);
    Func_02005ee8(0, 2, 0);
    Func_020056fa(1, 2, 30);
    Call3(Func_02005f34, 0, 0x101, 0);
    Call3(Func_02005f3e, 1, 0x101, 0);
    Func_02005e44(60);
    Func_0200574e(2, 3, 20);
    Func_02005f3e(2, 0);
    Call3(Func_02005f60, 1, 0x103, 0);
    Func_02005e66(60);
    Func_02005740(1, 0, 10);
    Func_02005f60(1, 0);
    Call3(Func_02005f84, 2, 0x100, 0);
    Func_02005e8a(60);
    Func_02005f7a(2, 0);
    Call3(Func_02005f9e, 1, 0x102, 0);
    Func_02005ea4(60);
    Func_02005e66((base8_125f + 13), 1);
    Func_02005f8e((base8_125f + 14));
    Func_02005f90(2, 0, 0);
    Func_02005f92(1, 0, 0);
    Call3(Func_02005fd6, 0, 0x102, 0);
    Func_02005edc(60);
    Func_02005800(2, 0);
    Func_020060a8();
    Func_020057a8(1, 30);
    Func_0200583c();
    Func_02005fce(2, 1, 0);
    Func_02005808(2, 3, 10);
    Func_020057c8(2, 20);
    Func_02005fea(2, 0, 0);
    Func_02005824(0, 3, 20);
    Func_02005fec(1, 1);
    Func_02005f32(20);
    Func_0200583c(1, 4, 10);
    Func_0200580a(1, 30);
    Func_02006024(2, 1, 0);
    Call3(Func_02006066, 2, 0x101, 0);
    Func_02005f6c(30);
    Func_0200585e(0, 1, 10);
    Func_02006016(0, 3);
    Func_02005888(1, 3, 30);
    Func_0200605a(0, 2, 0);
    Func_02006064(1, 2, 0);
    Func_0200606e(2, 0, 0);
    Call3(Func_020060b2, 2, 0x106, 0);
    Func_02005fb8(60);
    Func_02005878(2, 10);
    Func_02006060(0, 3);
    Func_020058d2(1, 3, 20);
    Func_020058dc(2, 4, 20);
    Func_0200589c(2, 20);
    Call3(Func_020060ee, 0, 0x101, 0);
    Call3(Func_020060f8, 1, 0x101, 0);
    Func_02005ffe(60);
    Func_02005908(2, 3, 10);
    Func_020058c8(2, 30);
    Call3(Func_0200611a, 0, 0x105, 0);
    Func_02006020(60);
    Func_0200592a(2, 4, 10);
    Func_020058ea(2, 30);
    Func_0200593c(2, 3, 10);
    Func_020060dc(0, 3);
    Func_0200594e(1, 3, 20);
    Func_02006076(2, 1);
    Call1(Func_02006138, base8_125f + 22);
    Func_02006128(2, 1);
    Func_02006158(2, 0);
}
