#include "types.h"

#define FieldScene_RunScene3a4SequenceG Func_02002be0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_0200665a();
s32 Func_02006682();
s32 Func_0200668a();
s32 Func_020066b2();
s32 Func_020066d4();
s32 Func_020066ec();
s32 Func_02006724();
s32 Func_0200673c();
void Func_02006758();
void Func_02006768();
void Func_02006794();
void Func_02006950();
void Func_02006958();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3a4SequenceG(void)
{
    u32 i;
    u8 *rec7;
    s32 record;
    s32 value;
    s32 base6_3001e40;

    base6_3001e40 = 0x3001e40;
    if (Value2(Func_0200665a, *(volatile s32 *)base6_3001e40, 3) == 0) {
        value = Value0(Func_02006682);
        rec7 = Value4(Func_020066ec, 200, ((((u32)(((value << 1) + value) << 4) >> 16) << 16) + 0x2fd0000), -0x400000, 0x2600000);
        if ((s32)rec7 != 0) {
            if (Value2(Func_0200668a, *(volatile s32 *)base6_3001e40, 9) == 0) {
                {
                    s32 v2 = Func_020066b2();
                    if (((u32)(v2 << 1) >> 16) != 0) {
                        Func_02006950(145);
                    } else {
                        Func_02006958(144);
                    }
                }
            }
            rec7[85] = 0;
            {
                s32 v3 = Func_020066d4();
                s32 tmp2824 = (((u32)(v3 << 15) >> 16) + 0x4ccc);
                *(volatile s32 *)((s32)rec7 + 72) = 0x6666;
                *(s32 *)((s32)rec7 + 28) = tmp2824;
                *(s32 *)((s32)rec7 + 24) = tmp2824;
            }
            rec7[97] = 1;
            Func_02006794((s32)rec7, 0);
            rec7[35] &= 254;
            {
                u8 *p80 = (u8 *)(*(volatile s32 *)((s32)rec7 + 80));
                s32 mask9 = -13;
                p80[9] = (mask9 & p80[9]) | 4;
            }
            Func_02006758((s32)rec7, 1);
            Call2(Func_02006768, (s32)rec7, 0x200c01c);
            value = Value0(Func_02006724);
            *(volatile s32 *)((s32)rec7 + 36) = ((((u32)(((value << 1) + value) << 1) >> 16) - 3) << 16);
            *(volatile s32 *)((s32)rec7 + 40) = 0x80000;
            value = Value0(Func_0200673c);
            *(volatile s32 *)((s32)rec7 + 44) = (((u32)(((value << 1) + value) << 9) >> 16) + -0x300);
        }
    }
}
