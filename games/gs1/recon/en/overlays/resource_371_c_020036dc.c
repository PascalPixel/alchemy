#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_020036dc

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 15 calls, 0 loop(s), and 6 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_020078d6();
void Func_02007986();
s32 Func_0200798c();
void Func_020079e6();
void *Func_020079f2();
void Func_02007a16();
s32 Func_02007a1c();
void Func_02007a38();
void Func_02007a5e();
void Func_02007a74();
void Func_02007a7e();
void Func_02007a8c();
void Func_02007abc();
void Func_02007aee();
void Func_02007be2();

void Func_020036dc(void)
{
    void *p1;

    p1 = Func_020079f2();
    if (Func_0200798c(752) == 0) {
        Func_020079e6();
        Func_02007aee();
        Func_02007a7e();
        if (Func_02007a1c() != 0) {
            Func_02007a5e();
        }
        Func_02007a8c();
        Func_02007be2(244);
        Func_020078d6();
        *(u8 *)((u8 *)(p1) + 85) = 0;
        Func_02007986();
        Func_02007abc();
        *(s32 *)(p1 + 40) = 0;
        *(u8 *)((u8 *)(p1) + 85) = 4;
        Func_02007a16(752);
        Func_02007a38(760, 180);
        Func_02007a74();
    }
}
