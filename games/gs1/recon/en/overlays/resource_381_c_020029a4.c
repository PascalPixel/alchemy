#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_020029a4

/* Audited retained supplemental scene body.
 * The complete production span preserves 28 calls, 3 loop(s), and 7 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02005e66();
void Func_02005e92();
void Func_02005ea4();
void *Func_02005ea6();
void Func_02005eae();
void Func_02005eb8();
void Func_02005ec0();
void Func_02005ec2();
void Func_02005ecc();
void Func_02005ed6();
void Func_02005ee0();
void Func_02005eea();
void Func_02005ef4();
void Func_02005efe();
void Func_02005f08();
void Func_02005f30();
void Func_02005f38();
void Func_02005f4e();
void Func_02005fcc();
void Func_02005fd6();
void Func_02005fe4();
void Func_0200601a();
void Func_02006048();
void Func_02006054();
void Func_020060e4();
void Func_02006150();
void Func_0200615e();

void Func_020029a4(void)
{
    void *p6;
    s32 i1;
    s32 i2;
    s32 i3;

    for (i1 = 0; i1 < 15; i1++) {
        Func_02005eae(16);
    }
    Func_02005fcc(4213326, 1);
    Func_02005fd6(4209234, 1);
    Func_02005fe4(60);
    Func_0200601a(214);
    for (i2 = 0; i2 < 9; i2++) {
        p6 = Func_02005ea6(284);
        *(u8 *)((u8 *)(p6) + 85) = 0;
        *(u8 *)((u8 *)(*(void **)(p6 + 80)) + 38) = 0;
        *(u8 *)((*(void **)(p6 + 80)) + 9) |= 0x4;
        Func_02005ec0();
        Func_02005e66(6);
    }
    Func_02006048(0, 256, 0);
    Func_02006054(1, 256, 0);
    Func_02005e92(20);
    Func_02005ea4(33598369, 3200);
    Func_020060e4(246);
    Func_02005eae(6);
    Func_02005eb8(6);
    Func_02005ec2(6);
    Func_02005ecc(6);
    Func_02005ed6(6);
    Func_02005ee0(6);
    Func_02005eea(6);
    Func_02005ef4(6);
    Func_02005efe(6);
    Func_02005f08(6);
    for (i3 = 4; i3 < 0; i3++) {
    }
    Func_02005f30(1);
    Func_02005f38(40);
    Func_02005f4e(33598369);
    Func_02006150(65536, 1);
    Func_0200615e(40);
}
