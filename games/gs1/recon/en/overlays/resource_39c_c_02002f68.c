#include "types.h"

struct EffectSpawnOptions {
    s32 mode_bits;
    s32 battle_mode;
    s32 position_x;
    s32 position_y;
    s32 target30;
    s32 target34;
    s16 kind;
    u16 reserved1a;
    s32 callback_arg;
    u16 angle;
    u16 step;
    u32 callback;
};

void Func_0200dab4();
void Func_0200daac();
void Func_0200ae08();
void Func_0200d98c();
void Func_0200da04();
void Func_0200dc64();
u32 Func_0200d994(void);
void Func_0200813c(s32, s32, s32, s32, s32, s32, s32, void *);
void Func_0200da3c();
void Func_0200da44();
void Func_0200dc5c();
s32 Func_0200da84(s32);
void Func_0200dadc();
void Func_0200db0c();
void Func_0200db3c();
void Func_0200db04();
void Func_0200db2c();
void Func_0200ae6c();
void Func_0200dbe4();
void Func_0200dbdc();
void Func_0200dbec();
void Func_0200dbcc();
void Func_0200dabc();
void Func_0200db94();
void Func_0200db24();
void Func_0200db34();
void Func_0200db4c();
void Func_0200db1c();
void Func_0200dba4();
void Func_0200db44();
void Func_0200db64();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, void *a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

void Func_02002f68(void)
{
    struct EffectSpawnOptions *p10;
    s32 p9;
    u32 value;
    s32 none;
    s32 v8;
    s32 base7_0;
    s32 v5;
    s32 v6;
    u32 x, z;
    s32 speed;
    struct EffectSpawnOptions options;

    Func_0200dab4();
    Func_0200daac(20);
    Func_0200ae08();
    Call1(Func_0200d98c, 0x200adcd);
    Call6(Func_0200da04, 45, 77, 45, 73, 9, 4);
    Func_0200daac(30);
    p10 = &options;
    p10->mode_bits = 1;
    p10->battle_mode = 5;
    p10->kind = 0x11e;
    p10->callback_arg = 0x200de70;
    none = 0;
    v8 = none;
    do {
        if ((1 & v8) != 0) {
            Func_0200dc64(246);
        }
        value = Func_0200d994();
        x = value * 48;
        x >>= 16;
        x <<= 16;
        x += 0x3000000;
        value = Func_0200d994();
        z = value * 56;
        z >>= 16;
        z <<= 16;
        z += 0x880000;
        Func_0200813c(x, 0, z, 0, 0, 0, 0x330001, p10);
        Call1((void (*)())Func_0200daac, 2);
        v8 = (v8 + 1);
    } while ((u32)v8 <= 15);
    Func_0200daac(40);
    none = 0;
    v8 = none;
    do {
        if ((1 & v8) != 0) {
            Func_0200dc64(246);
        }
        value = Func_0200d994();
        x = value * 48;
        x >>= 16;
        x <<= 16;
        x += 0x3000000;
        value = Func_0200d994();
        z = value * 56;
        z >>= 16;
        z <<= 16;
        z += 0x980000;
        value = Value0(Func_0200d994);
        speed = -((value * 10 >> 16) * 0x3333) - 0x3333;
        Func_0200813c(x, 0, z, 0, 0, speed, 0x330001, p10);
        Func_0200daac(2);
        v8 = (v8 + 1);
    } while ((u32)v8 <= 15);
    Func_0200daac(60);
    Func_0200dc64(141);
    Call3(Func_0200da3c, 0x50000, 0x50000, 0x10000);
    Func_0200daac(60);
    p10->battle_mode = 7;
    p10->position_x = 0xb333;
    p10->position_y = 0xb333;
    p10->target30 = 0x13333;
    p10->target34 = 0x13333;
    none = 0;
    v8 = none;
    do {
        Func_0200da04(59, (12 - v8), 48, (12 - v8), 3, 1);
        base7_0 = 0;
        p9 = (v8 << 4);
        do {
            value = Value0(Func_0200d994);
            v5 = ((((u32)(((value << 1) + value) << 4) >> 16) << 16) + 0x3000000);
            value = Func_0200d994();
            v6 = ((0x1999 * ((u32)(value << 3) >> 16)) + -0x6664);
            value = Func_0200d994();
            value = value * 8 >> 16;
            speed = 0x1999 * value;
            Func_0200813c(v5, 0, ((s32)(-((u32)base7_0 >> 1) - p9) << 16) + 0xc00000, v6, 0, speed, 0xd0001, p10);
            base7_0 = (base7_0 + 1);
            Func_0200daac(2);
        } while ((u32)base7_0 <= 31);
        v8 = (v8 + 1);
    } while ((u32)v8 <= 3);
    Call1(Func_0200dc64, 0x121);
    Call3(Func_0200da3c, -1, -1, 0xe666);
    Func_0200da44();
    Func_0200dc5c();
    Func_0200daac(30);
    if (Value1(Func_0200da84, 0x881) != 0) {
        Call3(Func_0200dadc, 0, 0xcccc, 0x6666);
        Call3(Func_0200db0c, 0, 0x338, 232);
        Call3(Func_0200db0c, 0, 0x318, 232);
        Call3(Func_0200db0c, 0, 0x318, 208);
        Call3(Func_0200dadc, 0, 0x20000, 0x10000);
        Func_0200db3c(0, 4, 0);
        Call3(Func_0200db04, 0, 0x318, 200);
        Func_0200daac(10);
        Func_0200db2c(0, 18);
        Func_0200ae6c(0);
        Func_0200daac(60);
        Call1(Func_0200d98c, 0x200adcd);
        Call2(Func_0200dbe4, 0x10000, 0);
        Call2(Func_0200dbdc, 0x10005, 0);
        Func_0200dbec(120);
        Func_0200daac(120);
        Call2(Func_0200dbdc, 0x7fff, 0);
        Func_0200dbec(60);
        Func_0200daac(60);
        Func_0200dbcc(9);
        Func_0200dabc();
    } else {
        Call3(Func_0200dadc, 0, 0xcccc, 0x6666);
        Call3(Func_0200dadc, 1, 0xcccc, 0x6666);
        Call3(Func_0200dadc, 2, 0xcccc, 0x6666);
        Call3(Func_0200dadc, 3, 0xcccc, 0x6666);
        Call3(Func_0200db0c, 0, 0x338, 240);
        Call3(Func_0200db94, 0, 0xa000, 20);
        Call3(Func_0200db24, 3, 0x3380000, 0xf00000);
        Call3(Func_0200db0c, 3, 0x318, 232);
        Call3(Func_0200db94, 3, 0xc000, 0);
        Func_0200daac(60);
        Call3(Func_0200db94, 3, 0x2000, 20);
        Func_0200db34(3, 3);
        Func_0200daac(40);
        Call3(Func_0200db94, 3, 0xc000, 20);
        Call3(Func_0200db0c, 3, 0x318, 200);
        Func_0200ae6c(3);
        Func_0200daac(20);
        Func_0200db4c(0, 2);
        Func_0200daac(30);
        Call3(Func_0200db0c, 0, 0x318, 232);
        Call3(Func_0200db94, 0, 0xc000, 0);
        Call3(Func_0200db24, 1, 0x3180000, 0xe80000);
        Call3(Func_0200db24, 2, 0x3180000, 0xe80000);
        Call3(Func_0200db04, 1, 0x330, 224);
        Call3(Func_0200db0c, 2, 0x300, 224);
        Func_0200db1c(1);
        Call3(Func_0200db94, 1, 0xa000, 0);
        Call3(Func_0200db94, 2, 0xe000, 20);
        Call3(Func_0200dba4, 0, 0x102, 0);
        Call3(Func_0200dba4, 1, 0x102, 0);
        Call3(Func_0200dba4, 2, 0x102, 80);
        Call3(Func_0200db94, 1, 0x6000, 0);
        Call3(Func_0200db94, 2, 0x2000, 20);
        Func_0200db4c(0, 1);
        Func_0200daac(60);
        Call3(Func_0200dadc, 0, 0x8000, 0x4000);
        Call3(Func_0200db0c, 0, 0x318, 224);
        Call3(Func_0200db94, 1, 0x8000, 0);
        Func_0200db94(2, 0, 0);
        Call3(Func_0200db0c, 0, 0x318, 208);
        Call3(Func_0200db94, 1, 0xa000, 0);
        Call3(Func_0200db94, 2, 0xe000, 20);
        Func_0200db34(0, 3);
        Func_0200daac(20);
        Call3(Func_0200dadc, 0, 0x20000, 0x10000);
        Func_0200db3c(0, 4, 0);
        Call3(Func_0200db04, 0, 0x318, 200);
        Func_0200daac(10);
        Func_0200db2c(0, 18);
        Call3(Func_0200dba4, 1, 0x100, 0);
        Call3(Func_0200dba4, 2, 0x100, 0);
        Func_0200db44(1, 2);
        Func_0200db44(2, 2);
        Func_0200ae6c(0);
        Func_0200daac(60);
        Call3(Func_0200dba4, 1, 0x102, 0);
        Call3(Func_0200dba4, 2, 0x102, 80);
        Func_0200db64(1, 2, 20);
        Func_0200db2c(1, 3);
        Func_0200db34(2, 3);
        Func_0200daac(40);
        Call3(Func_0200db0c, 1, 0x318, 216);
        Call3(Func_0200db94, 2, 0xe000, 0);
        Call3(Func_0200db0c, 1, 0x318, 200);
        Func_0200daac(30);
        Func_0200ae6c(1);
        Call3(Func_0200db0c, 2, 0x318, 216);
        Call3(Func_0200db0c, 2, 0x318, 200);
        Func_0200daac(30);
        Func_0200ae6c(2);
        Call2(Func_0200dbe4, 0x10000, 0);
        Call1(Func_0200d98c, 0x200adcd);
        Call2(Func_0200dbdc, 0x10005, 0);
        Func_0200dbec(120);
        Func_0200daac(120);
        Call2(Func_0200dbdc, 0x7fff, 0);
        Func_0200dbec(60);
        Func_0200daac(60);
        Func_0200dabc();
        Func_0200dbcc(8);
    }
}
