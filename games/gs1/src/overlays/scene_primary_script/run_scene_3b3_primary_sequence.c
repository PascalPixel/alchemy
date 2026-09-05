#include "types.h"

#define FieldScene_RunPrimarySequence Func_020011c4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern u8 Data_02000240_t[][2];
s32 Func_02001268();
void Func_02001386();
s32 Func_02001630();
s32 Func_02001662();
s32 Func_02003c3e();
s32 Func_02003c52();
s32 Func_02003c5c();
s32 Func_02003c66();
s32 Func_02003cee();
void Func_02003d20();
void Func_02003d5e();
void Func_02003d68();
void Func_02003d92();
u8 *Func_02003d94();
s32 Func_02003da0();
void Func_02003da6();
void Func_02003de2();
s32 Func_02003dea();
s32 Func_02003e08();
s32 Func_02003e14();
s32 Func_02003e22();
void Func_02003e32();
void Func_02003e3c();
void Func_02003e5a();
void Func_02003ea4();
void Func_02003eaa();
void Func_02003eee();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

void FieldScene_RunPrimarySequence(void)
{
    s32 rec;
    s32 flag;
    s32 p6;
    s32 p5;
    s32 record;
    s32 v1;
    s32 v2;
    s32 v3;
    u8 *base;
    u8 slot16[40];

    rec = Func_02003cee(0);
    flag = *(volatile s32 *)0x03001e40 & 3;
    if (flag == 0) {
        base = slot16;
        *(s32 *)(base + 4) = 10;
        *(s32 *)(base + 8) = 0xb333;
        *(s32 *)(base + 12) = 0xb333;
        v1 = Func_02003c3e();
        p6 = *(volatile s32 *)(rec + 8) + ((((u32)((v1 << 4) + v1) >> 16) - 8) << 16);
        v2 = Func_02003c52();
        p5 = *(volatile s32 *)(rec + 16) + ((((u32)((v2 << 4) + v2) >> 16) - 8) << 16);
        v3 = Func_02003c66();
        record = Func_02003c5c((((u32)((v3 << 2) + v3) >> 16) << 16) + 0x30000, 10);
        Call8(Func_02001386, p6, *(volatile s32 *)(rec + 12), p5, 0, record, flag, 0x90001, (s32)base);
    }
}


