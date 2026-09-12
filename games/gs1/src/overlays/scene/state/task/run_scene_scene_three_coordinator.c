#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
s32 Func_02001930();
void Func_02003346();
s32 Func_0200355c();
void Func_02003750();
s32 Func_020037cc();
void Func_0200417a();
void Func_020043fe();
s32 Func_020046d8();
s32 Func_020046e2();
void Func_020046f6();
void Func_02005490();
void Func_020054c6();
void Func_02005508();
void Func_0200553e();
void Func_02005544();
void Func_02005546();
void Func_02005570();
void Func_02005590();
void Func_02005592();
void Func_020055a8();
void Func_020055ac();
void Func_020055ae();
void Func_020055b8();
void Func_020055ba();
void Func_020055d2();
void Func_020055d8();
void Func_020055da();
void Func_020055e2();
void Func_020055e6();
void Func_020055fc();
void Func_02005620();
void Func_02005652();
void Func_02005654();
void Func_0200565c();
void Func_02005676();
void Func_0200568e();
void Func_02005692();

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

void FieldScene_RunSceneThreeCoordinator(s32 a0)
{
    u32 i;
    s32 rec2;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_02003346();
    } else {
        Func_02005490();
        rec2 = Value2(Func_0200355c, a0, 3);
        if (rec2 != 0) {
        } else {
            Call1(Func_0200553e, 0x20a6);
            Call2(Func_02005592, 0x30000, 0x6000);
            Call4(Func_020055ac, 0x2f00000, -1, 0xc00000, 1);
            Func_020055b8();
            Func_020054c6(60);
            Call2(Func_020055ba, 0x10000, 0x2000);
            Call4(Func_020055d8, 0x2f00000, -1, 0xe00000, 1);
            Func_020055e6();
            Func_020055a8(a0, 0);
            Func_0200417a(0, 0x358, 0x108);
            Func_02005508(10);
            Call3(Func_02005546, 0, 0x18000, 0xc000);
            Value3(Func_020046d8, 0, 0x358, 0x108);
            Value3(Func_020046e2, 0, 0x358, 232);
            Func_020055e2(a0, 0);
            Call3(Func_020046f6, 0, 0x348, 232);
            Func_02005544(10);
            Value3(Func_02001930, 33, -64, 0);
            Call4(Func_02005652, 0x2f00000, -1, 0xd80000, 1);
            Func_020055d2(0, 1);
            Func_02005570(10);
            Call3(Func_020055ae, 0, 0x10000, 0x8000);
            Call3(Func_020055da, 0, 0x2f8, 232);
            Func_02005590(10);
            Call3(Func_0200565c, 0, 0x4000, 30);
            Func_02005654(a0, 0);
            Func_020043fe(0);
            Func_02005692(0, 0);
            Call3(Func_02005620, 33, 0x3480000, 0xe80000);
            Func_02003750(a0, 3);
            goto L_020016b0;
        }
        if (rec2 == 1) {
            Call1(Func_02005676, 0x20a5);
            Func_0200568e(a0, 0);
        }
        L_020016b0:;
        Value3(Func_020037cc, rec2, a0, 3);
        Func_020055fc();
    }
}
