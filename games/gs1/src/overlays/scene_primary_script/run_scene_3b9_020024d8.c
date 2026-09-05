#include "types.h"

#define FieldScene_RunScene3b9_020024d8 Func_020024d8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020041c0();
void Func_02005092();
void Func_020050d8();
void Func_020050de();
s32 Func_020050ea();
void Func_02005124();
void Func_02005176();
void Func_020051b4();
void Func_020051ce();
void Func_020051de();
void Func_020051e2();
void Func_020051ec();
void Func_020051f8();
void Func_02005204();
void Func_02005206();
void Func_02005210();
void Func_02005218();
void Func_0200521c();
void Func_02005228();
void Func_02005260();
void Func_0200526c();
void Func_02005278();
void Func_02005282();
void Func_0200528e();
void Func_0200529a();

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

void FieldScene_RunScene3b9_020024d8(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_020050ea, 13);
    Func_020050d8();
    Func_02005204();
    Func_02005218();
    Func_020050de(40);
    Func_02005176(8, 2);
    Func_02005124(13);
    Func_02005092(1);
    Call3(Func_020051ce, 0, 0xe000, 0);
    Func_02005176(13, 1);
    Call3(Func_020051e2, 12, 0xd000, 0);
    Func_020051ec(13, 0, 0);
    Call3(Func_020051f8, 14, 0x8000, 0);
    Call3(Func_02005204, 15, 0xd000, 0);
    Call3(Func_02005210, 16, 0x8000, 0);
    Call3(Func_0200521c, 17, 0xb000, 0);
    Call3(Func_02005228, 18, 0xb000, 0);
    Call1(Func_02005206, 0x2112);
    Func_020041c0(8);
    Func_020051ec(0, 3);
    {
        u16 *target = (u16 *)(rec7 + 100);
        s32 shown = 0x2d0;

        *target = shown;
    }
    {
        u16 *target = (u16 *)(rec7 + 102);
        s32 shown = 112;

        *target = shown;
    }
    Func_020051b4(13, 2);
    Call3(Func_02005260, 12, 0x3000, 0);
    Call3(Func_0200526c, 14, 0xb000, 0);
    Call3(Func_02005278, 15, 0x5000, 0);
    Func_02005282(16, 0, 0);
    Call3(Func_0200528e, 17, 0x5000, 0);
    Call3(Func_0200529a, 18, 0x5000, 0);
    Func_020051de();
}
