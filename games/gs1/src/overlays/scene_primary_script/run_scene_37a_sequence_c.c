#include "types.h"

#define FieldScene_RunScene37aSequenceC Func_02000150

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000bc4();
s32 Func_02002704();
s32 Func_02002b14();
void Func_02002b3c();
void Func_02002b4a();
s32 Func_02002b4c();
void Func_02002b50();
void Func_02002b6c();
void Func_02002b70();
void Func_02002b74();
s32 Func_02002b7c();
void Func_02002b80();
void Func_02002b86();
void Func_02002bb0();
void Func_02002c2e();
void Func_02002c44();
void Func_02002c64();
void Func_02002c7a();

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene37aSequenceC(void)
{
    u32 i;
    s32 record;

    if (Value0(Func_02002704) != 0) {
        record = Value1(Func_02002b14, 0x200);
        if (record != 0) {
            goto L_020001d6;
        }
        Func_02002b3c();
        Call2(Func_02002c2e, 0x10000, 1);
        Func_02002c44(20);
        Call1(Func_02002b3c, 0x200);
        Call1(Func_02002b4a, 0x201);
        Call1(Func_02002b50, 0x202);
        Func_02002b6c();
    } else {
        if (Value1(Func_02002b4c, 0x201) == 0) {
            Func_02002b74();
            Call2(Func_02002c64, 0x2051cc, 1);
            Func_02002c7a(20);
            Call1(Func_02002b70, 0x201);
            Call1(Func_02002b80, 0x200);
            Call1(Func_02002b86, 0x202);
            if (Value1(Func_02002b7c, 0x80a) == 0) {
                Func_02000bc4();
            }
            Func_02002bb0();
        }
    }
    L_020001d6:;
}
