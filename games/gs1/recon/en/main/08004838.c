#include "video_dma_family.h"

#define PaletteDma_LoadBlock Func_08004838

void PaletteDma_LoadBlock(void)
{
    const void *src = (const void *)0x0800779c;
    void *dst = (void *)0x05000200;
    u32 ctrl = 0x800000e0;
    struct DmaChannel *dma = (struct DmaChannel *)0x040000d4;

    /*
     * Reconstruction note: candidate=32 reference=32 (lengths match),
     * differing_halfwords=4. The reference's DMA3 setup loads the base
     * address and all three field values from the literal pool into
     * r3/r0/r1/r2 (exactly as this candidate does), then writes them out
     * with a single "stmia r3!, {r0, r1, r2}" / "subs r3, #12" instead of
     * three discrete str's, followed by a "movs r0, r0" alignment nop
     * before the literal pool (the reference's 7-halfword body is odd,
     * so the assembler pads it to the required 4-byte pool alignment;
     * this candidate's 8-halfword body is already aligned and needs no
     * such pad). This is the same DMA3 "lone stmia" residual already
     * isolated and documented for the sibling owners Func_0800300c
     * (0800300c.c) and the save_state_family.h group (080058ac.c,
     * 08005b64.c, 08005c68.c, 08005d10.c): reading this route's actual
     * backend (alchemy-gcc/gs1cc/gcc/config/arm/arm.c,
     * thumb_output_move_mem_multiple / thumb_expand_movstrqi, and
     * arm.md's movmem12b/movmem8b and store_multiple patterns) confirms
     * Thumb has exactly one machine pattern that ever prints a multi-
     * register "stmia": movmem12b/movmem8b, and thumb_output_move_mem_multiple
     * always emits its paired "ldmia ...; stmia ...;" as one indivisible
     * output_asm_insn pair for a genuine memory-to-memory block move --
     * it can never represent a lone grouped store of three independently
     * computed register values. The dedicated ARM store-multiple expander
     * (arm.md "store_multiple", ~line 4997) and its peephole users are
     * gated TARGET_ARM only, with no Thumb equivalent or peephole/combine
     * substitute for merging separate consecutive str's into stmia.
     *
     * Confirmed empirically here too: a struct-copy through a local
     * struct DmaChannel temporary reaches the paired movmem12b shape
     * (ldmia+stmia through a stack-materialized 12-byte object, plus a
     * "sub sp"/"add sp" this reference does not have); a static inline
     * helper returning "struct DmaChannel" by value is not returned in
     * registers (12 bytes exceeds this ABI's register-return size) and
     * likewise lowers through the same stack-materialized paired
     * ldmia/stmia. An exhaustive PERM_GENERAL search of all 6 field-
     * write statement orders (out/permute/08004838/report.json) confirms
     * this exact order is the unique floor at differing_halfwords=4;
     * every other order regresses to 6 by additionally scrambling which
     * hard register (r0/r1/r2) holds which field value relative to the
     * reference. A volatile-qualified dma pointer (closest legitimate
     * shape ruling out any store reordering) was also tried and does not
     * change the floor. No ordinary C shape on this compiler route
     * reaches the reference's standalone, unpaired grouped stmia; the
     * per-field-store shape below is the closest ordinary-C rendering of
     * the real algorithm.
     */
    dma->control = ctrl;
    dma->destination = dst;
    dma->source = src;
}
