#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000804

/* Audited retained supplemental scene body.
 * The complete production span preserves 34 calls, 0 loop(s), and 10 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02000ce2();
void Func_02000d38();
void Func_02000dce();
void Func_02000e04();
s32 Func_020018de();
void *Func_02001900();
void *Func_0200190a();
void Func_02001910();
void Func_0200193c();
void *Func_02001952();
void Func_02001960();
void *Func_0200196a();
void Func_02001970();
void Func_02001978();
void *Func_0200198e();
void Func_02001990();
void Func_020019a2();
void Func_020019bc();
void *Func_020019c0();
void Func_020019e6();
s32 Func_020019f4();
void Func_020019fe();
void Func_02001a06();
void *Func_02001a1e();
void Func_02001a3a();
void *Func_02001a44();
void Func_02001a4a();
void *Func_02001a56();
void Func_02001a64();
void Func_02001a90();
void Func_02001aa6();

void Func_02000804(void)
{
    void *p1;
    void *p10;
    void *p15;
    void *p18;
    void *p2;
    void *p27;
    void *p32;
    void *p34;
    void *p9;

    p1 = Func_0200190a(14);
    *(u16 *)((u8 *)(p1) + 100) |= 0x2;
    p2 = Func_02001900();
    if (Func_020018de(775) != 0) {
        Func_02001990(7360);
        Func_02000ce2(14);
    }
    Func_02000dce();
    Func_02001910(775);
    Func_0200193c();
    p9 = Func_02001952(14);
    *(u16 *)((u8 *)(p9) + 100) = 1;
    p10 = Func_0200196a(15);
    *(u16 *)((u8 *)(p10) + 100) |= 0x2;
    Func_02001960();
    Func_020019e6(7361);
    Func_02000d38(15);
    Func_02001978();
    p15 = Func_0200198e(15);
    *(u16 *)((u8 *)(p15) + 100) = 0;
    Func_02001970(776);
    Func_020019a2();
    p18 = Func_020019c0(16);
    *(u8 *)((u8 *)(p18) + 91) = 1;
    Func_02001a06(16, 1);
    Func_02001a1e(16, 1);
    Func_020019bc(20);
    Func_02001a4a(7349);
    Func_02001a44(16, 0, 2);
    Func_02001a64(16, 0);
    if (Func_020019f4(0, 0) != 0) {
    }
    Func_02001a90(16, 0);
    p27 = Func_02001a1e(16);
    *(u8 *)((u8 *)(p27) + 91) = 0;
    Func_02001a3a(16, 2);
    Func_02001a1e();
    Func_020019fe(776);
    Func_02001aa6();
    p32 = Func_02001a44(16);
    *(u8 *)((u8 *)(p32) + 91) = 1;
    Func_02000e04(16);
    p34 = Func_02001a56(16);
    *(u8 *)((u8 *)(p34) + 91) = 0;
}
