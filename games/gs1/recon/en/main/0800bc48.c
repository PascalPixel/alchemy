#include "types.h"

typedef struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
} DmaTransfer;

#define ResourceMetadata_ClearRecord Func_0800bc48

/*
 * Reconstruction note: candidate=40 reference=40 (lengths match),
 * differing_halfwords=11. The reference builds the three DmaTransfer
 * fields directly into r0/r1/r2 (destination moved out of r0 into r1
 * up front via "adds r1, r0, #0" so r0 is free for "mov r0, sp", then
 * r2 loaded from the literal pool) and writes them with a single
 * unpaired "stmia r3!, {r0, r1, r2}" / "subs r3, #12" instead of three
 * discrete str's. This is the same DMA3/DmaTransfer "lone stmia"
 * residual already isolated and documented in depth for the sibling
 * owners Func_0800300c (0800300c.c), PaletteDma_LoadBlock (08004838.c),
 * and the save_state_family.h group (080058ac.c, 08005b64.c,
 * 08005c68.c, 08005d10.c): reading this route's actual backend
 * (alchemy-gcc/gs1cc/gcc/config/arm/arm.c,
 * thumb_output_move_mem_multiple / thumb_expand_movstrqi, and arm.md's
 * movmem12b/movmem8b and store_multiple patterns) confirms Thumb has
 * exactly one machine pattern that ever prints a multi-register
 * "stmia": movmem12b/movmem8b, and thumb_output_move_mem_multiple
 * always emits its paired "ldmia ...; stmia ...;" as one indivisible
 * output_asm_insn pair for a genuine memory-to-memory block move -- it
 * can never represent a lone grouped store of three independently
 * computed register values. The dedicated ARM store-multiple expander
 * (arm.md "store_multiple") and its peephole users are gated
 * TARGET_ARM only, with no Thumb equivalent or peephole/combine
 * substitute for merging separate consecutive str's into stmia.
 *
 * Tried and rejected here (all land on differing_halfwords=11, an
 * exhaustive PERM_GENERAL over all 6 field-write orders confirms 11 is
 * the floor -- out/permute/0800bc48_new/report.json): all 6 field-
 * write statement orders (every order ties at 11, unlike 08004838
 * where order mattered -- here the extra/missing instruction count
 * always balances out the same way); a `*dma = (DmaTransfer){...}`
 * compound-literal assignment and an equivalent named-local struct
 * temporary struct-copied to *dma (both land the fields in different,
 * still non-ascending registers, worse alignment, same floor); a
 * volatile-qualified dma pointer (closest legitimate shape ruling out
 * store reordering, matches sibling files' finding -- no change); a
 * static (non-inline) helper returning DmaTransfer by value, which
 * *does* reach a lone "stmia r3!, {r0,r1,r2}" matching the reference's
 * trailing instruction exactly, but only after round-tripping the
 * returned struct through its own caller-allocated stack slot via a
 * real `bl` call (candidate grows to 48 bytes); the same helper marked
 * `static inline` still lowers through the ABI's hidden-pointer
 * struct-return convention even when inlined, materializing two stack
 * temporaries instead of one (candidate grows to 52 bytes, worse). No
 * ordinary C shape on this compiler route reaches the reference's
 * standalone, unpaired grouped stmia; the per-field-store shape below
 * is the closest ordinary-C rendering of the real algorithm.
 */
void ResourceMetadata_ClearRecord(void *destination)
{
    if (destination != 0) {
        u32 clear_value = 0;
        const void *source = &clear_value;
        u32 control = 0x85000006;
        DmaTransfer *dma = (DmaTransfer *)0x040000d4;

        dma->control = control;
        dma->destination = destination;
        dma->source = source;
    }
}
