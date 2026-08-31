#include "types.h"

#define FieldScene_RunPrimarySequence Func_020011c4

/* Audited retained field-scene body at 0x020011c4.
 * The complete production body preserves 26 decoded calls, 0 loop(s), and 13 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02001386();
s32 Func_02001630();
s32 Func_02001662();
void Func_02003c3e();
void Func_02003c52();
void Func_02003c5c();
void Func_02003c66();
void Func_02003cee();
void Func_02003d20();
void Func_02003d5e();
void Func_02003d68();
void Func_02003d92();
void *Func_02003d94();
void Func_02003da0();
void Func_02003da6();
void Func_02003de2();
s32 Func_02003dea();
void Func_02003e08();
void Func_02003e14();
void Func_02003e22();
void Func_02003e32();
void Func_02003e3c();
void Func_02003e5a();
void Func_02003ea4();
void Func_02003eaa();
void Func_02003eee();

void Func_020011c4(void)
{
    void *p7;

    Func_02003cee(0);
    Func_02003c3e();
    Func_02003c52();
    Func_02003c66();
    Func_02003c5c();
    Func_02001386(589825);
    p7 = Func_02003d94(0);
    Func_02003d20(1048576, 4293918720);
    Func_02003da0();
    if (Func_02001630() == 0) {
        Func_02003d5e(2097152, 524288);
        if (Func_02001662() == 0) {
            if (Func_02003dea() == 0) {
                Func_02003e32();
                Func_02003d92();
                Func_02003d68(6);
                Func_02003eee(152);
                Func_02003da6();
                *(s32 *)(p7 + 48) = 196608;
                *(s32 *)(p7 + 52) = 131072;
                *(s32 *)(p7 + 40) = 262144;
                Func_02003e3c();
                Func_02003eaa(0);
                Func_02003de2();
                Func_02003e5a();
                Func_02003ea4();
            } else {
            }
        }
    }
    Func_02003e08();
    Func_02003e14();
    Func_02003e22();
}
