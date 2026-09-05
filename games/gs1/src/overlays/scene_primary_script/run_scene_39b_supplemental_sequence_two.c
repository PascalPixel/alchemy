#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020011e8();
s32 Func_0200345a();
s32 Func_02003466();
s32 Func_0200346c();
s32 Func_02003474();
s32 Func_020034cc();
s32 Func_020034d2();
s32 Func_020034e4();
void Func_020034f0();
void Func_020034fe();
void Func_02003512();
void Func_02003560();
void Func_0200357c();
void Func_02003586();
void Func_02003592();
void Func_0200359c();
void Func_020035da();
void Func_020035ea();
void Func_020035ee();
void Func_020035f0();
void Func_020035fe();
void Func_02003604();
void Func_0200366e();
void Func_020036c6();
void Func_020036d6();
void Func_020036dc();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunSupplementalSequenceTwo(void)
{
    s32 a;
    s32 b;
    s32 zero;
    s32 counter;
    s32 x;
    s32 y;
    s32 t;
    s32 record;
    u8 *slot;
    u8 slot16[40];

    a = *(volatile s32 *)(Value1(Func_020034d2, 0) + 8) / 0x100000;
    b = *(volatile s32 *)(Value1(Func_020034e4, 0) + 16) / 0x100000;
    if (a == 12 && b == 32) {
        Func_020034f0();
        Call2(Func_020035da, 0x10000, 0);
        Func_020035f0(60);
        Func_020034fe(120);
        Call2(Func_020035ee, 0x10005, 1);
        Func_02003604(60);
        Func_02003512(40);
        counter = 0;
        slot = slot16;
        zero = 0;
        do {
            *(s32 *)(slot) = 1;
            {
                s32 shown = 0x11e;

                *(u16 *)(slot + 24) = shown;
            }
            *(s32 *)(slot + 28) = 0x200af4c;
            Func_0200366e(246);
            x = 208 - ((u32)(Func_0200345a() << 4) >> 16);
            y = 560 - ((u32)(Func_02003466() << 4) >> 16);
            t = ((u32)(Value0(Func_02003474) << 2) >> 16);
            record = Func_0200346c((((t << 4) - t) << 16) + 0x3c0000, 100);
            Func_020011e8(x << 16, 0, y << 16, 0, record, zero, 0x320001, slot);
            Func_02003586(4);
            counter = counter + 1;
        } while ((u32)counter <= 14);
        Func_020036d6(220);
        Func_0200359c(60);
        Call1(Func_02003592, 0x875);
        Value2(Func_020034cc, 0x2008d99, 0xc80);
        Call6(Func_02003560, 37, 98, 10, 97, 5, 3);
        Call6(Func_0200357c, 70, 32, 13, 7, 6, 32);
        Call2(Func_020036c6, 0x10000, 0);
        Func_020036dc(60);
        Func_020035ea(120);
        Func_020035fe();
    }
}
