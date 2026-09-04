#include "types.h"

#define FieldScene_RunScene37aSequenceA Func_02000054

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000a8e();
void Func_02001c98();
s32 Func_02002608();
s32 Func_0200264e();
s32 Func_02002a16();
void Func_02002a3a();
void Func_02002a3e();
s32 Func_02002a46();
void Func_02002a4a();
void Func_02002a50();
s32 Func_02002a5c();
s32 Func_02002a72();
void Func_02002a90();
void Func_02002a9a();
void Func_02002aa8();
void Func_02002aae();
void Func_02002aca();
void Func_02002b2e();
void Func_02002b44();
void Func_02002b8c();
void Func_02002ba2();

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

void FieldScene_RunScene37aSequenceA(void)
{
    u32 i;
    s32 record;

    if (Value0(Func_02002608) != 0) {
        record = Value1(Func_02002a16, 0x201);
        if (record != 0) {
            goto L_020000f0;
        }
        Func_02002a3e();
        Call2(Func_02002b2e, 0x2051cc, 1);
        Func_02002b44(20);
        Call1(Func_02002a3a, 0x201);
        Call1(Func_02002a4a, 0x200);
        Call1(Func_02002a50, 0x202);
        if (Value1(Func_02002a46, 0x80a) == 0) {
            Func_02000a8e();
        }
        if (Value0(Func_0200264e) != 0) {
            if (Value1(Func_02002a5c, 0x811) == 0) {
                Func_02001c98();
            }
        }
        Func_02002a90();
    } else {
        if (Value1(Func_02002a72, 0x200) == 0) {
            Func_02002a9a();
            Call2(Func_02002b8c, 0x10000, 1);
            Func_02002ba2(20);
            Call1(Func_02002a9a, 0x200);
            Call1(Func_02002aa8, 0x201);
            Call1(Func_02002aae, 0x202);
            Func_02002aca();
        }
    }
    L_020000f0:;
}
