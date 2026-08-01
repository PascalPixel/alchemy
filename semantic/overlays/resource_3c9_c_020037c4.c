typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_3c9 owner at 0x020037c4, 252 bytes: a per-frame PARKED
 * check on the pair of scene records 20 and 19. Once record 19 has been
 * parked -- its three mirrored fields all carrying the sentinel
 * 0x80000000 -- it clears both records' +6 halfword and then, under two
 * separate scene flags, walks both records along fixed increments
 * toward fixed limits.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020037c4 through the single
 * epilogue `pop {r5, r6} / pop {r0} / bx r0` at 0x020038ae-0x020038b2,
 * then a three-word literal pool at 0x020038b4-0x020038bf. Next owner's
 * prologue at 0x020038c0. Measured to the epilogue.
 *
 * `arg0` is never read before being overwritten (r0 takes the constant
 * 20 as the first data instruction), so void in, void out.
 *
 * Published, not called: the address appears as the in-image spelling
 * 0x0200b7c4 in exactly one word, at overlay offset 0x35ac -- inside
 * the trailing pool of the still-parked 4,708-byte owner 0x02002360,
 * which is therefore its installer.
 *
 * COUNT DISCIPLINE, and this row is the worked example. `bun
 * tools/overlay_call_targets.ts resource_3c9 37c4 38c0` reports
 * `sites=11 distinct_targets=4 veneer=10 unknown=1`. Ten `bl`
 * instructions are transcribed here, and ten is right: the eleventh
 * "site" is the literal pool word at 0x020038bc, 0xfc5ef004, which
 * disassembles as `bl` and is not one. Assert the transcribed count
 * against the RESOLVED count, not against `sites=`, and rule the
 * difference explicitly -- a pool word wearing a call is the same trap
 * as a pool word wearing a store, and the arithmetic is what catches it
 * rather than the eye.
 *
 * A DEAD FIRST CLAUSE, transcribed as an observation and not smoothed
 * away: at 0x020037d6-0x020037e0 the row loads record 20's +56 field,
 * compares it against 0x80000000 and, when equal, loads +60 into r2 --
 * and r2 is unconditionally reloaded with 0x80000000 at 0x020037e2. The
 * result of record 20's test reaches nothing. The gate that survives
 * depends only on record 19. This is left described rather than emitted
 * because emitting a statement whose value is discarded would assert a
 * source shape that is not established; what IS established is that the
 * hardware performs the load and then throws it away.
 *
 * CORRECTED after drafting Func_02005688, and the first version of
 * this paragraph is worth stating so nobody re-derives it. I wrote that
 * this row waits for the orbit steps Func_02003600 and Func_02003660 to
 * "park on the sentinel", and flagged +60 as read here and written by
 * nothing. Both were wrong. The orbit steps write LIVE COORDINATES into
 * +56 and +64 and never write 0x80000000 at all; the 24-byte leaf
 * Func_02005688 stamps the sentinel into +56, +60 AND +64 in one go,
 * which is also what accounts for the +60 I could not place. So the
 * gate is not "the animation finished" -- it is "this record has been
 * parked", and the thing that parks it is a routine no keyed sweep
 * could see. Reading +56/+64 as belonging to the orbit steps was
 * proximity reasoning: they were the rows I had just drafted.
 *
 * Uncertainties:
 *   - the pool constant added to +16 at the end is 0xfc5ef004, read
 *     back independently from the assembled image and not from the
 *     listing alone. As a signed value it is -60,887,036, which is two
 *     orders of magnitude larger than the 0xb60000 limit it is gated
 *     on, so it is almost certainly a wrap rather than a step -- but
 *     "almost certainly" is not evidence, and it is transcribed exactly
 *     and left unexplained.
 *   - `& -13` clears bits 2 and 3; the constants 0x235 and 0x234 are
 *     scene flag ids of unestablished meaning; 0x03001e40 is the
 *     tree's free-running FRAME COUNTER, so `& 2` alternates the two
 *     arms every other frame rather than selecting a mode.
 *   - field +6 is a u16 of unknown role; +24/+28 and +8/+16 are s32
 *     position-family fields, not named to an axis.
 *   - 0x80000000 is read as a sentinel because Func_02005688 stamps it
 *     into three fields at once and this row tests all three for
 *     equality rather than magnitude. Nothing proves it could not also
 *     be a legitimate coordinate.
 */

extern u8 *Func_0808a080(s32 index);   /* scene-record accessor */
extern s32 Func_080770c0();            /* scene/event flag test */
extern void Func_0808a158(s32 index, s32 value);

void Func_020037c4(void)
{
    u8 *lead = Func_0808a080(20);
    u8 *follow = Func_0808a080(19);
    s32 arrived;

    /*
     * The dead clause described in the header sits here: record 20's
     * +56 is loaded and compared against 0x80000000, and on equality
     * +60 is loaded into a register that is immediately overwritten.
     */

    arrived = 0;
    if (*(s32 *)(follow + 56) == (s32)0x80000000
        && *(s32 *)(follow + 60) == *(s32 *)(follow + 56)
        && *(s32 *)(follow + 64) == *(s32 *)(follow + 60)) {
        arrived = 1;
    }
    if (arrived == 0) {
        return;
    }

    *(u16 *)(lead + 6) = 0;
    *(u16 *)(follow + 6) = 0;

    if (Func_080770c0(0x235) != 0) {
        Func_0808a158(20, 7);
        Func_0808a158(19, 7);
        if (*(s32 *)(lead + 24) < (160 << 9)) {
            *(s32 *)(lead + 24) += 128 << 2;
            *(s32 *)(lead + 28) += 128 << 2;
            *(s32 *)(follow + 24) += 128 << 2;
            *(s32 *)(follow + 28) += 128 << 2;
        }
    } else if ((*(s32 *)0x03001e40 & 2) != 0) {
        Func_0808a158(20, 15);
        Func_0808a158(19, 0);
    } else {
        Func_0808a158(20, 0);
        Func_0808a158(19, 15);
    }

    if (Func_080770c0(141 << 2) == 0) {
        return;
    }

    if (*(s32 *)(lead + 8) < (156 << 17)) {
        *(s32 *)(lead + 8) += 128 << 5;
        *(s32 *)(follow + 8) += 128 << 5;
    }

    if (*(s32 *)(lead + 16) > (182 << 16)) {
        *(s32 *)(lead + 16) += (s32)0xfc5ef004;
        *(s32 *)(follow + 16) += (s32)0xfc5ef004;
    }
}
