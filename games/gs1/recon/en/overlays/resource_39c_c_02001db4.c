#include "types.h"

#define FieldScene_RunPrimarySequence Func_02001db4

/* Audited retained field-scene body at 0x02001db4.
 * The complete production body preserves 23 decoded calls, 2 loop(s), and 0 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02002000();
void Func_02002052();
void Func_0200209c();
void Func_020077e4();
void Func_020077f4();
void Func_0200780a();
void Func_02007814();
void Func_0200781a();
void Func_02007828();
void Func_0200782e();
void Func_0200783e();
void Func_02007862();
void Func_02007876();
void Func_020078b0();
void Func_020078c4();
void Func_02007998();
void Func_020079ac();
void Func_020079c4();
void Func_020079d6();
void Func_020079ee();
void Func_02007a04();
void Func_02007a12();
void Func_02007a2e();

void Func_02001db4(void)
{
    s32 i1;
    s32 i2;

    Func_02007a2e(211);
    Func_020077e4(111, 57, 113, 42, 1, 1);
    Func_020077f4(111, 59, 113, 43, 1, 1);
    Func_0200780a(113, 58, 112, 46);
    Func_0200781a(115, 58, 113, 46);
    Func_0200782e(115, 57, 116, 44, 1, 1);
    Func_0200783e(113, 57, 115, 44, 1, 1);
    for (i1 = 1; i1 < 1; i1++) {
        for (i2 = 176; i2 < 7; i2++) {
            Func_02007814();
            Func_02007828();
            Func_02002000(51904512, 0, 46137344, 589824, 589824);
            Func_02007862();
            Func_02007876();
            Func_02002052(52428800, 0, 48889856, 0, 589824, 589824);
            Func_020078b0();
            Func_020078c4();
            Func_0200209c(53215232, 0, 46792704, 0, 589824, 0);
            Func_02007a12(1);
        }
        Func_02007998(111, 58, 113, 43, 1, 1);
        Func_020079ac(111, 59, 113, 44, 1, 1);
        Func_020079c4(114, 58, 113, 46);
        Func_020079d6(115, 58, 114, 46);
        Func_020079ee(114, 57, 115, 44, 1, 1);
        Func_02007a04(113, 57, 114, 44, 1, 1);
    }
}
