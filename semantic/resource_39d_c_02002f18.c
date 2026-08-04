#include "types.h"

/* Ordinary C field access retained from the reviewed disassembly dataflow. */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/*
 * resource_39d owner 0x02002f18..0x0200305f (0x148 = 328 bytes).
 *
 * Reviewed 32-iteration alternating spawn loop. The translated C preserves
 * the two record paths, fixed-point rounding, stack parameter block, local
 * eight-argument spawner calls, counter rollover, and final record cleanup.
 * Pool words 0x00006666, 0xfff40000 and 0xfff00000 are retained as literals;
 * the high-register unwind is at 0x02003042..0x02003050.
 */

struct SpawnParams { s32 field00; s32 field04; u8 pad08[0x10]; s16 field18; u8 pad1a[2]; s32 field1c; };

extern void Func_0200013c(
    s32 x, s32 y, s32 arg2, s32 arg3,
    s32 arg4, s32 arg5, u32 flags, struct SpawnParams *parameters);
extern s32 Func_080000f8(void);
extern s32 Func_080091e0();
extern void Func_0808a010();
extern u8 *Func_0808a080();
extern void Func_0808a0f0();
extern void Func_0808a158();
extern void Func_080f9010();

void Func_02002f18(void) {
    struct SpawnParams sp10;
    s32 temp_r5;
    s32 temp_r5_2;
    s32 temp_r6;
    u32 var_r8;
    void *temp_r9;
    void *temp_sl;

    temp_r9 = Func_0808a080(0x16);
    temp_sl = Func_0808a080(0x18);
    Func_080f9010(0xBE);
    Func_0808a158(0x16, 0x100);
    Func_0808a158(0x18, 0x100);
    Func_080091e0(Func_0808a080(0x16), 0);
    Func_080091e0(Func_0808a080(0x18), 0);
    FIELD(&sp10, s32 *, 0) = 1;
    FIELD(&sp10, s32 *, 4) = 5;
    FIELD(&sp10, s16 *, 0x18) = 0x11C;
    FIELD(&sp10, s32 *, 8) = (s32) 0x00006666;
    FIELD(&sp10, s32 *, 0xC) = 0x30000;
    var_r8 = 0;
    do {
        Func_0808a010(1);
        temp_r6 = 1 & var_r8;
        if (temp_r6 != 0) {
            temp_r5 = FIELD(temp_r9, s32 *, 8) + (((u32) (Func_080000f8() * 0x18) >> 0x10) << 0x10) + 0xfff40000;
            Func_0200013c(temp_r5, FIELD(temp_r9, s32 *, 0xC) + (((u32) (Func_080000f8() << 5) >> 0x10) << 0x10) + 0xfff00000, FIELD(temp_r9, s32 *, 0x10), 0, 0x40000, 0, 0x1B0000, &sp10);
        } else {
            temp_r5_2 = FIELD(temp_sl, s32 *, 8) + (((u32) ((s32) Func_080000f8() * 0x18) >> 0x10) << 0x10) + 0xfff40000;
            Func_0200013c(temp_r5_2, FIELD(temp_sl, s32 *, 0xC) + (((u32) (Func_080000f8() << 5) >> 0x10) << 0x10) + 0xfff00000, FIELD(temp_sl, s32 *, 0x10), 0, 0x40000, temp_r6, 0x1B0000, &sp10);
        }
        var_r8 += 1;
    } while (var_r8 <= 0x1FU);
    Func_0808a0f0(0x16, 0, 0);
    Func_0808a0f0(0x18, 0, 0);
}
