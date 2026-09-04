#include "types.h"

#define FieldScene_RunScene3b9_0200039c Func_0200039c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02002f62();
s32 Func_02002f6e();
void Func_02002f94();
s32 Func_02002fd8();
void Func_02002ff0();
s32 Func_02003008();
void Func_02003046();
void Func_02003050();
void Func_02003058();
s32 Func_02003068();
s32 Func_02003098();
void Func_020030b8();
void Func_020030c8();
void Func_020030d2();
void Func_020030da();
void Func_020030fa();

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

void FieldScene_RunScene3b9_0200039c(void)
{
    u32 i;
    s32 record;

    Func_02002f94();
    if (Value1(Func_02002f62, 0x962) != 0) {
        if (Value1(Func_02002f6e, 0x3c0) != 0) {
            Call1(Func_02003050, 0x225e);
        } else {
            Call1(Func_02003058, 0x225a);
            Value2(Func_02003068, 16, 0);
            if (Value2(Func_02002fd8, 0, 0) == 0) {
                bump_step(1);
                Call3(Func_020030b8, 16, 0x100, 40);
                Value2(Func_02003098, 16, 0);
                if (Value2(Func_02003008, 0, 0) == 0) {
                    bump_step(1);
                }
                ((void (*)())Func_02003008)(40);
                Func_020030c8(16, 0);
                Call1(Func_02002ff0, 0x3c0);
                goto L_02000448;
            }
        }
        Func_020030da(16, 0);
    } else {
        Call1(Func_020030d2, 0x205e);
        Func_020030fa(16, 0);
    }
    L_02000448:;
    Func_02003046();
}
