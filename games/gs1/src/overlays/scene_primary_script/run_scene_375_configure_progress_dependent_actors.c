#include "types.h"

#define FieldScene_ConfigureProgressDependentActors Func_0200150c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02001ffa();
s32 Func_02002f98();
void Func_02002fa2();
s32 Func_02002fa8();
void Func_02002fb2();
void Func_02002fda();
s32 *Func_0200302a();
s32 Func_02003032();
void Func_02003044();
void Func_0200304e();
void Func_02003050();
s32 *Func_02003058();
void Func_02003062();
void Func_0200306c();
void Func_02003074();
void Func_02003078();
void Func_02003082();
void Func_02003088();
void Func_0200308a();
void Func_0200309e();
void Func_020030a2();
void Func_020030ae();
void Func_020030c0();
s32 Func_020030ea();
void Func_020030f6();
s32 Func_02003100();
void Func_02003104();
s32 Func_0200312a();
void Func_02003142();
void Func_0200314c();
void Func_02003156();
void Func_02003160();
s32 Func_0200316a();
void Func_0200318e();
void Func_02003198();
void Func_020031a2();
void Func_020031a8();
void Func_020031bc();
void Func_020031ca();
void Func_020031fc();
void Func_02003218();
void Func_02003224();

extern s16 Data_02000240[];
extern u8 Data_02009ce0[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2_10(void (*f)(), s32 a1, s32 a0)
{
    f(a0, a1);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 *Value1p(s32 *(*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call3_120(void (*f)(), s32 a1, s32 a2, s32 a0)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 FieldScene_ConfigureProgressDependentActors(void)
{
    s16 *table;
    u8 *progress;
    u8 *record;
    s32 *p;
    s32 n21;
    s32 n19;
    s32 n23;
    s32 n24;
    s32 n20;
    s32 base;
    u32 lim;
    s32 twelve;
    u8 value;

    table = Data_02000240;
    progress = (u8 *)&table[225];
    lim = 128;
    if ((u32)((*(u16 *)progress - 5) << 16) <= (lim << 9)) {
        Call1(Func_02002fa2, 0x12f);
    }
    if (Value1(Func_02002f98, 0x109) != 0) {
        Call1(Func_02002fb2, 0x242);
    }
    if (Value1(Func_02002fa8, 0x834) != 0) {
        Call0(Func_020030ea);
        Call0(Func_020030f6);
        Call0(Func_02002fda);
        Call3(Func_02003044, 12, 0, 0);
        Call3(Func_0200304e, 13, 0, 0);
        Call3(Func_02003058, 14, 0, 0);
        Call3(Func_02003062, 15, 0, 0);
        Call3_120(Func_0200306c, 0, 0, 5);
        p = Value1p(Func_0200302a, 8);
        record = (u8 *)p + 89;
        value = *(volatile u8 *)record;
        *(volatile u8 *)record = (u8)(value | 8);
        Call3(Func_02003088, 11, 166 << 15, 0x01090000);
        Call3(Func_02003082, 11, 83, 0x111);
        Call2_10(Func_020030a2, 5, 11);
        p = Value1p(Func_02003058, 11);
        twelve = 12;
        *(u16 *)((u8 *)p + 32) = twelve;
        Call2_10(Func_02003074, 0x02009c34, 11);
        if (Value1(Func_02003032, 0x839) != 0) {
            Call3(Func_020030c0, 11, 0, 0);
        }
        Call0(Func_0200306c);
        n21 = 21;
        Call6(Func_02003050, 9, 24, 1, 1, 14, n21);
        Call6(Func_02003062, 9, 24, 1, 1, 15, n21);
        n23 = 23;
        n19 = 19;
        Call6(Func_02003078, 9, 24, 1, 1, n23, n19);
        n24 = 24;
        Call6(Func_0200308a, 9, 24, 1, 1, n24, n19);
        n20 = 20;
        Call6(Func_0200309e, 9, 24, 1, 1, n23, n20);
        Call6(Func_020030ae, 9, 24, 1, 1, n24, n20);
    } else {
        Call3(Func_02003142, 9, 0, 0);
        Call3(Func_0200314c, 10, 0, 0);
        Call3(Func_02003156, 1, 0, 0);
        Call3(Func_02003160, 11, 0, 0);
        Call0(Func_02003104);
        if (Value1(Func_020030ea, 0x109) == 0) {
            if (*(s16 *)progress == 10) {
                Call0(Func_02001ffa);
            }
        }
        if (Value1(Func_02003100, 0x801) != 0) {
            Call3(Func_0200318e, 13, 0, 0);
            Call3(Func_02003198, 14, 0, 0);
            Call3(Func_020031a2, 15, 0, 0);
        } else if (Value1(Func_0200312a, 0x808) != 0) {
            Call3_120(Func_020031bc, 196 << 17, 188 << 17, 14);
            Call3_120(Func_020031ca, 188 << 17, 188 << 17, 15);
            base = (s32)Data_02009ce0;
            Call3(Func_02003218, 14, 128 << 9, base);
            Call3(Func_02003224, 15, 128 << 9, base);
        }
        if (Value1(Func_0200316a, 0x87a) != 0) {
            Call3_120(Func_020031fc, 132 << 16, 132 << 17, 16);
        }
        Call0(Func_020031a8);
    }
    return 0;
}
