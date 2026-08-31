#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_020007d8

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 19 calls, 0 loop(s), and 6 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_0200565a();
void Func_02005676();
void Func_02005684();
void *Func_0200568a();
void Func_020056b0();
void Func_020056d8();
void Func_02005702();
void Func_02005712();
void Func_02005720();
void Func_0200575a();
void Func_0200578a();
void Func_02005794();
void Func_020057fa();
void Func_0200580e();
void Func_0200583c();
void Func_0200585a();
void Func_02005874();

void Func_020007d8(void)
{
    void *p1;

    p1 = Func_0200568a(0);
    Func_0200565a(265);
    Func_02005684();
    Func_0200578a(-1, -1, -1, 0);
    *(u8 *)((u8 *)(p1) + 85) = 0;
    Func_02005712(0);
    Func_0200575a(0, 15);
    Func_020056d8(0);
    Func_02005676();
    Func_020057fa();
    Func_0200580e();
    Func_0200583c(228);
    *(s32 *)(p1 + 108) = 33588897;
    Func_02005702(0, 26214);
    Func_0200583c(0, 0, 8);
    Func_02005794(0, 0);
    Func_02005712(0);
    Func_020056b0();
    Func_0200585a(0, 0, 8);
    *(u8 *)((u8 *)(p1) + 85) = 3;
    *(s32 *)(p1 + 108) = 0;
    Func_02005874();
    Func_02005720();
}
