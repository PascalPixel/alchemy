#include "types.h"

#define FieldScene_RunScene39aSequenceC Func_02001948

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000003e[];
extern u8 Data_0000003f[];
extern u8 Data_00000040[];
extern u8 Data_00000041[];
extern u8 Data_02000240[];
void Func_020029c4();
void Func_02002ae8();
void Func_02002c8e();
void Func_02002ff2();
void Func_02003c60();
void Func_02003c80();
void Func_02003cae();
void Func_02003cec();
void Func_02003d02();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunScene39aSequenceC(void)
{
    u8 *base;
    s32 val;

    base = Data_02000240;
    val = *(s16 *)(base + 0x1c0);
    if (val == (s32)Data_0000003e) {
        Call6(Func_02003c60, 8, 29, 15, 5, 8, 42);
        Func_020029c4();
    } else if (val == (s32)Data_0000003f) {
        Call6(Func_02003c80, 12, 8, 10, 18, 0, 28);
        Func_02002ae8();
    } else if (val == (s32)Data_00000040 && *(s16 *)(base + 0x1c2) != 1) {
        Call6(Func_02003cae, 12, 21, 9, 16, 12, 3);
        Func_02002c8e();
    } else if (*(s16 *)(base + 0x1c0) == (s32)Data_00000041) {
        if ((u32)((*(u16 *)(base + 0x1c2) - 1) << 16) <= 0x10000) {
            Call6(Func_02003cec, 14, 10, 9, 8, 22, 20);
        } else {
            Call6(Func_02003d02, 7, 45, 11, 4, 20, 45);
        }
        Func_02002ff2();
    }
}
