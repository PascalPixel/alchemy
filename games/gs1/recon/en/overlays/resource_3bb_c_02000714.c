#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000714

/* Audited retained supplemental scene body.
 * The complete production span preserves 36 calls, 1 loop(s), and 11 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_0200074c();
void Func_02000754();
void Func_02000766();
void Func_0200076e();
void Func_020008d2();
void Func_02004650();
s32 Func_0200465c();
void Func_02004666();
void *Func_02004674();
void Func_02004678();
void *Func_0200467e();
void Func_02004692();
void Func_020046a6();
void Func_020046ac();
void Func_020046ba();
void Func_020046ce();
void Func_020046ec();
void Func_02004700();
void Func_02004714();
void Func_02004728();
void Func_0200475c();
void Func_02004770();
void Func_02004784();
void Func_02004798();
s32 Func_020047b2();
void Func_020047b6();
void Func_020047ca();
void Func_020047de();
void Func_020047f2();
void Func_020047f6();
void Func_02004850();
void Func_0200489a();
void Func_020048a8();
void Func_020048b6();
void Func_020048c4();
void Func_020048d4();

void Func_02000714(void)
{
    void *p1;
    void *p2;
    s32 i1;

    p1 = Func_02004674();
    for (i1 = 22; i1 <= 25; i1++) {
        p2 = Func_0200467e(22);
        *(u8 *)((u8 *)(p2) + 91) = 0;
        Func_0200074c();
        Func_02000754();
        Func_02000766();
        Func_0200076e();
        if (Func_0200465c(260) != 0) {
        } else {
        }
        *(s32 *)(p1 + 16) = *(s32 *)(p1 + 16);
    }
    Func_02004650(58, 10, 1, 1, 58, 11);
    Func_02004666(57, 11, 1, 1, 58, 11);
    Func_02004678(58, 14, 7, 1, 58, 13);
    Func_0200475c(22);
    Func_02004692();
    Func_02004770(23);
    Func_020046a6();
    Func_02004784(24);
    Func_020046ba();
    Func_02004798(25);
    Func_020046ce();
    Func_020047f6(31, 11);
    Func_020047b6(22);
    Func_020046ec();
    Func_020047ca(23);
    Func_02004700();
    Func_020047de(24);
    Func_02004714();
    Func_020047f2(25);
    Func_02004728();
    Func_02004850(31, 10);
    Func_020008d2();
    if (Func_020047b2(260) == 0) {
    }
    Func_020046ac(33589013);
    Func_0200489a(22, 61341696, 14155776);
    Func_020048a8(23, 63438848, 14155776);
    Func_020048b6(24, 65536000, 14155776);
    Func_020048c4(25, 67633152, 14155776);
    Func_020048d4(31, 10);
}
