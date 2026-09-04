#include "types.h"

#define FieldScene_RunScene377_02000e34 Func_02000e34

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02002748();
void Func_02002762();
s32 Func_02002766();
void Func_02002788();
s32 Func_020027a0();
void Func_020027e6();
void Func_02002804();
void Func_02002816();
void Func_02002848();
void Func_0200285e();
void Func_02002860();
void Func_02002882();
s32 Func_02002886();
void Func_0200288a();
void Func_02002898();
void Func_020028a2();
void Func_020028f4();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene377_02000e34(void)
{
    u32 i;
    s32 record;

    record = Func_02002766(0);
    if ((u32)(*(volatile u16 *)(record + 6) + -0x2000) > 0x9000) {
        Func_020028f4(0, 13);
    } else {
        Func_02002762();
        if (Value1(Func_02002748, 0x87a) != 0) {
            Func_02002804(13, 2);
            Func_02002816(13, 0, 10);
            if (Value1(Func_02002766, 0x300) == 0) {
                Call1(Func_02002848, 0x1c14);
                Func_02002860(13, 0);
                Call1(Func_02002788, 0x300);
            }
            Call1(Func_0200285e, 0x1c15);
            Value2(Func_02002886, 13, 0);
            Func_02002898(13, 0x9000, 10);
        } else {
            if (Value1(Func_020027a0, 0x815) != 0) {
                Call1(Func_02002882, 0x11a9);
            } else {
                Call1(Func_0200288a, 0xf58);
            }
            Func_020028a2(13, 0);
        }
        Func_020027e6();
    }
}
