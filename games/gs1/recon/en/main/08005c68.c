#include "save_state_family.h"

s32 Func_08005c68(void)
{
    struct SaveWorkspace *work;
    u8 *summary;
    u32 zero;
    u32 group;
    s32 count;

    work = SAVE_WORKSPACE;
    summary = work->summary[0];
    count = 0;
    group = 0;
    do {
        u32 index;

        zero = 0;
        START_DMA(&zero, summary, 0x85000010);
        index = Func_08005b24(group);
        if (index <= 15) {
            ReadFlash((u16)index, 0, summary, 64);
            count++;
        }
        index = Func_08005b24(group + 3);
        if (index <= 15)
            ReadFlash((u16)index, 0x110, summary + 56, 4);
        else
            *(u32 *)(summary + 56) = zero;
        group++;
        summary += 64;
    } while (group <= 2);
    return count;
}

/*
 * Reconstruction note: candidate=144 reference=144 (lengths match),
 * differing_halfwords=54, matched_prefix=10. The reference's per-iteration
 * DMA3 setup is a single "ldr r3,=DMA3; adds r0,r7,#0 / adds r1,r5,#0 /
 * ldr r2,=ctl; stmia r3!,{r0,r1,r2}; subs r3,#12" -- source/destination/
 * control built directly into r0-r2 with no memory round trip, a transient
 * (non-hoisted) DMA3 pointer, and a persistent r7 dedicated to &zero (with
 * a persistent sl holding the constant 0, reused both for the per-iteration
 * "zero = 0" store and for the else-branch "*(summary+56) = zero" store).
 * This candidate's plain field-by-field DMA3->field = value writes (via the
 * shared save_state_family.h START_DMA macro) instead land the DMA3 base
 * address in a persistent r7 (loop-invariant-hoisted, since the macro's three
 * identical "(volatile struct DmaRegisters *)0x040000d4" casts CSE to one
 * loop-invariant address), forcing three separate str instructions instead
 * of one stmia, and leaves the register that should hold &zero (sl) without
 * a spare slot for the reusable 0 constant, which is instead rematerialized
 * with a fresh "movs r3,#0" each iteration.
 *
 * This is the same open residual already isolated and documented on two
 * sibling save_state_family.h owners: SaveState_ReadSlotAndCheckChecksum
 * (080058ac.c) and Func_08005b64 (08005b64.c). Both record an exhaustive
 * bounded search -- all 6 field-write orders, a static/static-inline
 * StartDma3_-style helper routed through the macro, a non-volatile second
 * DMA3 pointer type, a named local DmaRegisters temporary struct-copied to
 * *DMA3, and a struct compound-literal/brace-initializer RHS -- and every
 * one either fails to reach stmia at all, breaks the reference's frame size
 * (the extra spill inflates "sub sp" past what the reference emits), or (for
 * the compound-literal/brace-initializer form) lowers through an unsupported
 * memcpy call in this environment, never an inline block move. Both
 * conclude the shared START_DMA macro itself would need to be reshaped to
 * reach the reference's stmia lowering, which is out of scope for a single
 * owner's file.
 *
 * A bounded PERM_GENERAL search over all 6 DMA3 field-write orders on this
 * owner (same technique, applied fresh here since this owner sits inside a
 * do-while loop rather than the siblings' straight-line code, so the
 * register-pressure context differs) confirms the same floor: every order
 * still holds at candidate=144 differing_halfwords=54 or worse (best=54,
 * tied by two of the six orders; the other four regress to 55 or, when they
 * also disturb declaration order enough to add a spill, 73 with a length
 * mismatch). A local static/static-inline StartDma3_-style helper tried
 * directly on this owner (matching 080058ac.c's mitigation) makes the
 * residual worse here (72, with a length mismatch) rather than better,
 * because this owner already has group/count/&zero competing for the same
 * two spare callee-saved registers (r7, sl) that the helper's call-site
 * argument shuffle needs. differing_halfwords=54 is the floor reached for
 * this owner without editing save_state_family.h.
 */
