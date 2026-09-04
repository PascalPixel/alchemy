#include "types.h"

#define FieldScene_RunScene3b1_02000728 Func_02000728

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02004fae();
void Func_02004fc8();
s32 Func_02006bd2();
void Func_02006bf4();
s32 Func_02006c0c();
s32 Func_02006c26();
s32 Func_02006c3e();
s32 Func_02006c48();
void Func_02006c9e();
void Func_02006cb6();
void Func_02006cc4();
void Func_02006ce4();
void Func_02006d1e();
void Func_02006d36();
void Func_02006d38();
void Func_02006d50();
void Func_02006d6a();
void Func_02006d82();

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

void FieldScene_RunScene3b1_02000728(void)
{
    u32 i;
    s32 record;

    Func_02006bf4();
    if (Value1(Func_02006bd2, 0x928) != 0) {
        Call1(Func_02006ce4, 0x1eb2);
        Func_02004fae(8);
        Call3(Func_02006d1e, 8, 0xd000, 60);
        Func_02006cb6(8, 4);
        Func_02004fc8(8);
        Func_02006cc4(8, 3);
    } else {
        if (Value1(Func_02006c0c, 0x925) != 0) {
            Call1(Func_02006d1e, 0x1e06);
            Func_02006d36(8, 0);
        } else {
            if (Value1(Func_02006c26, 0x921) != 0) {
                Call1(Func_02006d38, 0x1dcd);
                Func_02006d50(8, 0);
                record = Value1(Func_02006c3e, 0x925);
                if (record != 0) {
                    goto L_020007cc;
                }
                record = Value1(Func_02006c48, 0x924);
                if (record == 0) {
                    goto L_020007cc;
                }
                {
                    u16 *target = (u16 *)((*(u8 *volatile *)Data_03001ebc + 0x172));
                    s32 shown = 1;
                
                    *target = shown;
                }
            } else {
                Call1(Func_02006d6a, 0x1d30);
                Func_02006d82(8, 0);
            }
        }
    }
    L_020007cc:;
    Func_02006c9e();
}
