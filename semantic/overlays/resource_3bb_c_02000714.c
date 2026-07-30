typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * resource_3bb owner at 0x02000714, 472 bytes (0x02000714-0x020008eb):
 * 454 bytes of code, two alignment bytes at 0x020008da, and the four-word
 * literal pool at 0x020008dc.
 *
 * Prologue `push {r5, r6, r7, lr} / sub sp, #8` at 0x02000714, epilogue
 * `add sp, #8 / pop {r5, r6, r7} / pop {r0} / bx r0`: r0 holds the popped
 * return address, so the owner is void.  The eight bytes of outgoing stack
 * carry the fifth and sixth arguments of the Func_080091c0 calls.
 *
 * This is the per-frame task the neighbouring owners install: both
 * Func_020008ec and Func_020009b0 pass 0x02008715, which is this entry plus
 * the Thumb bit, and the byte-exact sibling
 * assets/code/resource_3bb_c_02000950.c passes the same constant.  That is
 * also the cheapest witness for the overlay's 0x02008000 link base, under
 * which 0x0200c834 and 0x0200c838 are in-image words at file offsets 0x4834
 * and 0x4838 - the same pair Func_020008ec clears and Func_02000970 polls.
 * 0x02000240 is below the base and is a RAM global; the active subject id is
 * its word at +500 (250 << 1).
 *
 * All 25 `bl` sites are placed and reach six distinct callees, matching the
 * inventory row's calls=25.  Targets come from tools/overlay_call_targets.ts
 * (target offset = stored displacement + 2), never from the disassembler's
 * annotations, which print each of the ten Func_0808a080 sites, the eight
 * Func_08009150 sites, the three Func_080091c0 sites, the two Func_0808a100
 * sites and the two Func_080770c0 sites as separate functions.  0x3f50 ->
 * Func_0808a080, 0x3ee8 -> Func_080770c0, 0x3e88 -> Func_080091c0, 0x3e78 ->
 * Func_08009150, 0x3f98 -> Func_0808a100.
 *
 * Shape, in three parts.
 *
 * 1. For each of participants 22-25: clear the record's flag byte at +91,
 *    and when both its x and z are within 0x9ffff of the subject's, adjust
 *    the *subject's* z - to the participant's z when Func_080770c0(260) is
 *    set, otherwise by the participant's step at +44.  The range test is
 *    written as a signed difference in each direction rather than an absolute
 *    value, and that is reproduced literally.
 *
 * 2. Draw the status line.  Its shape depends on the frame counter being
 *    zero and participant 25's field at +56 holding 0x80000000 (128 << 24);
 *    when it does, one line is drawn whose parameters come from the toggle,
 *    and otherwise two fixed lines are drawn.
 *
 * 3. While the frame counter is zero, flip the toggle and re-place
 *    participants 22-25 at one of two z rows accordingly, then advance the
 *    counter; past 120 frames the counter returns to zero if
 *    Func_080770c0(260) is clear.  Since the counter is advanced every frame
 *    and only reset there, the toggle steps once per 120-frame cycle.
 *
 * Uncertainties: only the record fields at +8, +16, +44, +56 and +91 are
 * asserted.  The `str r3, [r6, #16]` in part 1 writes the *subject's* z, not
 * the participant's; that is what the instruction does, and it is the one
 * place in this owner where the intent is not obvious from the dataflow.
 * The counter comparison is unsigned (`bls`), so it is typed u32.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_0808a080();
s32 Func_080770c0();
void Func_080091c0();
void Func_08009150();
void Func_0808a100();

/* In-image words at file offsets 0x4834 (a toggle) and 0x4838 (a frame
 * counter), shared with Func_020008ec, Func_020009b0 and Func_02000970. */
extern volatile s32 Data_0200c834;
extern volatile u32 Data_0200c838;

void Func_02000714(void)
{
    u8 *subject;
    u8 *record;
    s32 id;
    s32 delta;
    u32 counter;

    subject = Func_0808a080(*(s32 *)(0x02000240 + 500));

    id = 22;
    do {
        record = Func_0808a080(id);
        record[91] = 0;

        delta = *(s32 *)(record + 8) - *(s32 *)(subject + 8);
        if (delta >= 0) {
            if (delta > 0x9ffff) goto next;
        } else {
            if ((*(s32 *)(subject + 8) - *(s32 *)(record + 8)) > 0x9ffff) goto next;
        }

        delta = *(s32 *)(record + 16) - *(s32 *)(subject + 16);
        if (delta >= 0) {
            if (delta > 0x9ffff) goto next;
        } else {
            if ((*(s32 *)(subject + 16) - *(s32 *)(record + 16)) > 0x9ffff) goto next;
        }

        if (Func_080770c0(260) != 0) {          /* 130 << 1 */
            *(s32 *)(subject + 16) = *(s32 *)(record + 16);
        } else {
            *(s32 *)(subject + 16) =
                *(s32 *)(subject + 16) + *(s32 *)(record + 44);
        }
    next:
        id = id + 1;
    } while (id <= 25);

    /* `record` still points at participant 25 here, which is what the field
     * test below reads. */
    if (Data_0200c838 == 0 && *(s32 *)(record + 56) == 0x80000000) {
        if (Data_0200c834 != 0) {
            Func_080091c0(58, 10, 1, 1, 58, 11);
        } else {
            Func_080091c0(58, 28, 7, 1, 58, 13);
        }
    } else {
        Func_080091c0(57, 11, 1, 1, 58, 11);
        Func_080091c0(58, 14, 7, 1, 58, 13);
    }

    if (Data_0200c838 == 0) {
        Data_0200c834 = Data_0200c834 ^ 1;
        if (Data_0200c834 != 0) {
            Func_08009150(Func_0808a080(22), 0x3a80000, 0, 0xb80000);
            Func_08009150(Func_0808a080(23), 0x3c80000, 0, 0xf80000);
            Func_08009150(Func_0808a080(24), 0x3e80000, 0, 0xb80000);
            Func_08009150(Func_0808a080(25), 0x4080000, 0, 0xf80000);
            Func_0808a100(31, 11);
        } else {
            Func_08009150(Func_0808a080(22), 0x3a80000, 0, 0xd80000);
            Func_08009150(Func_0808a080(23), 0x3c80000, 0, 0xd80000);
            Func_08009150(Func_0808a080(24), 0x3e80000, 0, 0xd80000);
            Func_08009150(Func_0808a080(25), 0x4080000, 0, 0xd80000);
            Func_0808a100(31, 10);
        }
    }

    counter = Data_0200c838 + 1;
    Data_0200c838 = counter;
    if (counter > 119 && Func_080770c0(260) == 0) {
        Data_0200c838 = 0;
    }
}
