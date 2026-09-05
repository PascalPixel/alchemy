#include "types.h"

#define FieldScene_RunScene3a2SequenceC Func_02000b2c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02002020();
void Func_02002026();
void Func_0200204c();
void Func_0200206a();
void Func_02002082();
void Func_020020b0();
void Func_020020ca();
void Func_020020d6();
void Func_020020ea();
void Func_020020ec();
void Func_020020f2();
void Func_020020f4();
void Func_020020fa();
void Func_0200210c();
void Func_02002114();
void Func_0200211c();
void Func_02002122();
void Func_02002124();
void Func_02002126();
void Func_02002128();
void Func_0200212a();
void Func_0200213c();
void Func_02002152();
void Func_02002158();
void Func_02002164();
void Func_02002170();
void Func_02002184();
void Func_0200218c();
void Func_020021b8();
void Func_020021be();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3a2SequenceC(void)
{
    u32 i;
    s32 record;

    Func_02002020();
    Func_0200211c();
    Func_02002128();
    Func_02002026(30);
    Call3(Func_020020f2, 0, 0x6000, 0);
    Func_0200212a(9, 1);
    Func_02002126();
    Func_020020d6(8, 2);
    Func_0200204c(20);
    Call1(Func_020020ea, 0x19cf);
    Func_0200210c(8, 0, 20);
    Func_020020f4(9, 1);
    Func_0200206a(20);
    Func_02002124(9, 0, 20);
    Func_020020f4(8, 3);
    Func_02002082(20);
    Func_0200213c(8, 0, 20);
    Call3(Func_02002158, 10, 0xd000, 20);
    Func_02002152(10, 0, 20);
    Func_02002122(8, 3);
    Func_020020b0(60);
    Call3(Func_0200218c, 8, 0x102, 60);
    Func_0200213c(8, 4);
    Func_020020ca(20);
    Func_02002184(8, 0, 20);
    Func_02002164(10, 2);
    Call2(Func_020021be, 10, 0x102);
    Func_020020ec(60);
    Call3(Func_020021b8, 10, 0xb000, 20);
    Func_02002170(9, 5);
    Func_02002114();
    Call1(Func_020020fa, 0x8b1);
}
