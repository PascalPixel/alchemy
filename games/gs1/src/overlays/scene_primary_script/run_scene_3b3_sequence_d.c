#include "types.h"

#define Lifted_020011c4 Func_020011c4

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


s32 Func_02001268(void)
{
    u8 *rec;
    u8 *pflag;
    s32 saved;
    s32 mode;
    s32 *p;
    s32 buf[3];

    rec = Func_02003d94(0);
    pflag = rec + 85;
    saved = *pflag;
    mode = (*(volatile u16 *)(rec + 6) + 0x2000) & 0xc000;
    if (Data_02000240_t[249][0] != 0) {
        return 0;
    }
    p = buf;
    p[0] = (*(volatile s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(volatile s32 *)(rec + 12);
    p[2] = (*(volatile s32 *)(rec + 16) & -0x100000) + 0x80000;
    Call3(Func_02003d20, 0x100000, mode, (s32)p);
    if (Value2(Func_02003da0, (s32)rec, (s32)p) == 1) {
        goto reject;
    }
    if (Value2(Func_02001630, (s32)p, (s32)rec) != 0) {
        goto reject;
    }
    p[0] = (*(volatile s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(volatile s32 *)(rec + 12);
    p[2] = (*(volatile s32 *)(rec + 16) & -0x100000) + 0x80000;
    Call3(Func_02003d5e, 0x200000, mode, (s32)p);
    if (Value2(Func_02001662, (s32)p, (s32)rec) != 0) {
        goto reject;
    }
    if (Value2(Func_02003dea, (s32)rec, (s32)p) != 0) {
        goto reject;
    }
    Func_02003e32();
    Func_02003d92((s32)rec, 6);
    Func_02003d68(6);
    Func_02003eee(152);
    Func_02003da6((s32)rec, 7);
    *(volatile s32 *)(rec + 48) = 0x30000;
    *(volatile s32 *)(rec + 52) = 0x20000;
    *(volatile s32 *)(rec + 40) = 0x40000;
    *pflag &= 126;
    Func_02003e3c((s32)rec, 0);
    Func_02003eaa(0, *(s16 *)((u8 *)p + 2), *(s16 *)((u8 *)p + 10));
    Func_02003de2((s32)rec, 6);
    Func_02003e5a((s32)rec, 1);
    *pflag = saved;
    Func_02003ea4();
    return 1;
reject:
    return 0;
}

