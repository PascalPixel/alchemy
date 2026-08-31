#include "types.h"

#define FieldScene_RunSupplementalSequenceTwo Func_02001550

/* Audited retained supplemental scene body.
 * The complete production span preserves 31 calls, 1 loop(s), and 6 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02001f74();
void Func_020020de();
void Func_020045cc();
void Func_020045ee();
void Func_02004600();
void *Func_0200464e();
void Func_0200465c();
void Func_0200466c();
void Func_0200469e();
void *Func_020046a0();
void Func_020046aa();
void *Func_020046ba();
void Func_020046e0();
void Func_020046f8();
void Func_02004700();
void *Func_02004726();
void Func_02004730();
void Func_02004736();
void Func_02004744();
void Func_0200474c();
void Func_0200476e();
void Func_02004770();
void Func_0200477a();
void Func_0200477c();
void Func_02004788();
void Func_0200478e();
void Func_020047ac();
void Func_020047b6();
void Func_020047d0();
void Func_02004804();
void Func_02004840();

void Func_02001550(void)
{
    void *p1;
    void *p14;
    void *p5;
    void *p8;
    s32 i1;

    p1 = Func_0200464e();
    Func_0200466c(18);
    Func_0200465c(30);
    Func_02001f74(48758784, 0, 12058624, 253);
    p5 = Func_020046a0(18);
    *(u8 *)((u8 *)(p5) + 85) = 0;
    Func_0200474c(185);
    for (i1 = 0; i1 < 15; i1++) {
        Func_020045cc(3);
        p8 = Func_020046ba(18);
        *(s32 *)(p8 + 12) = *(s32 *)(p8 + 12);
        Func_020045ee();
        Func_02004600();
        Func_020020de(0, 0, 589824);
    }
    Func_020046aa(51, 8, 1, 1, 49, 8);
    Func_02004700(30);
    p14 = Func_02004726(18);
    *(u8 *)((u8 *)(p14) + 35) |= 0x2;
    Func_02004770(18, 3);
    Func_0200469e();
    Func_020046e0(45, 4, 1, 1, 46, 8);
    Func_0200478e(20, 48758784, 8912896);
    Func_02004804(188);
    Func_020046f8(58, 8, 49, 8, 1, 3);
    Func_02004736(0, 327680, 65536);
    Func_02004744(-1, -1, 58982);
    Func_0200477a(20);
    Func_02004840(188);
    Func_02004730(59, 8, 49, 8, 1, 3);
    Func_0200476e(0, 327680, 65536);
    Func_0200477c(-1, -1, 58982);
    Func_02004788();
    Func_020047b6(10);
    Func_020047ac();
    Func_020047d0();
}
