#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_02000d24

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 16 calls, 0 loop(s), and 4 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02001fa2();
void Func_02001fa8();
void Func_02001fbe();
void *Func_02001fca();
void Func_02001fd2();
void *Func_02001fd6();
void Func_02002026();
void Func_02002042();
void Func_0200204a();
void *Func_0200206e();
void Func_02002070();
void *Func_0200207a();
void Func_020020ce();
void Func_020020d4();
void Func_02002180();
void Func_0200218c();

void Func_02000d24(void)
{
    void *p1;
    void *p10;
    void *p2;
    void *p9;

    p1 = Func_02001fca(14);
    *(u8 *)((u8 *)(p1) + 35) = 2;
    p2 = Func_02001fd6(14);
    *(u8 *)((u8 *)(p2) + 85) = 3;
    Func_02002026(14, 0, 0, 3);
    Func_02001fa2(16, 44, 1, 1, 15, 44);
    Func_020020d4(100, 0, 0);
    Func_02001fbe(12, 71, 1, 1, 127, 127);
    Func_02001fd2(11, 71, 1, 1, 12, 71);
    Func_02001fa8(33590501);
    p9 = Func_0200206e(14);
    *(u8 *)((u8 *)(p9) + 35) = 2;
    p10 = Func_0200207a(14);
    *(u8 *)((u8 *)(p10) + 85) = 0;
    Func_020020ce(14, 16252928, 46661632);
    Func_0200204a(31, 95, 1, 1, 15, 44);
    Func_02002180(100, -1, -1);
    Func_0200218c();
    Func_02002070(127, 127, 1, 1, 12, 71);
    Func_02002042(33590501, 3200);
}
