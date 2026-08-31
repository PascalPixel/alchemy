#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_020001d0

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 21 calls, 0 loop(s), and 7 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

s32 Func_02001062();
void *Func_0200107c();
void Func_02001094();
void Func_020010e0();
void Func_020010f6();
void Func_020010fc();
void Func_02001114();
void Func_0200112a();
void Func_0200112c();
void Func_02001130();
void Func_0200113c();
void Func_0200114e();
void Func_0200115e();
void Func_02001184();
void Func_0200119e();
void *Func_020011a6();
void Func_020011aa();
void Func_020011b2();
void Func_020011d6();
void Func_020011e8();
void Func_020011f4();

void Func_020001d0(void)
{
    void *p1;
    void *p10;

    p1 = Func_0200107c();
    if (Func_02001062(2313) != 0) {
        Func_02001114(6431);
        Func_0200113c(14, 0);
    } else {
        Func_020010f6(14, 4);
        Func_0200112c(6343);
        Func_0200114e(14, 0, 10);
        Func_02001094();
        Func_020010e0(18);
        p10 = Func_020011a6();
        *(u8 *)((u8 *)(p10) + 85) = 0;
        Func_0200119e(65536, 8192);
        Func_020011b2();
        Func_0200115e(0, 16384, 0);
        Func_020011aa(14, 12288, 0);
        Func_020011d6();
        Func_020010fc(120);
        Func_0200112a(0);
        Func_020011e8();
        Func_020011f4();
        Func_02001184(14, 4);
    }
    Func_02001130();
}
