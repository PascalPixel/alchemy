#include "types.h"

/*
 * Owners at 0x080069a4 (Func_080069a4, tick handler) and 0x080069c8
 * (Func_080069c8, installer) -- 92 bytes total including both pools,
 * byte-exact under old_agbcc at -O2 (routed via AGBCC_SOURCES; the m4a
 * band's stock-compiler precedent extends here on an independent 92/92
 * proof). Adopted from the semantic draft semantic/main/080069a4.c
 * (same structure; that file's header documents the behavioural read).
 *
 * Exactness notes, all measured against the fork first:
 * - The whole pair is 23 halfwords short of exact under the fork
 *   (gcc296 pushes lr even in these call-free leaves and returns
 *   pop {r0}/bx r0 with interwork, pop {pc} without; the reference has
 *   no prologue at all and returns bare `bx lr`, which old_agbcc emits
 *   for a Thumb leaf).
 * - Every named RAM cell is volatile: the reference re-reads
 *   Data_02004c22 after the zero test (two back-to-back `ldrh` with no
 *   store between), and Func_080069c8 re-reads Data_02004c20 through
 *   `ldrb` immediately after its own `strb` when computing the timer
 *   register address.
 * - The decrement flows through an s32 local narrowed at the store
 *   (`strh`), and the just-reached-zero test is the register copy's
 *   `(v << 16) == 0`, not a re-read -- HANDOVER's "narrow at the store"
 *   rule; a u16 local instead pools a -1 word and re-widens.
 * - The guard arm order is `if (timerIndex > 3) return 1;` first:
 *   old_agbcc then places the return-1 arm after the mid-function pool,
 *   sharing the final `bx lr`, exactly as the reference lays it out.
 *
 * 0x04000100 + timerIndex * 4 walks the four TMxCNT_L hardware timer
 * count/reload registers; Data_02004c28 caches the chosen one.
 */

extern volatile u16 Data_02004c22;
extern volatile u8 Data_02004c24;
extern volatile u8 Data_02004c20;
extern volatile u32 Data_02004c28;

void Func_080069a4(void)
{
    if (Data_02004c22 != 0) {
        s32 v = Data_02004c22;

        v -= 1;
        Data_02004c22 = v;
        if ((v << 16) == 0) {
            Data_02004c24 = 1;
        }
    }
}

s32 Func_080069c8(u8 timerIndex, void (**callback)(void))
{
    if (timerIndex > 3) {
        return 1;
    }
    Data_02004c20 = timerIndex;
    Data_02004c28 = 0x04000100 + (u32)Data_02004c20 * 4;
    *callback = Func_080069a4;
    return 0;
}
