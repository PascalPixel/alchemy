#include "types.h"

#define FieldScene_RunScene373SequenceE Func_020010d8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02007076();
s32 Func_02007082();
void Func_020070a6();
void Func_020070b0();
void Func_020070c4();
void Func_020070ee();
void Func_020070f0();
void Func_02007112();
s32 Func_02007114();
void Func_02007120();
void Func_0200712e();
void Func_02007142();
void Func_0200714e();
void Func_02007156();
void Func_02007174();
void Func_0200719a();
void Func_020071bc();
void Func_020071c6();
void Func_020071d0();

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

void FieldScene_RunScene373SequenceE(void)
{
    u32 i;
    s32 pos[3];
    s32 **slot;
    s32 *saved;
    s32 record;

    if (Value1(Func_02007082, 0x808) == 0) {
        slot = *(s32 ***)0x03001e70;
        Func_020070b0();
        Call3(Func_020070ee, 0, 0x10000, 0x8000);
        Func_0200714e(0, 1);
        Func_020070c4(2);
        Call1(Func_0200719a, 0xf4d);
        Func_020071bc(15, 0, 2);
        Func_020071c6(16, 0, 2);
        record = Value1(Func_02007114, 0);
        pos[0] = *(s32 *)(record + 8);
        pos[1] = *(s32 *)(record + 12);
        pos[2] = *(s32 *)(record + 16);
        saved = *slot;
        *slot = pos;
        for (i = 0; i != 40; i++) {
            pos[2] += 0x20000;
            Func_02007112(1);
            Func_02007076();
        }
        Func_02007120(60);
        Call2(Func_020070f0, 0xf4f, 1);
        Func_0200712e(6);
        for (i = 0; i != 40; i++) {
            pos[2] += -0x20000;
            Func_02007142(1);
            Func_020070a6();
        }
        *slot = saved;
        Func_02007156(60);
        Call3(Func_020071d0, 0, 70, 0x2e5);
        Func_02007174();
    }
}
