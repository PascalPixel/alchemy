#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000124

/* Audited retained supplemental scene body.
 * The complete production span preserves 25 calls, 0 loop(s), and 2 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

s32 Func_020009a8();
s32 Func_020009da();
s32 Func_020009fc();
s32 Func_02000a16();
void Func_02000a40();
s32 Func_02000a4c();
void Func_02000a4e();
void Func_02000a54();
void Func_02000a58();
void Func_02000a80();
void Func_02000ab6();
void *Func_02000ae6();
void Func_02000b00();
void Func_02000b08();
void Func_02000b10();
void Func_02000b18();
void Func_02000b1e();
void Func_02000b20();
void Func_02000b34();
void Func_02000b36();
void *Func_02000b52();
void Func_02000bc0();
void Func_02000c16();
void Func_02000c46();

void Func_02000124(void)
{
    void *p16;
    void *p18;

    if (Func_020009a8(2207) != 0) {
    }
    if (Func_020009da(2199) != 0) {
        Func_02000a58(10, 0, 0);
    }
    if (Func_020009fc(2299) != 0) {
    }
    if (Func_02000a16(2300) != 0) {
    }
    Func_02000a40(303);
    Func_02000a4e(2299);
    if (Func_02000a4c(2415) == 0) {
        Func_02000a54(6, 0, 2, 1, 8, 27);
    }
    Func_02000a80(2300);
    Func_02000b00(8, 4);
    Func_02000b08(9, 4);
    Func_02000b10(10, 3);
    Func_02000b18(11, 4);
    Func_02000b20(12, 3);
    p16 = Func_02000ae6(15);
    *(s32 *)(p16 + 28) = 104857;
    Func_02000ab6(108, 38, 1, 1, 102, 56);
    p18 = Func_02000b52(0);
    *(u8 *)((u8 *)(p18) + 85) = 0;
    Func_02000c16(158);
    Func_02000b20(66, 36, 71, 8, 2, 2);
    Func_02000b1e(4);
    Func_02000b36(68, 36, 71, 8, 2, 2);
    Func_02000b34(4);
    Func_02000bc0(0, 3, -16);
    Func_02000c46();
}
