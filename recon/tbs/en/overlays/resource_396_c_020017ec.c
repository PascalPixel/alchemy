/* NONMATCHING kuupuappu H1 (2026-09-27): explicit byte offset removes
 * the strength-reduced logical index but emits 96/96, 15 halfwords/15 edits.
 * Offset zero moves before the sentinel compare, and signed loads exchange
 * their base/offset roles. Size still precedes sentinel retention. Full
 * normalized diff and normal/diagnostic text agree. Reject this model;
 * checkpoint its body, then restore flat indices for one preheader test.
 * Previous NONMATCHING: 96 bytes, candidate 96, 3 differing halfwords (2026-09-27).
 * Complete entry 020017ec through the pool ending at 0200184c; the caller
 * is ENTER_ROOM.C. The candidate unit now records both import veneers.
 * H1: six s16 fields in an indexed MapPatch record produced 112 bytes,
 * 55 halfwords / 39 edits: an extra +4 induction and saved sl register.
 * H2: s16 rows[][6] produced 108 bytes, 53 halfwords / 31 edits: separate
 * flag and enabled inductions survive. Neither is the reference's one
 * shared offset. H3: FIELD_EVENT.H's Map_CopyCellsTo boundary is identical
 * to the flat-array baseline (96 bytes, 3 halfwords / 2 aligned edits).
 * Typed-row and call-boundary axes stopped; keep the complete flat model.
 * Remaining:
 * the preheader order: the reference sets the -1 (r8), the offset 0 (r5) and
 * then size 1 (r7); here the hoisted size comes first. The loop dump shows
 * size as the first movable; for, while, goto and condition placements of
 * size did not move it after the strength-reduced offset.
 * Fresh inversion audit (2026-09-27): complete 96-byte score remains
 * 3 halfwords / 2 aligned edits; ordinary and -da assembly are identical.
 * Loop pass first moves size pseudo33, insn35 -> 203; next moves the -1
 * producer to209, then creates shared byte-offset pseudo100 at215 when
 * eliminating index32. Global allocation preserves this 1/-1/0 order as
 * r7/r8/r5. Thus the divergence predates the final scheduler and call reloads.
 * The whole loop after its entry and the table pool are exact. Caller and
 * FIELD_EVENT.H agree on six word arguments, with both sizes on the stack.
 * No new source boundary was established; no repeated loop/type/wrapper
 * trial was run, and no DONE credit is claimed.
 * 2026-09-27 phased-value transfer from exact SHIAN_MURA/SETUP.C: use
 * size for the -1 sentinel test, then 1 for each copy, then -1 again.
 * Complete output is 92/96 bytes, 47 halfwords / 19 aligned edits. The
 * sentinel now lives in call-clobbered r4 and is rebuilt each iteration;
 * r8 and its save/restore disappear, violating the reference lifetime.
 * Full diff read. Reject and stop this phase-reuse axis; keep 96-byte
 * canonical and its three-halfword preheader residual. DONE +0. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Each patch stores flag, enabled, source x/y and destination x/y. */
extern s16 Data_02009ca8[];

/* Copy the one-cell patch of each enabled entry whose flag is set. */
void ToretoHeya_ApplyFlaggedMapPatches(void)
{
    s32 i;
    s32 size;

    for (i = 0; Data_02009ca8[i] != -1; i += 6) {
        size = 1;
        if (Engine_GameFlagIsSet(Data_02009ca8[i]) && Data_02009ca8[i + 1] != 0)
            Map_CopyCellsTo(Data_02009ca8[i + 2], Data_02009ca8[i + 3],
                Data_02009ca8[i + 4], Data_02009ca8[i + 5], size, size);
    }
}
