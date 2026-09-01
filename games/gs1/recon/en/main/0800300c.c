#include "types.h"

/*
 * Startup interrupt/DMA init routine. Ordering and register-window
 * evidence:
 *   - REG_IME/REG_IE/REG_DISPSTAT/REG_KEYCNT addresses and values come
 *     straight from the literal pool in games/gs1/asm/0800300c.s.
 *   - The first DMA (source 0x08000770 -> dest 0x03000000, control
 *     0x84000500 = enabled|32-bit|0x500 words = 5120 bytes) matches the
 *     "relocated_iwram_payload" group's 5120-byte ROM->IWRAM copy
 *     recorded in games/gs1/asm/classification.json.
 *   - The second DMA (source 0x08007320 -> dest 0x030000e0, control
 *     0x8400000e = enabled|32-bit|0x0e words = 56 bytes) matches the
 *     "iwram_dispatch_pointers_08007320" table copy recorded in
 *     games/gs1/asm/executable_gaps/index.json (14 handler pointers,
 *     defaulting to Func_08003008/reserved_no_op_f, landing at
 *     run address 0x030000e0).
 *   - REG_IE = 0x1001 enables VBlank (bit0) + Keypad (bit12) IRQs, and
 *     REG_KEYCNT = 0xc3ff arms the keypad IRQ condition.
 *
 * Compiler-backend note (see compiler_backend_evidence in
 * games/gs1/recon/en/dossiers.json#main:08003e10, and independently confirmed
 * here by reading alchemy-gcc/agscc/gcc/config/arm/arm.md and arm.c):
 * the reference emits two grouped `stmia r3!, {r0, r1, r2}` stores for
 * the two DMA descriptor writes, each fed by three *independently*
 * loaded registers (no preceding ldmia). On this Thumb/GCC 2.96 route
 * the only machine pattern that ever emits literal "stmia" text with
 * more than one register is movmem12b/movmem8b (thumb_expand_movstrqi,
 * arm.md ~5111-5155; thumb_output_move_mem_multiple, arm.c ~9425-9469),
 * and that pattern is a single indivisible RTL insn that *always*
 * emits a paired ldmia immediately before the stmia (both lines come
 * from the same "case 3:"/"case 2:" branch in
 * thumb_output_move_mem_multiple) -- i.e. it can only represent a
 * genuine memory-to-memory struct copy, never a lone grouped store of
 * three independently computed register values. The dedicated
 * store-multiple machinery (arm.md "store_multiple" expander and the
 * *stmsi / *stmsi_postinc patterns, arm.md ~4997-5075) is gated
 * "TARGET_ARM" only, with no Thumb equivalent. So no ordinary C shape
 * (field-by-field volatile stores, an aggregate/compound-literal
 * assignment, or sequential post-increment pointer stores) reaches the
 * reference's standalone grouped stmia on this compiler route -- this
 * mirrors, structurally, the same conclusion already recorded for
 * Runtime_CopyAndCallRoutine's DMA setup in 08003e10.json. Tried and
 * measured here: plain per-field volatile struct stores (closest
 * legitimate shape, kept below); a compile-time-constant compound
 * literal, which *does* trigger movmem12b -- confirmed empirically by
 * the resulting paired `ldmia r3!, {..}` / `stmia r2!, {..}` -- but
 * that is the paired shape, not the reference's unpaired one, and it
 * also pulls in an extra callee-saved register (r7) instead of
 * matching the reference's r5/r6 push; a local three-word descriptor
 * copied via assignment; and raw `u32 *`/post-increment pointer
 * writes (`*p++ = x`), which independently confirms Thumb needs a
 * grouped stmia purely to synthesize a post-incrementing SImode store
 * (no true post-indexed str), but each store gets its own single-
 * register stmia rather than merging into one three-register form.
 * None reproduce the reference's exact grouped, unpaired stmia; the
 * per-field-store shape below is the closest ordinary-C rendering of
 * the real algorithm.
 */

#define REG_IME (*(volatile u16 *)0x04000208)
#define REG_IE (*(volatile u16 *)0x04000200)
#define REG_DISPSTAT (*(volatile u16 *)0x04000004)
#define REG_KEYCNT (*(volatile u16 *)0x04000132)

struct DmaRegisters
{
    u32 source;
    u32 destination;
    u32 control;
};

#define DMA3 ((volatile struct DmaRegisters *)0x040000d4)

extern void *Data_03007ffc;

void Func_0800300c(void)
{
    REG_IME = 0;

    DMA3->source = 0x08000770;
    DMA3->destination = 0x03000000;
    DMA3->control = 0x84000500;

    Data_03007ffc = (void *)0x03000000;

    DMA3->source = 0x08007320;
    DMA3->destination = 0x030000e0;
    DMA3->control = 0x8400000e;

    REG_DISPSTAT = 0;
    REG_KEYCNT = 0xc3ff;
    REG_IE = 0x1001;
    REG_IME = 1;
}
