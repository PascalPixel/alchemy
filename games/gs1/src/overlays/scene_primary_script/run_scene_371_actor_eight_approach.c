#include "types.h"

#define FieldScene_RunActorEightApproach Func_020032f0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02007490();
void Func_020074b6();
void Func_020074d6();
void Func_020075d0();
s32 Func_02007622();
void Func_02007678();
void Func_0200767a();
void Func_0200767c();
void Func_0200769c();
void Func_020076a0();
void Func_020076aa();
void Func_020076ae();
void Func_020076b2();
void Func_020076ba();
void Func_020076c8();
void Func_020076d0();
void Func_020076e6();
void Func_020076fa();
void Func_020076fc();
void Func_02007730();
void Func_02007734();
void Func_020077a0();
void Func_020077c6();
void Func_020077f0();
void Func_0200781e();
void Func_0200782a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunActorEightApproach(void)
{
    u32 i;
    s32 record;

    Func_020075d0();
    Call4(Func_020076fa, -1, -1, -1, 0);
    Func_02007490(1);
    Func_02007678(8, 2);
    Call3(Func_0200767c, 8, 0x13080000, 0x3280000);
    record = Func_02007622(8);
    {
        s32 shown = 0xa000;
    
        *(u16 *)(record + 6) = shown;
    }
    Func_020074b6(1);
    Call2(Func_020077c6, 0x13333, 1);
    Func_020076a0(0, 0, 0);
    Func_02007730(8, 1);
    Func_020074d6(1);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x100;
    Func_020077a0();
    Call3(Func_0200767a, 8, 0x6666, 0x3333);
    Call3(Func_020076ae, 8, 0x12d8, 0x2c8);
    Call3(Func_020076ba, 8, 0x12a8, 0x268);
    Call3(Func_0200769c, 8, 0x4ccc, 0x2666);
    Call3(Func_020076d0, 8, 0x12a8, 0x1d8);
    Call3(Func_020076b2, 8, 0x3333, 0x1999);
    Call3(Func_020076e6, 8, 0x1298, 0x1c8);
    Call3(Func_020076c8, 8, 0x1999, 0xccc);
    Call3(Func_020076fc, 8, 0x1298, 0x1b8);
    Func_02007734(8, 1);
    Func_020076aa(40);
    Func_0200781e();
    Func_0200782a();
    Func_020077f0(110);
}
