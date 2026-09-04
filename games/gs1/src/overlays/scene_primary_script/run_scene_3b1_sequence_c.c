#include "types.h"

#define FieldScene_RunScene3b1SequenceC Func_02001894

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02007d50();
s32 Func_02007d5a();
s32 Func_02007d78();
s32 Func_02007d82();
s32 Func_02007d8c();
void Func_02007d9a();
s32 Func_02007d9e();
s32 Func_02007db0();
s32 Func_02007dc8();
s32 Func_02007dd2();
void Func_02007e1c();
void Func_02007e8a();
void Func_02007e9e();
void Func_02007eb0();
void Func_02007ec2();
void Func_02007eca();
void Func_02007ef6();
void Func_02007f00();
void Func_02007f1c();
void Func_02007f24();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b1SequenceC(void)
{
    u32 i;
    s32 record;

    record = Func_02007d82(0);
    if ((u32)((*(volatile u16 *)(record + 6) + -0x2000) << 16) > -0x40000000) {
        if (Value1(Func_02007d50, 0x928) != 0) {
            if (Value1(Func_02007d5a, 0x93e) == 0) {
                Func_02007f1c(17);
                goto L_0200194e;
            }
        }
        Func_02007f24(15);
    } else {
        Func_02007d9a();
        if (Value1(Func_02007d78, 0x93e) != 0) {
            Call1(Func_02007e8a, 0x1f81);
        } else {
            if (Value1(Func_02007d8c, 0x8a0) != 0) {
                Call1(Func_02007e9e, 0x1f48);
            } else {
                if (Value1(Func_02007d9e, 0x928) != 0) {
                    Call1(Func_02007eb0, 0x1f7f);
                } else {
                    if (Value1(Func_02007db0, 0x925) != 0) {
                        Call1(Func_02007ec2, 0x1f7d);
                    } else {
                        Call1(Func_02007eca, 0x1f7b);
                    }
                }
            }
        }
        if (Value1(Func_02007dc8, 0x928) != 0) {
            record = Value1(Func_02007dd2, 0x93e);
            if (record != 0) {
                goto L_02001942;
            }
            Func_02007ef6(17, 0);
        } else {
            L_02001942:;
            Func_02007f00(15, 0);
        }
        Func_02007e1c();
    }
    L_0200194e:;
}
