#include "types.h"

#define FieldScene_RunFourActorPresentation Func_0200056c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02003bec();
void Func_02003bf6();
void Func_02003c10();
void Func_02003c1c();
void Func_02003c28();
void Func_02003c34();
void Func_02003c38();
void Func_02003c3c();
void Func_02003c48();
u8 *Func_02003c6a();
u8 *Func_02003c76();
void Func_02003c7c();
u8 *Func_02003c82();
u8 *Func_02003c8e();
void Func_02003c90();
void Func_02003cb2();
void Func_02003ce0();
void Func_02003d24();
void Func_02003d38();
void Func_02003d66();
void Func_02003d7e();
void Func_02003d96();
void Func_02003d9e();
void Func_02003dac();
void Func_02003db2();
void Func_02003dba();
void Func_02003dc8();
void Func_02003dca();
void Func_02003dcc();
s32 Func_02003dd4();
u8 *Func_02003dd8();
void Func_02003de0();
void Func_02003de2();
void Func_02003de8();
void Func_02003dee();
void Func_02003df0();
void Func_02003dfa();
void Func_02003e02();
void Func_02003e06();
void Func_02003e08();
void Func_02003e0c();
u8 *Func_02003e14();
void Func_02003e1a();
void Func_02003e22();
void Func_02003e26();
void Func_02003e28();
void Func_02003e2c();
void Func_02003e3c();
void Func_02003e3e();
void Func_02003e52();
void Func_02003e58();
u8 *Func_02003e5c();
void Func_02003e62();
void Func_02003e76();
u8 *Func_02003e82();
void Func_02003e84();
void Func_02003e98();
void Func_02003eaa();
void Func_02003eb0();
void Func_02003eb6();
void Func_02003ec0();
void Func_02003ec6();
void Func_02003eca();
void Func_02003ecc();
void Func_02003ed0();
void Func_02003ede();
u8 *Func_02003ee0();
void Func_02003ee6();
void Func_02003eee();
u8 *Func_02003ef8();
void Func_02003f02();
void Func_02003f0a();
void Func_02003f0e();
u8 *Func_02003f10();
void Func_02003f12();
void Func_02003f1a();
void Func_02003f32();
void Func_02003f46();
void Func_02003f58();
void Func_02003f68();
void Func_02003f6a();
void Func_02003f78();
void Func_02003f7a();
void Func_02003f8a();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunFourActorPresentation(void)
{
    u32 i;
    u8 *record;

    Func_02003c48();
    Func_02003c3c();
    record = Func_02003c6a(0);
    Func_02003c10(record, 0);
    record = Func_02003c76(1);
    Func_02003c1c(record, 0);
    record = Func_02003c82(2);
    Func_02003c28(record, 0);
    record = Func_02003c8e(3);
    Func_02003c34(record, 0);
    Call4(Func_02003d66, 0x1300000, -1, 0x780000, 0);
    Func_02003bec(1);
    Func_02003c38();
    Func_02003bf6(1);
    Func_02003e0c(141);
    Call3(Func_02003c7c, 0x50000, 0x50000, 0x10000);
    Call1(Func_02003e22, 0x121);
    Call3(Func_02003c90, -1, -1, 0xe666);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x100;
    Func_02003dfa();
    Func_02003e06();
    Func_02003cb2();
    Func_02003ce0(30);
    *(u8 *)(Func_02003dd4() + 85) = 0;
    Call2(Func_02003dca, 0xcccc, 0x1999);
    Call4(Func_02003de2, 0x2000000, -0x180000, 0xa00000, 1);
    Func_02003dee();
    Call2(Func_02003e28, 0x10000, 0);
    Call2(Func_02003e28, 0x10005, 0);
    Func_02003e3e(50);
    Func_02003d24(50);
    Call2(Func_02003e3c, 0x7fff, 0);
    Func_02003e52(30);
    Func_02003d38(30);
    Call3(Func_02003d9e, 0, 0x1f80000, 0xa80000);
    Call3(Func_02003dac, 1, 0x2100000, 0x900000);
    Call3(Func_02003dba, 2, 0x1e80000, 0x900000);
    Call3(Func_02003dc8, 3, 0x2000000, 0x980000);
    Call2((void (*)())Func_02003dd8, 0, 19);
    Func_02003de0(1, 19);
    Func_02003de8(2, 19);
    Func_02003df0(3, 19);
    Func_02003d96(10);
    Call2(Func_02003eb0, 0x10000, 0);
    Func_02003ec6(30);
    Func_02003dac(30);
    Func_02003db2(80);
    record = Func_02003dd8(0);
    Func_02003d7e(record, 1);
    Func_02003e26(0, 1);
    Func_02003dcc(30);
    Func_02003e3c(0, 4);
    Call3(Func_02003e98, 0, 0xc000, 20);
    Func_02003e58(0, 2);
    Func_02003dee(60);
    record = Func_02003e14(1);
    Func_02003dba(record, 1);
    Func_02003e62(1, 1);
    Func_02003e08(20);
    Call3(Func_02003ecc, 1, 0x2000, 0);
    Func_02003e1a(20);
    Call3(Func_02003ede, 1, 0x6000, 0);
    Func_02003e2c(20);
    Func_02003eee(1, 0, 0);
    record = Func_02003e5c(2);
    Func_02003e02(record, 1);
    Func_02003eaa(2, 1);
    Call3(Func_02003f0e, 1, 0x6000, 0);
    Func_02003e5c(40);
    record = Func_02003e82(3);
    Func_02003e28(record, 1);
    Func_02003ed0(3, 1);
    Func_02003e76(20);
    Func_02003ee6(3, 3);
    Func_02003e84(20);
    Call3(Func_02003eb6, 1, 0xcccc, 0x6666);
    Call3(Func_02003ec0, 2, 0xcccc, 0x6666);
    Call3(Func_02003eca, 3, 0xcccc, 0x6666);
    Func_02003f0a(1, 2);
    Func_02003f12(2, 2);
    Func_02003f1a(3, 2);
    record = Func_02003ee0(0);
    if (record != 0) {
        Func_02003f02(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    record = Func_02003ef8(0);
    if (record != 0) {
        Func_02003f1a(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    record = Func_02003f10(0);
    if (record != 0) {
        Func_02003f32(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02003f58(3);
    Func_02003f6a(3, 0, 0);
    Func_02003f68(2);
    Func_02003f7a(2, 0, 0);
    Func_02003f78(1);
    Func_02003f8a(1, 0, 0);
    Func_02003f46();
}
