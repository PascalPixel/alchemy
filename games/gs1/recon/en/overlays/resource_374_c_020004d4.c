#include "types.h"

#define FieldScene_RunMiddleAuxiliarySequence Func_020004d4

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 28 calls, 2 loop(s), and 5 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_020027c8();
void Func_020027ec();
void Func_02002824();
void Func_02002a84();
void Func_02002b20();
void Func_02002b2e();
void Func_02002b36();
void Func_02002b40();
void Func_02002b6c();
void *Func_02002b78();
void Func_02002b86();
void Func_02002b8a();
void Func_02002b90();
void Func_02002bb0();
void Func_02002bec();
void *Func_02002c22();
void Func_02002c3e();
void Func_02002c4a();
void Func_02002c5c();
void Func_02002c74();
void Func_02002cba();
void Func_02002ce4();
void Func_02002cfc();
void Func_02002d04();
void Func_02002d10();
void Func_02002d40();
void Func_02002dec();

void Func_020004d4(void)
{
    void *p1;
    void *p16;
    s32 i1;
    s32 i2;

    p1 = Func_02002b78();
    Func_02002bec(0, 82, 760);
    Func_02002c3e(15, 0, 30);
    Func_02002c5c(3758);
    Func_020027c8(15, 20);
    Func_020027ec(15, 40960, 20);
    Func_02002cba(15, 258);
    Func_02002bb0(20);
    Func_02002a84();
    for (i1 = 0; i1 < 39; i1++) {
        Func_02002bec(15);
        Func_02002824();
        Func_02002b20(1);
    }
    Func_02002b36(33596801, 3200);
    Func_02002b40(33596833, 3200);
    Func_02002ce4(0, 40960, 10);
    p16 = Func_02002c22(20);
    *(u8 *)((u8 *)(p16) + 85) = 0;
    for (i2 = 0; i2 < 39; i2++) {
        *(s32 *)(p16 + 12) = *(s32 *)(p16 + 12);
        Func_02002b6c(1);
    }
    *(u8 *)((u8 *)(p16) + 85) = 0;
    Func_02002b8a(33596801);
    Func_02002b90(33596833);
    Func_02002b86(1);
    Func_02002dec(161);
    Func_02002cfc(15, 0);
    Func_02002d04(20, 0);
    Func_02002c4a(40);
    Func_02002b2e();
    Func_02002d10(0, 15, 30);
    Func_02002d40(15, 0);
    Func_02002c74();
}
