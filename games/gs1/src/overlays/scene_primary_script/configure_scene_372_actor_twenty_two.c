#include "types.h"

#define FieldScene_ConfigureActorTwentyTwoScene Func_02003fb0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200861e();
void Func_02008656();
void Func_02008676();
void Func_020086b0();
void Func_02008720();
u8 *Func_02008728();
void Func_02008752();
void Func_0200875c();
void Func_02008776();
void Func_020087a0();
void Func_020087b0();
void Func_020087c0();
void Func_020087ea();
void Func_020087ec();
void Func_020087f2();
void Func_020087f4();
void Func_02008802();
void Func_0200880c();
void Func_02008836();
void Func_02008838();
void Func_0200883e();
void Func_02008842();
void Func_0200885c();
void Func_02008862();
void Func_02008878();
void Func_0200887a();
void Func_02008884();
void Func_02008896();

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

void FieldScene_ConfigureActorTwentyTwoScene(void)
{
    u32 i;
    u8 *record;

    Func_020086b0();
    Call4(Func_020087ea, -1, -1, -1, 0);
    Func_02008720(22);
    Call1(Func_0200861e, 0x200c5b9);
    Call3(Func_0200875c, 0, 0x1e0, 0x570);
    Func_02008776(0, 0, 0);
    Call3(Func_020087f2, 22, 0x3000, 20);
    {
        u8 *record = Func_02008728(22);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Call3(Func_020087a0, 22, 0xf90000, 0x4d80000);
    Func_02008656(1);
    Call1(Func_020087f4, 0xed3);
    Call2(Func_0200880c, 0x1016, 0);
    Call3(Func_020087c0, 22, 0xac0000, 0x4fe0000);
    Func_02008676(1);
    Call2(Func_02008862, 0x40000, 0x8000);
    Call4(Func_02008878, 0xa20000, 0, 0x5050000, 1);
    Func_02008884();
    Func_02008752(40);
    Func_02008802(22, 4);
    Call3(Func_0200885c, 0x1016, 0, 10);
    Call3(Func_02008878, 22, 0xc000, 20);
    Func_02008838(22, 2);
    Call3(Func_0200887a, 0x1016, 0, 10);
    Call3(Func_02008896, 22, 0x1000, 20);
    Func_0200883e(22, 3);
    Call3(Func_020087ec, 22, 0x20000, 0x10000);
    Call3(Func_02008836, 22, 165, 0x514);
    Call3(Func_02008842, 22, 195, 0x598);
    Call1(Func_020087b0, 0x842);
}
