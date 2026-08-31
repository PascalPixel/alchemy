#include "types.h"

#define FieldScene_RunSupplementalSequenceThree Func_02000b6c

/* Audited retained supplemental scene body.
 * The complete production span preserves 36 calls, 1 loop(s), and 5 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02002532();
void *Func_02002554();
s32 Func_0200257e();
void Func_020025c2();
void *Func_020025c6();
void Func_020025c8();
s32 Func_020025d6();
void Func_020025e0();
void Func_020025e6();
void Func_0200260c();
void Func_0200261c();
void Func_0200262c();
void *Func_02002636();
void Func_02002638();
void Func_02002644();
void Func_0200264c();
void Func_02002660();
void Func_02002662();
void Func_02002676();
void Func_0200268a();
void *Func_02002690();
void Func_02002692();
void Func_020026a2();
void Func_020026aa();
void Func_020026b8();
void Func_020026c4();
void Func_020026c6();
void Func_020026d2();
void Func_020026da();
void Func_020026e6();
void Func_02002712();
void Func_0200279e();
void Func_020027ba();
void Func_020027c6();
void Func_020027ca();
void Func_020027e0();

void Func_02000b6c(void)
{
    void *p1;
    void *p17;
    void *p25;
    void *p6;
    s32 i1;

    p1 = Func_02002554();
    for (i1 = 8; i1 < 65; i1++) {
        if (Func_0200257e(8) != 0) {
        }
    }
    Func_020026c4(158);
    Func_02002532();
    Func_020025c8(0, 32768, 16384);
    p6 = Func_020025c6(0);
    *(u8 *)((u8 *)(p6) + 85) = 0;
    Func_0200261c(0, 2);
    Func_0200260c(0, 2, -8);
    Func_020025c2(10);
    Func_020026aa();
    Func_020026c6();
    Func_020026d2();
    Func_020025e6();
    if (Func_020025d6(514) != 0) {
        Func_020025e0(513);
        Func_02002638(0, 13107, 6553);
        p17 = Func_02002636(0);
        *(u8 *)((u8 *)(p17) + 85) = 0;
        Func_0200268a(0, 2);
        Func_02002676(0, 2, -8);
        Func_0200262c(13);
        Func_02002712(12);
    }
    Func_02002644(578);
    Func_02002660();
    Func_02002692(0);
    p25 = Func_02002690(0);
    *(u8 *)((u8 *)(p25) + 85) = 0;
    Func_020026e6(0, 2);
    Func_020027ca(0, 0, -8);
    Func_020027e0(158);
    Func_0200264c(53, 4, 2, 2, 41, 4);
    Func_020026a2(10);
    Func_02002662(53, 6, 2, 2, 41, 4);
    Func_020026b8(10);
    Func_0200279e(1);
    Func_020027ba();
    Func_020027c6();
    Func_020026da();
}
