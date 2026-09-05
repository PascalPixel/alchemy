#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020016d8();
void Func_02001716();
void Func_02001740();
void Func_0200177c();
void Func_0200178c();
void Func_02001796();
void Func_02001798();
void Func_020017a4();
void Func_020017a6();
void Func_020017ae();
void Func_020017b0();
void Func_020017c2();
void Func_020017c4();
void Func_020017d6();
void Func_02001824();
void Func_0200183a();
void Func_02001846();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

void FieldScene_RunScene3b0_020004b0(void)
{
    u32 i;
    s32 record;

    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x202;
    Func_020017ae();
    Func_020017c2();
    Func_020016d8(20);
    Call3(Func_02001716, 8, 0x10000, 0x8000);
    Call3(Func_02001740, 8, 164, 0x141);
    Call3(Func_0200178c, 8, 0xd000, 40);
    Call3(Func_02001798, 8, 0xb000, 40);
    Call3(Func_020017a4, 8, 0xd000, 40);
    Call3(Func_020017b0, 8, 0x3000, 10);
    Call3(Func_0200177c, 8, 164, 0x14e);
    Func_02001796(8, 4, 40);
    Func_020017a6(8, 2);
    Call1(Func_020017c4, 0x1e3a);
    Func_020017d6(8, 0, 20);
    Func_0200183a();
    Func_02001846();
    Func_02001824(10);
}
