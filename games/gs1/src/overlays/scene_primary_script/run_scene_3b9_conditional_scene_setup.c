#include "types.h"

#define FieldScene_RunConditionalSceneSetup Func_020011c4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02002e32();
s32 Func_02003d8a();
void Func_02003da8();
void Func_02003dbc();
s32 Func_02003e1a();
void Func_02003e62();
void Func_02003e70();
void Func_02003e74();
void Func_02003e78();
void Func_02003e9a();
void Func_02003eb2();
void Func_02003f00();
s32 Func_02003f06();
void Func_02003f1a();
void Func_02003f26();
void Func_02003f64();
void Func_02003f6c();
void Func_02003f78();
void Func_02003f84();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunConditionalSceneSetup(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Func_02003dbc();
    rec7 = Value1(Func_02003d8a, 0x8a4);
    if (rec7 != 0) {
        Func_02003e62(17, 0, 40);
        Call1(Func_02003e78, 0x206f);
        Func_02002e32(17);
        Call3(Func_02003eb2, 17, 0x3000, 20);
    } else {
        Func_02003e74(17, 2);
        Call1(Func_02003e9a, 0x206d);
        Func_02003eb2(17, 0);
        *(u8 *)(Func_02003f06() + 85) = rec7;
        Func_02003da8(1);
        Call2(Func_02003f00, 0x66666, 0xcccc);
        Call4(Func_02003f1a, 0x21c0000, -1, 0xd00000, 1);
        Func_02003f26();
        *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x200;
        *(s32 *)((*(u8 **)0x03001ebc + 0x1c8)) = 32;
        Func_02003f78();
        Func_02003f84();
        if (Value1(Func_02003e1a, 0x8a3) != 0) {
            Func_02003f64(70);
        } else {
            Func_02003f6c(7);
        }
    }
    Func_02003e70();
}
