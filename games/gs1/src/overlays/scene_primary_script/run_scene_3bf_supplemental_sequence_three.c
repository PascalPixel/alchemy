#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020019a4();
s32 Func_020062f2();
s32 Func_02006316();
void Func_0200631a();
void Func_02006324();
void Func_02006356();
void Func_020064c2();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunSupplementalSequenceThree(void)
{
    s32 v6;
    s32 raw;
    s32 d;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    v6 = 1;
    if (Func_020062f2(234) != -v6) {
        raw = *(s16 *)(((s32)p5 + 0x16c));
        d = raw - 40;
        if (Value1(Func_02006316, 0x941) != 0) {
            if (d == 4) {
                goto L_02000d40;
            }
        }
        Func_020019a4(d);
        Func_020064c2(157);
        Call3(Func_0200631a, 0x30000, 0x30000, 0x10000);
        Call3(Func_02006324, -v6, -v6, 0xe666);
        Func_02006356(raw + 0x328);
    }
    L_02000d40:;
}
