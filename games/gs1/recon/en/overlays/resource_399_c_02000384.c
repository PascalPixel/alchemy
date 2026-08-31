#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000384

/* Audited retained supplemental scene body.
 * The complete production span preserves 41 calls, 0 loop(s), and 3 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

s32 Func_0200259a();
void Func_020025b6();
void Func_020025c2();
void Func_020025ca();
s32 Func_020025cc();
void Func_020025d6();
void Func_020025ec();
void Func_020025f4();
void Func_02002630();
void Func_02002632();
void *Func_0200264e();
void Func_0200266c();
void *Func_0200267c();
void Func_02002684();
void Func_02002690();
void Func_0200269a();
void Func_020026a4();
void Func_020026a8();
void Func_020026b2();
void Func_020026b4();
void Func_020026bc();
void Func_020026c8();
void Func_020026dc();
void Func_020026e8();
void Func_020026ec();
void *Func_02002708();
void Func_0200270c();
void Func_02002718();
void Func_0200271a();
void Func_0200272a();
void Func_02002744();
void Func_02002754();
void Func_02002760();
void Func_02002762();
void Func_02002764();
void Func_02002782();
void Func_020027ae();
void Func_020027c2();
void Func_020027c6();
void Func_020027ca();

void Func_02000384(void)
{
    void *p18;
    void *p3;
    void *p39;

    if (Func_0200259a(2177) != 0) {
        Func_020025c2();
        p3 = Func_0200267c(9, 0, 0);
        Func_020025ca(10);
        Func_020026a8(5700);
        Func_020026c8(9, 0);
        Func_020025ec();
    } else {
        if (Func_020025cc(2091) != 0) {
            Func_020025f4();
            Func_02002684(9, 7);
            Func_020025b6(33599936, 10, 69);
            Func_020026dc(5484);
            Func_020026ec(9, 0);
            Func_020026a4(9, 8);
            Func_020025d6(33599958, 10, 69);
            Func_02002632();
        } else {
            Func_02002630();
            p18 = Func_0200264e(9);
            *(u16 *)((u8 *)(p18) + 100) = 10;
            Func_0200266c(9, 33596660);
            Func_0200271a(5428);
            Func_0200272a(9, 0);
            Func_02002690(8);
            Func_02002764(8, 256, 40);
            Func_02002760(8, 53248, 10);
            Func_02002718(8, 2);
            Func_02002762(8, 0, 20);
            Func_020026b2(0, 33596772);
            Func_020026b4(8, 104857, 52428);
            Func_020026dc(8, 33596680);
            Func_0200269a(40);
            Func_02002744(8, 2, 0);
            Func_02002754(8, 2);
            Func_020027c6(8, 258);
            Func_020026bc(60);
            Func_020027ae(8, 0, 10);
            Func_020027ca(8, 12288, 20);
            Func_02002782(8, 2);
            Func_020027c2(8, 0);
            p39 = Func_02002708(8);
            *(u8 *)((u8 *)(p39) + 89) ^= 0x2;
            Func_020026e8();
            Func_0200270c();
        }
    }
}
