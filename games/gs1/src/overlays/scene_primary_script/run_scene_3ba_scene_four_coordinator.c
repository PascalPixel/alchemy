#include "types.h"

#define FieldScene_RunSceneFourCoordinator Func_020016ec

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
s32 Func_02001b18();
void Func_0200325e();
s32 Func_02003474();
void Func_02003680();
s32 Func_020036fa();
s32 Func_02004080();
void Func_0200432e();
s32 Func_0200447a();
void Func_02004546();
s32 Func_02004606();
s32 Func_02004610();
void Func_0200461c();
void Func_020053a8();
void Func_020053fc();
void Func_0200544e();
void Func_0200546a();
void Func_02005474();
void Func_02005488();
void Func_02005494();
void Func_0200549a();
void Func_020054b4();
void Func_020054c0();
void Func_020054c2();
void Func_020054ca();
void Func_020054d2();
void Func_020054de();
void Func_020054f6();
void Func_020054fa();
void Func_020054fe();
void Func_0200552a();
void Func_02005550();
void Func_02005566();
void Func_02005578();
void Func_0200557c();
void Func_02005594();
void Func_0200559c();
void Func_020055b2();
void Func_020055b4();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunSceneFourCoordinator(s32 a0)
{
    u32 i;
    s32 rec;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_0200325e();
    } else {
        Func_020053a8();
        rec = Value2(Func_02003474, a0, 4);
        if (rec != 0) {
        } else {
            Call1(Func_0200544e, 0x2099);
            Call2(Func_0200549a, 0x30000, 0x6000);
            Call4(Func_020054b4, 0x4400000, -1, 0xa80000, 1);
            Func_020054c0();
            Func_02005488(a0, 0);
            Value3(Func_0200447a, 120, 72, 0);
            Func_0200549a(a0, 0);
            Func_02004546();
            Func_020053fc(15);
            Value3(Func_02004080, 0, 0x3d8, 200);
            Func_020054ca(0, 0, 10);
            Func_020054c2(a0, 0);
            Call3(Func_020054de, 0, 0x4000, 30);
            Call3(Func_020054fa, 0, 0x106, 60);
            Call3(Func_02005474, 0, 0x18000, 0xc000);
            Func_02004606(0, 0x3e8, 192);
            Value3(Func_02004610, 0, 0x3e8, 176);
            Call3(Func_0200461c, 0, 0x3f8, 168);
            Func_0200546a(15);
            Value3(Func_02001b18, 18, 160, 0);
            Call4(Func_02005566, 0x4400000, -1, 0xa80000, 1);
            Func_020054f6(0, 1);
            Func_02005494(10);
            Call3(Func_020054d2, 0, 0x10000, 0x8000);
            Call3(Func_020054fe, 0, 0x4a8, 168);
            Func_020054b4(10);
            Call3(Func_02005578, 0, 0x8000, 30);
            Call3(Func_02005594, 0, 0x102, 60);
            Func_0200557c(a0, 0);
            Func_0200432e(0);
            Func_020055b2(0, 0);
            Call3(Func_02005550, 18, 0x3f80000, 0xa80000);
            Func_02003680(a0, 4);
            goto L_02001876;
        }
        if (rec == 1) {
            Call1(Func_0200559c, 0x2098);
            Func_020055b4(a0, 0);
        }
        L_02001876:;
        Value3(Func_020036fa, rec, a0, 4);
        Func_0200552a();
    }
}
