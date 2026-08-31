#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_020009c6

/* Audited retained supplemental scene body.
 * The complete production span preserves 22 calls, 1 loop(s), and 14 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02000b9a();
void Func_0200579c();
void Func_020057e6();
void Func_02005812();
void Func_02005816();
void Func_02005870();
void *Func_02005882();
void Func_020058a8();
void *Func_020058b8();
void Func_02005906();
void Func_02005914();
void Func_02005920();
void Func_02005972();
void Func_0200597e();
void Func_0200599e();
void Func_020059d6();
void Func_020059ea();
void Func_02005a0e();
void Func_02005a1e();
void Func_02005a42();
void Func_02005a8e();
void Func_02005ac6();

void Func_020009c6(void)
{
    void *p1;
    void *p10;
    s32 i1;

    p1 = Func_02005882(0);
    Func_02005870();
    Func_02005972(-1, -1, -1, 0);
    Func_020057e6();
    Func_0200579c(1);
    *(s32 *)(p1 + 12) = 8519680;
    *(s32 *)(p1 + 72) = 32768;
    *(s32 *)(p1 + 68) = 0;
    *(u8 *)((u8 *)(p1) + 85) = 0;
    Func_020059d6();
    Func_020059ea();
    Func_020058a8(30);
    Func_02005a1e(204);
    *(u8 *)((u8 *)(p1) + 85) = 3;
    p10 = Func_020058b8(24);
    *(s32 *)(p10 + 4) = 7;
    *(s32 *)(p10 + 36) = 33589613;
    *(s32 *)(p10 + 8) = 52428;
    *(s32 *)(p10 + 12) = 52428;
    for (i1 = 7; i1 < 16; i1++) {
        Func_02005812();
        Func_02005816();
        Func_02000b9a(17367041);
    }
    Func_02005a8e(188);
    Func_02005a0e(0, 257);
    Func_0200599e(0, 22);
    Func_02005906(327680, 327680, 65536);
    Func_02005914(-1, -1);
    Func_02005920();
    Func_02005a42(0, 256);
    Func_02005ac6();
    *(s32 *)(p1 + 72) = 65536;
    *(s32 *)(p1 + 68) = 16384;
    Func_0200597e();
}
