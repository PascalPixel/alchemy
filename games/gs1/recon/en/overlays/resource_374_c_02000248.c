#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_02000248

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 33 calls, 2 loop(s), and 0 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_020025fa();
void Func_02002858();
s32 Func_020028c6();
s32 Func_020028d8();
void Func_020028dc();
void Func_020028e8();
s32 Func_020028f2();
void Func_020028f6();
s32 Func_0200290c();
void Func_02002950();
void Func_0200295a();
s32 Func_0200295e();
void Func_02002976();
void Func_02002984();
void Func_020029c2();
void Func_020029e2();
void Func_02002a06();
void Func_02002a0a();
void Func_02002a10();
void Func_02002a18();
void Func_02002a1e();
void Func_02002a20();
void Func_02002a2c();
void Func_02002a2e();
void Func_02002a30();
void Func_02002a60();
void Func_02002ac4();
void Func_02002ae0();
void Func_02002af8();
void Func_02002b6a();
void Func_02002ba4();

void Func_02000248(void)
{
    s32 base;
    s32 i1;
    s32 i2;

    Func_020028e8();
    if (Func_020028c6(2069) != 0) {
        base = 4503;
        Func_020029c2(base);
        if (Func_020028d8(2) != 0) {
        }
        if (Func_020028f2(3) != 0) {
        }
        Func_02002a06(17, 0);
        if (Func_0200295e(0, 0) == 0) {
            Func_02002a10(base + 3);
        } else {
            Func_02002a18(base + 4);
        }
        Func_02002a30(17, 0);
    } else {
        Func_02002a2e(3912);
        Func_02002a20(17, 0, 0);
        Func_02002a60(17, 0);
        Func_02002976(20);
        Func_02002a1e(17, 2);
        Func_02002984(15);
        Func_02002858();
        for (i1 = 0; i1 < 39; i1++) {
            Func_020029c2(17);
            Func_020025fa();
            Func_020028f6(1);
        }
        Func_0200290c(33596817, 3200);
        Func_02002b6a(107);
        for (i2 = 0; i2 != 180; i2++) {
            if (Func_0200290c(0, 10) == 0) {
            }
            Func_020029e2(1);
        }
        Func_02002ba4(289);
        Func_0200295a(33596817);
        Func_02002950(1);
        Func_020028dc();
        Func_02002ac4(17, 0);
        Func_02002a0a(40);
        Func_02002ae0();
        Func_02002af8(17, 0);
    }
    Func_02002a2c();
}
