#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_02000108

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 59 calls, 0 loop(s), and 1 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

s32 Func_020044d0();
void Func_020044e4();
void Func_020044f2();
void Func_02004504();
void Func_02004510();
void Func_0200453c();
void Func_0200456c();
void Func_0200456e();
void Func_0200459c();
void Func_020045b4();
void Func_020045c4();
void Func_020045e0();
void Func_020045f8();
s32 Func_020045fc();
void Func_02004604();
void Func_02004612();
s32 Func_02004614();
void Func_02004626();
void Func_0200462c();
void Func_02004630();
void Func_02004636();
void Func_0200463c();
void Func_0200464a();
void Func_02004664();
void Func_02004680();
void Func_02004684();
void Func_0200468c();
void Func_02004696();
void Func_0200469c();
void Func_020046a6();
void Func_020046b6();
s32 Func_020046bc();
void Func_020046be();
void Func_020046c2();
void Func_020046ce();
void Func_020046d0();
void Func_020046d2();
void Func_020046da();
void Func_0200470e();
void Func_02004718();
void Func_02004740();
void Func_0200474c();
void Func_02004756();
void Func_0200475a();
void Func_0200475c();
void Func_0200476c();
void Func_02004774();
void Func_02004776();
void Func_02004780();
void Func_0200478c();
void Func_020047bc();
void Func_020047cc();
void Func_02004842();
void Func_0200484c();

void Func_02000108(void)
{
    s32 base;

    Func_02004510(0);
    Func_02004504();
    Func_02004630();
    if (Func_020044d0(512) == 0) {
        Func_020044e4(512);
        Func_020044f2(2409);
        Func_020045e0(8183);
        Func_020045f8();
        Func_0200453c(10);
        Func_020045b4(0, 40, 104);
        Func_02004636(0, 0, 0);
        Func_0200459c();
        Func_020046d0();
        Func_020046da();
        Func_02004664();
    } else {
        Func_0200456e(512);
        Func_0200456c(2409);
        Func_02004680();
        Func_02004612(0, 120, 96);
        Func_02004696(0, 49152, 0);
        Func_020045c4(20);
        base = 8184;
        Func_0200468c(base);
        Func_0200469c();
        if (Func_020045fc(0, 0) == 0) {
            Func_020046a6(base + 1);
            Func_020046be();
        } else {
            Func_020046b6(base + 2);
            Func_020046ce();
        }
        Func_02004604(10);
        Func_0200469c();
        Func_02004612(20);
        Func_02004776();
        Func_02004780();
    }
    Func_0200463c();
    Func_0200464a();
    Func_02004776();
    if (Func_02004614(2406) == 0) {
        Func_02004626(2406);
        Func_0200462c(2407);
        Func_02004740();
        Func_020046d2(0, 120, 96);
        Func_02004756(0, 49152, 0);
        Func_02004684(20);
        base = 8769;
        Func_0200474c(base);
        Func_0200475c();
        if (Func_020046bc(0, 0) == 0) {
            Func_020046a6(10);
            Func_0200476c(base + 1);
        } else {
            Func_02004774(base + 2);
        }
        Func_0200478c();
        Func_020046c2(10);
        Func_0200475a();
        Func_020046d0(20);
        Func_0200470e();
        Func_02004842();
        Func_0200484c();
    } else {
        Func_020047bc();
        Func_020047cc();
    }
    Func_02004718();
}
