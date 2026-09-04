#include "types.h"

#define FieldScene_RunScene389_0200121c Func_0200121c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern s16 Data_02000240[];
extern u8 Data_020098ec[];
extern u8 Data_03001ebc[];
void Func_02002046();
void Func_0200265e();
s32 Func_0200266a();
void Func_02002672();
u8 *Func_0200268a();
void Func_0200269c();
s32 Func_020026aa();
s32 Func_020026d2();
void Func_020026e4();
void Func_02002706();
void Func_02002720();
s32 Func_02002730();
void Func_02002734();
s32 Func_0200273c();
u8 *Func_02002748();
void Func_0200274c();
void Func_0200277c();
s32 Func_0200279a();
void Func_020027c4();
void Func_020027c6();
void Func_02002824();
void Func_02002830();

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

s32 FieldScene_RunScene389_0200121c(void)
{
    u8 *record;
    u8 *work;

    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x204;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 24;
    {
        u8 *record = Func_0200268a(9);
        u8 value = *(volatile u8 *)&record[89];
    
        record[89] = (u8)(value | 16);
    }
    if (Value1(Func_0200266a, 0x302) != 0) {
        Call3(Func_020026e4, 8, 0x1580000, 0x680000);
        Call6(Func_0200265e, 24, 40, 6, 3, 18, 6);
    } else {
        Call6(Func_02002672, 18, 40, 6, 3, 18, 6);
    }
    if (Value1(Func_020026aa, 0x300) != 0) {
        Func_02002720(9, 0, 0);
        Call6(Func_0200269c, 21, 45, 4, 2, 21, 11);
    }
    if (Value1(Func_020026d2, 0x301) != 0) {
        Call3(Func_0200274c, 10, 0x2680000, 0xe80000);
        if ((u32)(((u16)Data_02000240[225] - 2) << 16) > 0x10000) {
            goto L_0200131c;
        }
        *(u8 *)(Func_02002730(10) + 34) = 2;
        record = Value1(Func_0200273c, 10);
        *(volatile s32 *)((s32)record + 12) = (*(volatile s32 *)((s32)record + 12) - 1);
        {
            u8 mask = 2;
            u8 *record = Func_02002748(10);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | mask);
        }
        Call6(Func_02002706, 36, 48, 5, 1, 36, 14);
    } else {
    }
    L_0200131c:;
    if (Data_02000240[225] == 99) {
        Func_02002824();
        Func_02002830();
        Call3(Func_020027c6, 9, 0x1800000, 0xc00000);
        Func_0200277c(60);
        *(u8 *)(Func_0200279a(9) + 34) = 2;
        Call3(Func_020027c4, 9, 0x198, 192);
        Call1((void (*)())Func_0200279a, 60);
        Func_02002046();
    }
    if (Data_02000240[282] != 0) {
        {
            s32 zero = 0;
            *(volatile s32 *)Data_020098ec = zero;
        }
        Call2(Func_02002734, 0x20090c9, 0xc80);
    }
    return 0;
}
