#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02007056();
void Func_0200709e();
s32 Func_02007386();
s32 Func_0200738a();
s32 Func_02007392();
s32 Func_0200739e();
s32 Func_020073a4();
s32 Func_020073b0();
void Func_020073c4();
s32 Func_020073cc();
s32 Func_020073ea();
s32 Func_020073ec();
s32 Func_020073f8();
s32 Func_020073fc();
void Func_0200740c();
void Func_0200744c();
void Func_0200745c();
void Func_0200746e();
void Func_02007540();
void Func_0200755e();
void Func_02007566();
void Func_02007588();
void Func_020075a6();

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

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}


void Func_02003864(s32 a0, s32 a1, s32 a2)
{
    s32 p8;
    u8 *rec7;
    s32 value;
    s32 mask;
    u8 *link;

    p8 = a2;
    value = Value0(Func_0200739e);
    rec7 = (u8 *)Value4(Func_020073fc, 222, (a0 + -0x80000), (((((u32)(value << 3) >> 16) << 16) + a1) + 0x100000), p8);
    if ((s32)rec7 != 0) {
        rec7[85] = (mask = 0);
        link = (u8 *)*(volatile s32 *)((s32)rec7 + 80);
        mask -= 13;
        link[9] = ((link[9] & mask) | 8);
        Func_02007566((s32)rec7, 9);
        Func_0200746e((s32)rec7, 0);
        value = Func_020073ea();
        *(volatile s32 *)((s32)rec7 + 36) = ((((u32)(value << 1) >> 16) - 1) << 16);
        value = Value0(Func_020073f8);
        *(volatile s32 *)((s32)rec7 + 40) = ((((u32)(((value << 1) + value) << 1) >> 16) - 3) << 16);
        {
            volatile u16 *target = (volatile u16 *)((s32)rec7 + 100);
            s32 shown = 20;

            *target = shown;
            *((u8 *)target - 3) = 1;
        }
        Func_0200744c((s32)rec7, 1);
        Func_0200745c((s32)rec7, 0x200d120);
    }
}
