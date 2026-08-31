#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02003766

/* Audited retained supplemental scene body.
 * The complete production span preserves 24 calls, 0 loop(s), and 23 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_020037ae();
void Func_02007056();
void Func_0200709e();
s32 Func_02007386();
void *Func_0200738a();
void Func_02007392();
void Func_0200739e();
s32 Func_020073a4();
void Func_020073b0();
void Func_020073c4();
s32 Func_020073cc();
void Func_020073ea();
s32 Func_020073ec();
void Func_020073f8();
void *Func_020073fc();
void Func_0200740c();
void Func_0200744c();
void Func_0200745c();
void Func_0200746e();
void Func_02007540();
void Func_0200755e();
void Func_02007566();
void Func_02007588();
void Func_020075a6();

void Func_02003766(void)
{
    void *p1;
    void *p18;

    p1 = Func_0200738a(8);
    Func_02007392(9);
    Func_020037ae();
    Func_020073b0(0);
    *(u8 *)((*(void **)(p1 + 80)) + 9) &= 0xfffffffffffffff3;
    if (Func_02007386(770) == 0) {
        if (Func_020073a4(514) == 0) {
            Func_02007540(-1);
            Func_0200755e(230);
            Func_020073c4(514);
        }
        Func_02007056();
    }
    if (Func_020073cc(771) == 0) {
        if (Func_020073ec(515) == 0) {
            Func_02007588(-1);
            Func_020075a6(230);
            Func_0200740c(515);
        }
        Func_0200709e();
    }
    Func_0200739e();
    p18 = Func_020073fc(222);
    *(u8 *)((u8 *)(p18) + 85) = 0;
    *(u8 *)((*(void **)(p18 + 80)) + 9) &= 0x0;
    Func_02007566();
    Func_0200746e();
    Func_020073ea();
    *(s32 *)(p18 + 36) = 0;
    Func_020073f8();
    *(s32 *)(p18 + 40) = 0;
    *(u16 *)((u8 *)(p18) + 100) = 20;
    *(u8 *)((u8 *)(p18) + 100) = 1;
    Func_0200744c();
    Func_0200745c();
}
