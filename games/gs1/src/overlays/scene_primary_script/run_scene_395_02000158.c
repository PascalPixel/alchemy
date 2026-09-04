#include "types.h"

#define FieldScene_RunScene395_02000158 Func_02000158

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001462();
void Func_02001478();
void Func_0200148c();
s32 Func_020014a2();
s32 Func_02001ae2();
void Func_02001af4();
void Func_02001af6();
s32 Func_02001b0c();
void Func_02001b26();
s32 Func_02001b2c();
void Func_02001b94();
void Func_02001bac();
void Func_02001bbe();
void Func_02001bc2();
void Func_02001bd6();
void Func_02001bf2();
void Func_02001c0a();
void Func_02001c10();
void Func_02001c30();
void Func_02001c38();
void Func_02001c5a();
void Func_02001c70();
void Func_02001c8a();
void Func_02001ca0();

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

void FieldScene_RunScene395_02000158(void)
{
    u32 i;
    s32 record;

    Func_02001af4();
    if (Value1(Func_02001ae2, 0x845) != 0) {
        Func_02001462(10, 1);
        Call1(Func_02001b94, 0x151c);
        Func_02001bac(8, 0);
        Func_02001478(10, 0);
    } else {
        if (Value1(Func_02001b0c, 0x844) != 0) {
            Func_0200148c(10, 1);
            Call1(Func_02001bbe, 0x14eb);
            Func_02001bd6(8, 0);
            Value2(Func_020014a2, 10, 0);
            record = Func_02001b2c(184);
            if (record == -1) {
                goto L_02000220;
            }
            {
                u16 *target = (u16 *)(*(u8 *volatile *)Data_03001ebc + 0x172);
                s32 shown = 1;

                *target = shown;
            }
        } else {
            Call1(Func_02001bf2, 0x14c9);
            Func_02001c0a(8, 0);
            Call2(Func_02001c5a, 0x406218, 1);
            Func_02001c70(20);
            Func_02001af6(40);
            Call3(Func_02001c30, 0x200e, 0, 10);
            Func_02001c10(0, 2);
            Call2(Func_02001c38, 0x200e, 0);
            Call2(Func_02001c8a, 0x10000, 1);
            Func_02001ca0(20);
            Func_02001b26(40);
        }
    }
    L_02000220:;
    Func_02001bc2();
}
