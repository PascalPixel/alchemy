#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02001742

/* Audited retained supplemental scene body.
 * The complete production span preserves 32 calls, 1 loop(s), and 6 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02002172();
void Func_020022dc();
void Func_020047ca();
void Func_020047ec();
void Func_020047fe();
s32 Func_02004838();
void *Func_02004842();
void Func_0200485c();
void Func_02004860();
void Func_0200489c();
void *Func_0200489e();
void Func_020048a8();
void *Func_020048b8();
void Func_020048de();
void Func_020048f6();
void Func_020048fe();
void *Func_02004924();
void Func_0200492e();
void Func_02004934();
void Func_02004942();
void Func_0200494a();
void Func_0200496c();
void Func_0200496e();
void Func_02004978();
void Func_0200497a();
void Func_02004986();
void Func_0200498c();
void Func_020049aa();
void Func_020049b4();
void Func_020049ce();
void Func_02004a02();
void Func_02004a3e();

void Func_02001742(void)
{
    void *p1;
    void *p15;
    void *p6;
    void *p9;
    s32 i1;

    p1 = Func_02004842();
    Func_02004860(19);
    if (Func_02004838(514) == 0) {
    } else {
        Func_0200485c(30);
        Func_02002172(50462720, 0, 17956864, 223);
        p6 = Func_0200489e(19);
        *(u8 *)((u8 *)(p6) + 85) = 0;
        Func_0200494a(185);
        for (i1 = 0; i1 < 15; i1++) {
            Func_020047ca(3);
            p9 = Func_020048b8(19);
            *(s32 *)(p9 + 12) = *(s32 *)(p9 + 12);
            Func_020047ec();
            Func_020047fe();
            Func_020022dc(0, 0, 589824);
        }
        Func_020048a8(51, 8, 1, 1, 45, 14);
        Func_020048fe(30);
        p15 = Func_02004924(19);
        *(u8 *)((u8 *)(p15) + 35) |= 0x2;
        Func_0200496e(19, 3);
        Func_0200489c();
        Func_020048de(45, 4, 1, 1, 48, 14);
        Func_0200498c(21, 50855936, 15204352);
        Func_02004a02(188);
        Func_020048f6(58, 8, 45, 14, 1, 3);
        Func_02004934(0, 327680, 65536);
        Func_02004942(-1, -1, 58982);
        Func_02004978(20);
        Func_02004a3e(188);
        Func_0200492e(59, 8, 45, 14, 1, 3);
        Func_0200496c(0, 327680, 65536);
        Func_0200497a(-1, -1, 58982);
        Func_02004986();
        Func_020049b4(10);
        Func_020049aa();
    }
    Func_020049ce();
}
