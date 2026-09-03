#include "types.h"

#define RunEventScript01 Func_02003028

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200c0e4[];
extern u8 Data_0200c12c[];
extern u8 Data_03001ebc[];
void Func_02006b9c();
void Func_02006bdc();
s32 Func_02006c12();
void Func_02006c22();
s32 Func_02006c26();
void Func_02006c30();
s32 Func_02006c3a();
void Func_02006c40();
void Func_02006c5a();
void Func_02006c64();
void Func_02006c6e();
void Func_02006c70();
s32 Func_02006c7e();
void Func_02006c84();
s32 Func_02006c86();
void Func_02006c98();
s32 Func_02006c9e();
s32 Func_02006cee();
void Func_02006d1a();
void Func_02006d22();
void Func_02006d26();
void Func_02006d34();
void Func_02006d42();
void Func_02006d46();
void Func_02006d4e();
void Func_02006d54();
void Func_02006d5a();
void Func_02006d62();
void Func_02006d6c();
void Func_02006d6e();
void Func_02006d76();
void Func_02006d84();
void Func_02006d8c();
s32 Func_02006d8e();
void Func_02006d92();
void Func_02006d94();
void Func_02006d9e();
void Func_02006da4();
void Func_02006dca();
void Func_02006dd4();
void Func_02006df0();
void Func_02006e08();
void Func_02006e24();
void Func_02006e34();
void Func_02006e3a();
void Func_02006e3c();
void Func_02006e40();
void Func_02006e48();
void Func_02006e4e();
void Func_02006e50();
void Func_02006e66();
void Func_02006e6a();
void Func_02006e74();
void Func_02006e7e();
void Func_02006e86();
void Func_02006e88();
void Func_02006e9e();
void Func_02006eaa();
void Func_02006eb6();
void Func_02006ec2();
void Func_02006ede();
void Func_02006ee8();
void Func_02006eea();
void Func_02006ef6();
void Func_02006ef8();
void Func_02006f00();
void Func_02006f02();
void Func_02006f08();
void Func_02006f10();
void Func_02006f14();
void Func_02006f1e();
void Func_02006f28();
void Func_02006f32();
void Func_02006f36();
s32 Func_02006f3a();
void Func_02006f42();
void Func_02006f44();
void Func_02006f48();
void Func_02006f4e();
void Func_02006f50();
void Func_02006f52();
void Func_02006f5a();
void Func_02006f62();
void Func_02006f72();
void Func_02006f78();
void Func_02006f8c();
void Func_02006f98();
void Func_02006fa2();
void Func_02006fc8();
void Func_0200affd();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void RunEventScript01(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 base6_200affd;
    s32 base5_200c0e4;
    s32 base5_200c12c;

    Func_02006b9c();
    Call3(Func_02006c22, 8, 0x1480000, 0x580000);
    Call3(Func_02006c30, 9, 0x1480000, 0x580000);
    Func_02006c40(8, 0);
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)((work + 0x1c0)) = 0x100;
    *(volatile s32 *)((work + 0x1c8)) = 40;
    Func_02006d1a();
    Func_02006d26();
    Func_02006bdc(20);
    record = Value1(Func_02006c12, 0);
    if (record != 0) {
        Func_02006c70(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1(Func_02006c26, 0);
    if (record != 0) {
        Func_02006c84(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1(Func_02006c3a, 0);
    if (record != 0) {
        Func_02006c98(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Call3(Func_02006c5a, 1, 0x9999, 0x4ccc);
    Call3(Func_02006c64, 2, 0x9999, 0x4ccc);
    Call3(Func_02006c6e, 3, 0x9999, 0x4ccc);
    Value2(Func_02006c7e, 1, 0x200c054);
    Value2(Func_02006c86, 2, 0x200c084);
    Value2(Func_02006c9e, 3, 0x200c0b4);
    Call3(Func_02006d42, 1, 0xc000, 0);
    Call3(Func_02006d4e, 2, 0xc000, 0);
    Call3(Func_02006d5a, 3, 0xc000, 40);
    Func_02006d22(1, 1);
    Call3(Func_02006d6e, 1, 0xe000, 10);
    Call1(Func_02006d54, 0x190c);
    Func_02006d76(1, 0, 10);
    Func_02006d46(2, 1);
    Call3(Func_02006d92, 2, 0xa000, 10);
    Call3(Func_02006d9e, 0, 0x2000, 0);
    Value2(Func_02006d8e, 2, 0);
    if (Value2(Func_02006cee, 0, 0) == 0) {
        Func_02006d62(2, 3);
    } else {
        Func_02006d6c(2, 4);
        bump_step(1);
    }
    Func_02006dd4(2, 0, 20);
    Func_02006d84(0, 3);
    Func_02006d8c(1, 3);
    Func_02006d94(0, 3);
    Func_02006da4(0, 3);
    Func_02006d1a(20);
    Func_02006dca(3, 2);
    Call1(Func_02006df0, 0x1910);
    Func_02006e08(3, 0);
    Call3(Func_02006e24, 0, 0x4000, 10);
    Call3(Func_02006e40, 1, 0x102, 60);
    Call3(Func_02006e3c, 1, 0x2000, 40);
    Func_02006e34(1, 0);
    Call3(Func_02006e50, 0, 0x6000, 10);
    Call3(Func_02006e6a, 0, 0x101, 0);
    Call3(Func_02006e74, 1, 0x101, 0);
    Call3(Func_02006e7e, 2, 0x101, 0);
    Call3(Func_02006e88, 3, 0x101, 40);
    Func_02006f4e(190);
    Func_02006e4e(8, 7);
    Func_02006da4(10);
    Call1(Func_02006f62, 0x121);
    Call3(Func_02006e9e, 0, 0xc000, 0);
    Call3(Func_02006eaa, 1, 0xc000, 0);
    Call3(Func_02006eb6, 2, 0xc000, 0);
    Call3(Func_02006ec2, 3, 0xc000, 0);
    Call3(Func_02006ede, 0, 0x100, 0);
    Call3(Func_02006eea, 1, 0x100, 0);
    Call3(Func_02006ef6, 2, 0x100, 0);
    Call3(Func_02006f02, 3, 0x100, 40);
    Func_02006fc8(103);
    base6_200affd = (s32)Func_0200affd;
    Call2(Func_02006d34, base6_200affd, 0xc80);
    base5_200c0e4 = (s32)Data_0200c0e4;
    Func_02006e66(9, base5_200c0e4);
    Func_02006e7e(8, base5_200c0e4);
    Func_02006d54(base6_200affd);
    Func_02006e3a(60);
    Func_02006eea(2, 2);
    Func_02006e48(20);
    Func_02006f28(2, 0);
    Call3(Func_02006f44, 1, 0xe000, 0);
    Call3(Func_02006f50, 2, 0xa000, 20);
    Func_02006ef8(1, 3);
    Func_02006f00(2, 3);
    Func_02006f10(3, 3);
    Func_02006e86(20);
    Func_02006f36(3, 1);
    Func_02006f78(3, 0, 20);
    Call3(Func_02006f8c, 1, 0xe000, 0);
    Call3(Func_02006f98, 0, 0x6000, 40);
    Func_02006f48(1, 3);
    Func_02006fa2(1, 0, 10);
    Func_02006f5a(0, 3);
    Func_02006f5a(1, 3);
    Func_02006f62(2, 3);
    Func_02006f72(3, 3);
    Func_02006ee8(20);
    base5_200c12c = (s32)Data_0200c12c;
    Func_02006f32(1, base5_200c12c);
    Value2(Func_02006f3a, 2, base5_200c12c);
    Func_02006f52(3, base5_200c12c);
    Func_02006f08(20);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x204;
    Call1(Func_02006f14, 0x12f);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 16;
    Call1(Func_02006f1e, 0x909);
    Func_02006f42();
}
