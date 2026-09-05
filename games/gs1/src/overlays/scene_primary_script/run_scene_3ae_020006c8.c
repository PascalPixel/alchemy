#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02001c22();
void Func_02001c4c();
void Func_02001c5e();
void Func_02001c78();
s32 Func_02001c80();
void Func_02001c94();
void Func_02001cb2();
void Func_02001cc6();
void Func_02001cd8();
void Func_02001ce8();
s32 Func_02001cee();
void Func_02001d04();
void Func_02001d0c();
void Func_02001d1c();
void Func_02001d1e();
void Func_02001d24();
void Func_02001d34();
void Func_02001d3a();
void Func_02001d48();
void Func_02001d52();
void Func_02001d5c();
s32 Func_02001d6e();
void Func_02001d90();
void Func_02001d96();
void Func_02001db6();
void Func_02001dde();
void Func_02001df6();

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

void FieldScene_RunScene3ae_020006c8(void)
{
    u32 i;
    s32 record;

    Func_02001c4c();
    if (Value1(Func_02001c22, 0x8a8) != 0) {
        Func_02001ce8(11, 0, 0);
        Func_02001c5e(20);
        Call1(Func_02001d04, 0x1f1c);
        Func_02001d1c(11, 0);
        ((void (*)())Func_02001c80)();
    } else {
        Func_02001c78(20);
        Call3(Func_02001d5c, 11, 0x100, 50);
        Func_02001d1e(11, 0, 0);
        Func_02001c94(20);
        Call1(Func_02001d3a, 0x1f18);
        Func_02001d52(11, 0);
        if (Value1(Func_02001c80, 0x8a6) != 0) {
            Func_02001cb2(20);
            Call3(Func_02001d96, 11, 0x102, 40);
            Value2(Func_02001d6e, 11, 0);
            if (Value2(Func_02001cee, 0, 0) == 0) {
                Func_02001cd8(20);
                Func_02001d90(11, 0);
                Call1(Func_02001cc6, 0x8a8);
                goto L_020007be;
            }
            ((void (*)())Func_02001cee)(10);
            bump_step(1);
            Func_02001db6(11, 0);
            Func_02001d0c(10);
            Func_02001dde(11, 0, 0);
            Func_02001d1c(30);
        } else {
            Func_02001d24(10);
            Func_02001df6(11, 0, 0);
            Func_02001d34(30);
        }
        L_020007be:;
        Func_02001d48();
    }
}
