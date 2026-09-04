#include "types.h"

#define FieldScene_RunScene383SequenceC Func_0200082c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001360[];
extern u8 Data_03001ebc[];
void Func_02004e3e();
void Func_02004e50();
void Func_02004e90();
void Func_02004ea8();
void Func_02004eca();
s32 Func_0200554a();
void Func_02005574();
void Func_020055a8();
void Func_020055b6();
s32 Func_020055d6();
void Func_020055dc();
void Func_02005604();
void Func_02005618();
void Func_02005634();
void Func_02005650();
void Func_02005656();
s32 Func_02005658();
void Func_02005670();
void Func_02005692();
void Func_020056c0();
void Func_020056e2();
void Func_020056f2();
void Func_02005724();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene383SequenceC(void)
{
    u32 i;
    s32 record;

    Func_02005574();
    if (Value1(Func_0200554a, 0x857) == 0) {
        Func_02005656((s32)Data_00001360);
        Func_02004e3e(16, 20);
        Func_02004e90(16, 3, 20);
        Func_02004e50(16, 30);
        Func_02005692(16, 0, 0);
        Func_020055a8(30);
        Func_02005670(16, 2);
        Func_020055b6(30);
        Func_02004ea8(0, 16, 20);
        Func_02004eca(16, 3, 20);
        bump_step(1);
        if (Value0(Func_020055d6) == 0) {
            Func_020056c0(((s32)Data_00001360 + 3));
            Func_02004ea8(16, 20);
            Func_02005604();
            goto L_02000906;
        }
        Call1(Func_020055dc, 0x857);
        Func_02005634(189, 0);
    }
    Call1(Func_020056e2, 0x1364);
    Func_020056f2(16, 0);
    Func_02005618(20);
    if (Value2(Func_02005658, 0, 0) != 0) {
        bump_step(1);
    }
    Func_02005724(16, 0);
    Func_02005650();
    L_02000906:;
}
