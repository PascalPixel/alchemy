#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000242e[];
extern u8 Data_00002430[];
extern u8 Data_03001ebc[];
s32 Func_0200a212();
s32 Func_0200a23a();
void Func_0200a24a();
void Func_0200a272();
void Func_0200a2a4();
void Func_0200a2ac();
void Func_0200a2ae();
void Func_0200a2b2();
void Func_0200a2ea();
void Func_0200a2f0();
void Func_0200a2f4();
void Func_0200a300();
s32 Func_0200a302();
void Func_0200a30c();
void Func_0200a324();
void Func_0200a32c();
void Func_0200a33a();
void Func_0200a352();
void Func_0200a356();
void Func_0200a36e();
void Func_0200a37a();
void Func_0200a380();
void Func_0200a392();
s32 Func_0200a3a2();
void Func_0200a3ac();
void Func_0200a3bc();
void Func_0200a3c2();
void Func_0200a3d2();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

void RunActor20SceneSequence(void)
{
    u32 i;
    s32 record;
    s32 base5_242e;
    s32 base5_2430;

    if (Value1(Func_0200a212, 0x226) != 0) {
        Call1(Func_0200a2f4, 0x2434);
        Func_0200a30c(20, 0);
    } else {
        Func_0200a24a();
        Func_0200a2f4(20, 0, 0);
        if (Value1(Func_0200a23a, 0x227) == 0) {
            Func_0200a2f0(20, 4, 0);
            Func_0200a2ae(20);
            Func_0200a2ac(20);
            Func_0200a272(20);
            base5_242e = (s32)Data_0000242e;
            Func_0200a33a(base5_242e);
            Func_0200a352(20, 0);
            Call3(Func_0200a380, 20, 0x102, 30);
            Func_0200a356((base5_242e + 1));
            Func_0200a36e(20, 0);
            Func_0200a2a4(30);
            Func_0200a32c(20, 4);
            Func_0200a2b2(30);
        }
        base5_2430 = (s32)Data_00002430;
        Func_0200a37a(base5_2430);
        Func_0200a392(20, 0);
        Call3(Func_0200a3bc, 20, 0x101, 40);
        Func_0200a392((base5_2430 + 1));
        Value2(Func_0200a3a2, 20, 0);
        if (Value2(Func_0200a302, 0, 0) == 0) {
            Func_0200a3ac((base5_2430 + 2));
            Func_0200a3bc(20, 0);
            Call1(Func_0200a2ea, 0x226);
        } else {
            Func_0200a3c2((base5_2430 + 3));
            Func_0200a3d2(20, 0);
        }
        Call1(Func_0200a300, 0x227);
        Func_0200a324();
    }
}
