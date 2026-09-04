#include "types.h"

#define FieldScene_RunScene39c_02001340 Func_02001340

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02006cd6();
void Func_02006d7e();
void Func_02006dfc();
void Func_02006e3c();
s32 Func_02006e42();
void Func_02006e82();
void Func_02006e94();
void Func_02006ea6();
void Func_02006eb2();
u8 *Func_02006eb6();
void Func_02006ebe();
void Func_02006ec4();
void Func_02006ef8();
void Func_02006fe2();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunScene39c_02001340(void)
{
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    Func_02006dfc();
    Value2(Func_02006cd6, 0x200ba65, 0xc80);
    Call3(Func_02006e3c, 0, 0x28000, 0x14000);
    Func_02006e94(0, 1);
    *(u8 *)(Func_02006e42(0) + 90) &= 254;
    Func_02006fe2(228);
    if (*(s16 *)((s32)p5 + 0x16c) == 2) {
        Call3(Func_02006ea6, 0, 232, (154 << 2));
    } else if (*(s16 *)((s32)p5 + 0x16c) == 3) {
        Call3(Func_02006ea6, 0, (180 << 1), (182 << 2));
    } else if (*(s16 *)((s32)p5 + 0x16c) == 4) {
        Call3(Func_02006ea6, 0, 248, (198 << 2));
    } else {
        Call3(Func_02006ebe, 0, 696, 592);
        Call3(Func_02006ec4, 0, 696, 600);
        Call1(Func_02006e82, 30);
    }
    Func_02006ef8(0);
    {
        u8 *record = Func_02006eb6(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Call1(Func_02006d7e, 0x200ba65);
    Func_02006eb2();
}
