#include "save_state_family.h"

#undef START_DMA
#define START_DMA(source_, destination_, control_) do { \
    u32 src_ = (u32)(source_); \
    u32 dst_ = (u32)(destination_); \
    u32 ctl_ = (control_); \
    DMA3->control = ctl_; \
    DMA3->destination = dst_; \
    DMA3->source = src_; \
} while (0)

s32 Func_08005b64(s32 index)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    u32 zero;

    work = SAVE_WORKSPACE;
    zero = 0;
    START_DMA(&zero, &header, 0x85000004);
    WAIT_DMA();
    START_DMA(SAVE_HEADER_TEMPLATE, &header, 0x84000002);
    WAIT_DMA();
    header.record_id = 0x10;
    header.sequence = 0;
    START_DMA(&header, &work->slot.record.header, 0x84000004);
    WAIT_DMA();
    if (Func_08005868(index) != 0)
        return 1;
    work->occupied[index] = 0;
    work->record_id[index] = 0x10;
    work->sequence[index] = 0;
    return 0;
}

/*
 * Reconstruction note: the reference emits each of the three DMA3 field
 * writes as a single "ldr r3,=DMA3; stmia r3!, {r0,r1,r2}; subs r3,#12"
 * (source/destination/control built directly into r0-r2, no memory round
 * trip), and reloads a fresh DMA3 pointer from the literal pool for every
 * following WAIT_DMA poll rather than reusing the register still holding
 * that address. This candidate's plain field-by-field DMA3->field = value
 * writes (through this file's local START_DMA override, matching the
 * shared save_state_family.h shape) total the same instruction bytes
 * (candidate=164 reference=164, lengths match) but never combine into an
 * stmia, and freely CSEs the DMA3 pointer register into the following
 * WAIT_DMA instead of reloading it.
 *
 * Tried and rejected: all 6 field-write orderings (best two tie at the
 * differing_halfwords=39 recorded here; a families-transplant-style
 * PERM_GENERAL search over both known-good field orders x every header
 * field-write order x every trailing work-> field-write order, 36
 * combinations, confirms 39 as the floor); all 6 local-declaration orders
 * of work/header/zero; a static-inline helper taking three u32 registers
 * (worse, 44); a non-volatile second DMA3 pointer type for the writes
 * (breaks length, 160 vs 164); a struct-by-value helper relying on GCC's
 * register struct-return convention, which *does* produce stmia at the
 * call site but only after round-tripping the returned struct through its
 * own stack slot (blows the frame past the reference's observed sub sp,
 * #20); a named local DmaRegisters temporary assigned field-by-field then
 * struct-copied to *DMA3 (also spills to its own 12-byte stack slot,
 * growing the frame to sub sp, #32); and a struct compound-literal or
 * brace-initializer assigned to *DMA3 (both lower through an unsupported
 * memcpy call in this environment). This matches the same open residual
 * documented on the sibling save_state_family.h owner
 * SaveState_ReadSlotAndCheckChecksum (080058ac.c): the shared START_DMA
 * shape this compiler accepts does not reach the reference's stmia
 * lowering, and no local-file macro reshaping found here closes it either.
 */
