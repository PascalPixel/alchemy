#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000368

/* Audited retained supplemental scene body.
 * The complete production span preserves 32 calls, 0 loop(s), and 2 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02000aba();
s32 Func_02001500();
void Func_0200157a();
void *Func_0200158e();
void Func_02001598();
void Func_020015a2();
void Func_020015ac();
void Func_020015b6();
void Func_020015c0();
s32 Func_020015c4();
void Func_020015ca();
void Func_020015d4();
s32 Func_020015de();
void Func_020015e8();
void Func_020015f2();
void Func_020015fc();
void Func_02001606();
s32 Func_0200160c();
void Func_02001610();
void Func_0200161a();
void Func_02001620();
void *Func_0200162c();
void Func_02001664();
void Func_0200169e();
void Func_020016ba();
void Func_020016ce();
void Func_020016ec();
void Func_02001706();
void Func_0200171a();

void Func_02000368(void)
{
    void *p2;
    void *p26;

    if (Func_02001500(2100) != 0) {
        p2 = Func_0200158e(8, 0, 0);
        Func_02001598(9, 0, 0);
        Func_020015a2(10, 0, 0);
        Func_020015ac(11, 0, 0);
        Func_020015b6(12, 0, 0);
        Func_020015c0(13, 0, 0);
        Func_020015ca(14, 0, 0);
        Func_020015d4(15, 0, 0);
        Func_020015de(16, 0, 0);
        Func_020015e8(17, 0, 0);
        Func_020015f2(18, 0, 0);
        Func_020015fc(19, 0, 0);
        Func_02001606(20, 0, 0);
        Func_02001610(21, 0, 0);
        Func_0200161a(22, 0, 0);
        Func_020016ce();
        Func_020016ec();
        Func_0200157a(30);
        Func_02001706();
        Func_0200171a();
        Func_02001706();
    }
    if (Func_020015c4(2170) != 0) {
        if (Func_020015de(2077) == 0) {
            Func_02000aba();
        }
        p26 = Func_0200162c(10);
        *(u8 *)((u8 *)(p26) + 89) |= 0x80;
    }
    if (Func_0200160c() != 0) {
        Func_0200169e(13, 29753344, 9830400);
        Func_02001664(13);
        Func_0200161a();
        Func_020016ba(13, 5);
        Func_02001620(4);
    }
}
