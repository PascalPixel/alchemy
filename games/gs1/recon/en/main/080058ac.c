#include "save_state_family.h"

/*
 * Routing START_DMA's three field stores through their own (inlined) helper
 * changes register allocation at the call site relative to writing the
 * three DMA3 fields directly in the caller; see the reconstruction note
 * near the bottom of this file for the residual this still leaves open.
 */
static inline void StartDma3_(u32 source_, u32 destination_, u32 control_)
{
    DMA3->source = source_;
    DMA3->destination = destination_;
    DMA3->control = control_;
}

#undef START_DMA
#define START_DMA(source_, destination_, control_) \
    StartDma3_((u32)(source_), (u32)(destination_), (control_))

s32 Func_080058ac(s32 index)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    u32 checksum;

    work = SAVE_WORKSPACE;
    ReadFlash((u16)index, 0, work->slot.bytes, sizeof(work->slot));
    START_DMA(&work->slot, &header, 0x84000004);
    WAIT_DMA();
    checksum = Func_08005ae0();
    return (u16)checksum - header.checksum;
}

/*
 * Reconstruction note: the reference DMA3 setup here is a single
 * "stmia r3!, {r0,r1,r2}; subs r3,#12" writing source/destination/control
 * built directly into r0-r2 (adds r0,r5,#0 / mov r1,sp / ldr r2,=const),
 * with no memory round trip. The shared START_DMA macro's plain three
 * DMA3->field = value stores never combine into an stmia in this compiler
 * (verified with many equivalent field-assignment shapes, with and without
 * named temporaries, with and without volatile, and with the fields in
 * every order); whole-struct assignment does emit an inline
 * ldmia/stmia pair, but only after first spilling the built struct to its
 * own stack slot (or calling memcpy outright for a compound-literal RHS),
 * which is one extra ldmia stage relative to the reference and grows the
 * frame beyond the observed "sub sp, #16". Routing the three stores through
 * their own inlined helper (StartDma3_ above) reproduces the call-site
 * register-allocation change enough to shrink the residual from the
 * baseline (candidate=84 reference=88 differing_halfwords=36) to
 * candidate=84 reference=88 differing_halfwords=26, but the allocator still
 * lands the three values in r5/r6/r2 rather than r0/r1/r2 and still emits
 * three separate str instructions instead of one stmia, so the 4-byte
 * length gap (one stmia+subs pair vs three str) remains open.
 *
 * Further isolated during this pass: the gap is specifically that the
 * reference recomputes the header address fresh ("mov r3, sp") after the
 * Func_08005ae0 call and reloads a second DMA3 pointer for WAIT_DMA's poll,
 * because its "mov r1, sp" for the destination address lives only in a
 * scratch register consumed by stmia's forced r0/r1/r2 register list and
 * does not survive the call. Every candidate shape tried here that cannot
 * emit stmia (helper call, plain macro stores in the caller in all 6
 * orders, helper-body field order in all 6 orders, pointer-typed helper
 * parameters instead of u32, checksum read routed through a second inlined
 * helper taking a header pointer, checksum stored to a local vs folded
 * directly into the return expression, and local declaration order) is
 * instead free to park the destination address in whatever register is
 * already live across the call, and always lands on differing_halfwords=26
 * at the same 84-vs-88 length gap (a bounded PERM_GENERAL search over the
 * field-store order and the checksum-read shape together confirms 26 is
 * the floor for this family). Closing it looks like it needs the shared
 * save_state_family.h START_DMA macro itself reshaped to reach stmia at
 * all, which is out of scope for this file alone -- the same conclusion
 * independently reached on the sibling save_state_family.h owner in
 * 08005b64.c, which cites this file's residual explicitly.
 */
