#include "types.h"

#define FieldScene_RunPrimarySequence Func_02001750

/* Audited retained field-scene body at 0x02001750.
 * The complete production body preserves 43 decoded calls, 0 loop(s), and 3 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02001894();
void Func_020018b2();
void Func_02002718();
void Func_02002726();
void Func_020029c4();
void Func_02002ae8();
void Func_02002c8e();
void Func_02002f30();
void Func_02002f72();
void Func_02002ff2();
void Func_02003132();
void Func_02003334();
void Func_02003530();
void Func_0200353c();
void Func_02003562();
s32 Func_02003a7e();
void Func_02003aa4();
void Func_02003ab0();
void Func_02003ae4();
void Func_02003b02();
void Func_02003b18();
s32 Func_02003b1a();
void Func_02003b20();
void Func_02003b34();
void Func_02003b7c();
void Func_02003b84();
void *Func_02003bac();
void Func_02003bb6();
void Func_02003bc8();
s32 Func_02003be0();
void Func_02003c02();
void Func_02003c0a();
void Func_02003c60();
void Func_02003c80();
void Func_02003c88();
void Func_02003c94();
void Func_02003cae();
void Func_02003cc0();
void Func_02003cd6();
void Func_02003cec();
void Func_02003d02();
s32 Func_02003d9e();

void Func_02001750(void)
{
    void *p17;

    if (Func_02003a7e(265, 448, 516, 50339964) == 0) {
        Func_02003aa4(324);
        Func_02002f30();
    } else {
        Func_02002f72();
    }
    Func_02003ae4(0);
    Func_02003b02(8);
    Func_02003ab0();
    Func_02003b84();
    Func_02003b20(0, 131072, 6553);
    Func_02003b34(0, 264, 196);
    Func_02003b18();
    Func_02003132();
    if (Func_02003b1a(3859) == 0) {
        Func_02003334();
    }
    Func_02002718(10223616, 0, 29622272, 223);
    Func_02002726(12320768, 0, 29622272, 223);
    p17 = Func_02003bac(8);
    *(u8 *)((u8 *)(p17) + 85) = 0;
    *(s32 *)(p17 + 12) = 0;
    Func_02003c0a(8, 1);
    Func_02003c02(8, 15);
    Func_020018b2();
    Func_02001894();
    Func_02003c88(0);
    Func_02003c94(0);
    if (Func_02003be0(2080) != 0) {
        Func_02003bb6(30, 57, 19, 57, 1, 1);
        Func_02003bc8(30, 8, 12, 8, 8, 7);
    } else {
        Func_02003cc0(2109704, 1, 448, 50339964);
        Func_02003cc0(2109704, 1);
        Func_02003cd6(1);
        Func_02003b7c(1);
    }
    Func_02003c60(8, 29, 15, 5, 8, 42);
    Func_020029c4();
    Func_02003c80(12, 8, 10, 18, 0, 28);
    Func_02002ae8();
    Func_02003cae(12, 21, 9, 16, 12, 3);
    Func_02002c8e();
    Func_02003cec(14, 10, 9, 8, 22, 20);
    Func_02003d02(7, 45, 11, 4, 20, 45);
    Func_02002ff2();
    Func_02003530(0, -32);
    Func_0200353c(-32, 0);
    if (Func_02003d9e() != 0) {
        Func_02003562(0, 32);
    }
}
