#include "types.h"
#include "configured_effect_spawn.h"

#define Lifted_02004610 Func_02004610


/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02004814();
s32 Func_02009442();
s32 Func_02009456();
s32 Func_0200946a();
void Func_02009480();
s32 Func_02009488();
void Func_020094b2();
void Func_020094c4();
void Func_020094dc();
s32 Func_020094e2();
void Func_020094e4();
void Func_02009562();
void Func_02009578();
void Func_02009586();
void Func_020095e6();
void Func_02009600();
void Func_02009608();
void Func_02009612_setup();
void Func_02009626();
void Func_02009632();
void Func_02009636();
void Func_0200965a();
void Func_020096fa();
void Func_02009704();
void Func_02009736();

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
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

void Lifted_02004610(s32 a0, s32 a1)
{
    struct ConfiguredEffectOptions params;
    u32 i;
    s32 p10;
    s32 p11;
    s32 p8;
    s32 p9;
    s32 record;
    s32 value;
    s32 none;
    s32 v8;
    s32 v9;
    s32 v10;
    s32 v5;
    s32 base6_0;
    s32 v6;
    s32 v7;

    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x202;
    Func_020094c4();
    record = Func_020094e2(0);
    Func_02009480(record, 0);
    Func_02009578(0, 15);
    Func_02009636(170);
    Func_02009612_setup();
    Func_02009626();
    Func_020094e4(40);
    Func_0200965a(162);
    Call2(Func_020095e6, 0x8000, 0x1000);
    Call4(Func_02009600, 0x1b80000, -1, 0x1680000, 1);
    none = 0;
    p8 = none;
    v8 = p8;
    v9 = p8;
    v10 = none;
    do {
        value = Func_02009442();
        params.accum18 = (((u32)(value << 1) >> 16) * 0x4ccc) + 0x17ffc;
        value = Func_02009456();
        params.accum1c = (((u32)(value << 1) >> 16) * 0x4ccc) + 0x17ffc;
        value = Func_0200946a();
        params.step = (((u32)(value << 12) >> 16) + 0xf800);
        L_020046c8:;
        base6_0 = 0;
        v7 = 0;
        v5 = (0xc00000 + v9);
        do {
            value = Value0(Func_02009488);
            Call8(Func_02004814, ((((u32)((value << 3) - value) >> 16) << 19) + 0x1a00000), 0, v5, 0, 0, 0, 0x880000, (s32)&params);
            base6_0 = (base6_0 + 1);
            v5 = (v5 + 0x40000);
        } while ((u32)base6_0 <= 3);
        Func_020094b2(3);
        if (v8 == 3) {
            if ((u32)v10 <= 2) {
                v10 = (v10 + 1);
                goto L_020046c8;
            }
            if (v10 == 3) {
                Call2(Func_020094dc, 0x200c5f1, 0xc80);
            }
        }
        Call6(Func_02009562, 53, (v8 + 12), 26, (v8 + 12), 3, 1);
        v8 = (v8 + 1);
        v9 = (v9 + 0x100000);
        v6 = base6_0;
    } while ((u32)v8 <= 12);
    Call6(Func_02009586, 81, 41, 89, 14, 9, 2);
    Func_020096fa();
    Call4(Func_02009704, -1, -1, -1, 0);
    Func_020094e4(60);
    Call1(Func_02009608, 0x306);
    Func_02009736(19);
    Func_02009632();
    p9 = v5;
    p10 = v6;
    p11 = v7;
}
