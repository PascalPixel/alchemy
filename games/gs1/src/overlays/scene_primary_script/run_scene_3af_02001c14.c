#include "types.h"

#define FieldScene_RunScene3af_02001c14 Func_02001c14

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020027fa();
void Func_020056ac();
void Func_02005f22();
void Func_02005f76();
void Func_02005f86();
void Func_02005fbc();
void Func_02005fc2();
void Func_02005fce();
void Func_02005fda();
void Func_02005fe4();
void Func_02006018();
void Func_0200603a();
void Func_0200604a();
void Func_02006068();
void Func_02006070();
void Func_02006084();
void Func_02006092();
void Func_0200610a();
void Func_02006120();
void Func_0200612c();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3af_02001c14(s32 a0, s32 a1)
{
    u32 i;
    s32 p10;
    s32 record;

    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x100;
    Func_02006070();
    Func_02006084();
    Func_02005f22(20);
    Func_020027fa();
    Call3(Func_02005fbc, a0, 0xd80000, 0x24c0000);
    Call3(Func_02005f76, a0, 0xcccc, 0x6666);
    Call3(Func_02005fc2, a0, 216, 0x258);
    Call3(Func_02005fce, a0, 218, 0x25c);
    Call3(Func_02005fda, a0, 234, 0x25c);
    Call3(Func_02005fe4, a0, 236, 0x26a);
    Call3(Func_02006068, a0, 0x5000, 20);
    Func_02006018(a0, 3);
    Func_02005f86(20);
    Call2(Func_020056ac, a1, 0x5000);
    Func_0200603a(a1, 4, 40);
    Func_0200604a(a1, 2);
    Call1(Func_02006070, 0x1e39);
    Func_02006092(a1, 0, 20);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x202;
    Func_02006120();
    Func_0200612c();
    Func_0200610a(10);
    p10 = a0;
}
