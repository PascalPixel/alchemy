#include "types.h"

#define FieldScene_RunThreeActorChoreography Func_020019e8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003c1c();
void Func_02003c52();
s32 Func_02003c94();
s32 Func_02003cac();
void Func_02003cae();
void Func_02003cc6();
void Func_02003cf4();
void Func_02003cfa();
void Func_02003d0c();
void Func_02003d18();
void Func_02003d22();
void Func_02003d30();
void Func_02003d40();
s32 Func_02003d5e();
void Func_02003d60();
s32 Func_02003d68();
void Func_02003d70();
void Func_02003d74();
void Func_02003d82();
void Func_02003d94();
void Func_02003da4();
void Func_02003da8();
void Func_02003db0();
void Func_02003db8();
void Func_02003dbe();
void Func_02003dc2();
void Func_02003dc6();
void Func_02003dda();
void Func_02003de4();
void Func_02003dec();
void Func_02003e04();
void Func_02003e08();
void Func_02003e0a();
void Func_02003e32();
void Func_02003e3e();
void Func_02003e4a();
void Func_02003e4e();
void Func_02003e5a();
void Func_02003e66();
void Func_02003e72();
void Func_02003e7e();
void Func_02003e86();
void Func_02003e8a();
s32 Func_02003e8c();
void Func_02003e8e();
void Func_02003ea0();
void Func_02003ea2();
s32 Func_02003eb0();
void Func_02003eb8();
void Func_02003eca();
void Func_02003ed4();
void Func_02003ed6();
void Func_02003ede();
void Func_02003eec();
void Func_02003ef2();
void Func_02003efe();
void Func_02003f0a();
void Func_02003f10();
void Func_02003f16();
void Func_02003f2c();
void Func_02003f38();
void Func_02003f44();
void Func_02003f50();
void Func_02003f54();
void Func_02003f62();
void Func_02003f74();
void Func_02003f92();
void Func_02003fa2();
void Func_02003fb6();
void Func_02003fbc();
void Func_02003fcc();
void Func_02003fd2();
void Func_02003fd4();
void Func_02003fe6();
void Func_02003ffe();
void Func_02004002();
void Func_02004004();
void Func_02004008();
void Func_0200400c();
void Func_02004016();
void Func_0200401e();
void Func_02004020();
void Func_02004032();
void Func_0200403e();
void Func_02004046();
void Func_0200404e();
void Func_02004050();
void Func_02004078();
void Func_0200407a();
void Func_02004080();
void Func_0200409e();
void Func_020040a0();
void Func_020040a4();
void Func_020040ae();
void Func_020040b2();
void Func_020040c8();
void Func_020040d4();
void Func_020040da();
void Func_020040de();
void Func_020040f6();
void Func_020040f8();
void Func_0200410a();
s32 Func_02004110();
void Func_0200411a();
void Func_0200411c();
void Func_02004126();
void Func_02004128();
s32 Func_02004130();
void Func_02004136();
void Func_02004144();
void Func_02004148();
void Func_02004150();
void Func_0200415c();
void Func_02004166();
void Func_02004168();
void Func_0200416c();
void Func_02004172();
void Func_02004174();
void Func_02004176();
void Func_020041a2();
void Func_020041a8();
void Func_020041be();

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
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

void FieldScene_RunThreeActorChoreography(void)
{
    u32 i;
    s32 record;
    u8 *work;

    Func_02003c1c();
    Call3(Func_02003d18, 3, 0xa000, 0);
    Call3(Func_02003c52, 0, 0x9999, 0x4ccc);
    Value3(Func_02003c94, 0, 0x2b2, 200);
    *(u8 *)(Func_02003d68() + 85) = 0;
    Value2(Func_02003d5e, 0xcccc, 0x1999);
    Call4(Func_02003d74, 0x2b20000, 0, 0xa40000, 1);
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)((work + 0x1c0)) = 0x100;
    *(volatile s32 *)((work + 0x1c8)) = 48;
    Func_02003dc6();
    Func_02003cf4(0);
    Func_02003d0c(0, 1);
    Call3(Func_02003cae, 3, 0x9999, 0x4ccc);
    record = Value1(Func_02003cac, 0);
    if (record != 0) {
        Func_02003d22(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Call3(Func_02003d0c, 3, 0x2a1, 183);
    Call3(Func_02003da8, 3, 0xc000, 0);
    Func_02003d60(19, 2);
    Func_02003d70(20, 2);
    Func_02003cc6(40);
    Call1(Func_02003da4, 0x165b);
    Func_02003dbe(19, 0, 10);
    Call3(Func_02003dda, 3, 0xe000, 40);
    Func_02003d82(3, 3);
    Call2(Func_02003e04, 20, 0x102);
    Func_02003cfa(20);
    Call3(Func_02003dec, 0x4014, 0, 10);
    Call3(Func_02003e08, 3, 0xa000, 40);
    Func_02003db0(3, 4);
    Call3(Func_02003e0a, 0x2003, 0, 10);
    Func_02003dc2(20, 3);
    Func_02003d30(20);
    Call2(Func_02003e4a, 19, 0x102);
    Func_02003d40(20);
    Func_02003e32(19, 0, 10);
    Call3(Func_02003e4e, 0, 0xa000, 0);
    Call3(Func_02003e5a, 3, 0xf000, 10);
    Call3(Func_02003e66, 3, 0x2000, 40);
    Call3(Func_02003e72, 0, 0xc000, 0);
    Call3(Func_02003e7e, 3, 0xc000, 40);
    Func_02003e3e(20, 2);
    Func_02003d94(20);
    Call3(Func_02003e86, 0x4014, 0, 20);
    Call3(Func_02003ea2, 3, 0xa000, 20);
    Func_02003e4a(3, 3);
    Func_02003db8(60);
    Call3(Func_02003eca, 3, 0x105, 60);
    Call3(Func_02003ed4, 19, 0x101, 0);
    Call3(Func_02003ede, 20, 0x101, 60);
    Func_02003e8e(19, 1);
    Func_02003de4(20);
    Func_02003ed6(19, 0, 10);
    Call3(Func_02003ef2, 3, 0xe000, 40);
    Call3(Func_02003efe, 3, 0xa000, 40);
    Call3(Func_02003f0a, 3, 0xe000, 20);
    Call3(Func_02003f16, 3, 0x6000, 80);
    Call3(Func_02003f10, 0x2003, 0, 20);
    Call3(Func_02003f2c, 20, 0xf000, 0);
    Call3(Func_02003f38, 19, 0x7000, 40);
    Call3(Func_02003f44, 19, 0x5000, 0);
    Call3(Func_02003f50, 20, 0x3000, 20);
    Call3(Func_02003e8e, 20, 0x10000, 0x8000);
    *(u8 *)(Func_02003e8c(20) + 90) &= 254;
    Call3(Func_02003eec, 20, 0x290, 166);
    Func_02003e8a(1);
    *(u8 *)(Func_02003eb0(20) + 90) |= 1;
    Func_02003ea0(20);
    Call3(Func_02003f92, 0x4014, 0, 10);
    Func_02003f62(3, 2);
    Func_02003eb8(40);
    Call3(Func_02003fbc, 3, 0xa000, 10);
    Call3(Func_02003fb6, 0x2003, 0, 40);
    Call3(Func_02003fd2, 3, 0x2000, 20);
    Call3(Func_02003fcc, 0x4003, 0, 10);
    Call2(Func_02003ffe, 19, 0x102);
    Call2(Func_02004008, 20, 0x102);
    Func_02003efe(40);
    Call3(Func_02004002, 3, 0xc000, 20);
    Func_02003fa2(3, 4);
    Call3(Func_02004004, 0x2003, 0, 20);
    Func_02003fd4(19, 1);
    Func_02004016(19, 0, 10);
    Call3(Func_02004032, 3, 0x2000, 40);
    Call3(Func_0200403e, 3, 0xc000, 20);
    Func_02003fe6(3, 3);
    Func_02003f54(20);
    Func_0200400c(20, 1);
    Call3(Func_0200404e, 0x4014, 0, 20);
    Func_0200401e(3, 1);
    Func_02003f74(20);
    Call3(Func_02004078, 3, 0xa000, 20);
    Func_02004020(3, 3);
    Call3(Func_0200407a, 0x2003, 0, 80);
    Call3(Func_020040a4, 19, 0x105, 0);
    Call3(Func_020040ae, 20, 0x105, 60);
    Func_02004046(19, 4);
    Func_020040a0(19, 0, 10);
    Func_02004050(20, 4);
    Call3(Func_020040b2, 0x4014, 0, 20);
    Call3(Func_020040de, 3, 0x102, 60);
    Call3(Func_020040c8, 0x2003, 0, 40);
    Func_02004080(19, 3);
    Func_020040da(19, 0, 10);
    Call3(Func_020040f6, 3, 0xe000, 20);
    Func_0200409e(20, 3);
    Call3(Func_020040f8, 0x4014, 0, 10);
    Call3(Func_02004150, 3, 0xa000, 60);
    Call3(Func_0200415c, 3, 0xe000, 20);
    Call3(Func_02004168, 3, 0xa000, 20);
    Call3(Func_02004174, 3, 0xc000, 40);
    Func_0200411c(3, 3);
    Call3(Func_02004176, 0x2003, 0, 10);
    Func_02004126(19, 3);
    Func_02004136(20, 3);
    Func_020040a4(40);
    Call3(Func_020041a8, 3, 0x2000, 20);
    Call3(Func_020041a2, 0x4003, 0, 20);
    Call3(Func_020041be, 0, 0xa000, 20);
    Func_02004166(0, 3);
    Func_020040d4(20);
    Call3(Func_02004148, 3, 0x2b0, 200);
    Func_02004172(3, 0, 0);
    *(u8 *)(Func_02004110(20) + 90) &= 254;
    Call3(Func_0200416c, 20, 0x284, 166);
    Func_0200410a(1);
    *(u8 *)(Func_02004130(20) + 90) |= 1;
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x209;
    Call1(Func_0200411a, 0x82e);
    Call1(Func_02004128, 0x82d);
    Func_02004144();
}
