#include "types.h"

/* Ordinary C field access retained from the reviewed disassembly dataflow. */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/*
 * resource_39d owner 0x02002ddc..0x02002eb7 (0xdc = 220 bytes).
 *
 * Reviewed helper: updates the scene-0xb2 workspace slot, configures record 9,
 * applies the branch-independent record counters, and finishes the six-argument
 * draw call before the interworking return at 0x02002ea2.
 */

















extern void Func_020064c4();
extern void Func_0200646e();
extern void Func_0200642c();
extern u8 * Func_020064f2();
extern void Func_0200653e();
extern u8 * Func_0200650c();
extern u8 * Func_02006516();
extern u8 * Func_020065ec();
extern void Func_020065e0();
extern void Func_020065fa();
extern void Func_02006606();
extern void Func_02006524();
extern void Func_020064e6();
extern void Func_020065fe();
extern void Func_0200619e();
extern void Func_02006612();
extern void Func_0200661a();
extern void Func_02006560();
extern void Func_0200654e();
extern void Func_0200657a();
void Func_02002ddc(void) {
    s32 temp_r5_2;
    void *temp_r5;

    temp_r5 = *(u8 **)0x03001e70 + 0x164;
    Func_020064c4();
    FIELD(temp_r5, s32 *, 0xC) = 0x03800000;
    Func_0200646e();
    Func_0200642c(1);
    FIELD(Func_020064f2(9), s8 *, 0x55) = 0;
    Func_0200653e(9, 0x680000, 0x01080000);
    FIELD(Func_0200650c(9), s32 *, 0xC) = temp_r5_2;
    FIELD(Func_02006516(9), s32 *, 0x3C) = temp_r5_2;
    temp_r5_2 = 0xffe00000;
    FIELD(Func_020065ec(), s8 *, 0x55) = 0;
    Func_020065e0(0x0000cccc, 0x00001999);
    Func_020065fa(0x800000, -1, 0xB80000, 1);
    Func_02006606();
    Func_02006524(0x1E);
    Func_020064e6(0x1D, 0x4A, 4, 0x4A, 5, 4);
    Func_020065fe(0x11, 0);
    Func_02006606(0x12, 0);
    Func_0200619e();
    Func_02006612(0x11, 1);
    Func_0200661a(0x12, 1);
    Func_02006560(0x14);
    Func_0200654e(0x00000251);
    Func_0200657a();
}
