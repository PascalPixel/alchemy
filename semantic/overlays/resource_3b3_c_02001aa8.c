typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 3b3 layer inheritance resolver at 0x02001aa8 (320-byte whole
 * owner, 6 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues, and its start is confirmed from the other side: the sibling
 * reconstruction's resource_3b3_c_020019f0.c ends its literal pool at 0x02001aa7 and
 * names 0x02001aa8 as the next prologue.  Walking from that prologue every
 * branch (0x02001ac8, 0x02001aec, 0x02001b0e, 0x02001b12, 0x02001b28,
 * 0x02001b32, 0x02001b66, 0x02001ba4, 0x02001bc4) lands inside
 * 0x02001aa8-0x02001bdd, and the walk stops at the interworking return
 * `pop {r3} / mov r8,r3 / pop {r5,r6,r7} / pop {r1} / bx r1` at
 * 0x02001bd4-0x02001bdc.  So the executable extent is exactly
 * 0x02001aa8-0x02001bdd (310 executable bytes).  The whole-owner span is
 * 320 bytes after annexing the alignment and pool described below.
 *
 * THE EPILOGUE POPS INTO r1, NOT r0, so by the epilogue rule r0 is the result.
 * r0 is set to 0 at 0x02001bd2 on the joined path and the early arm falls into
 * the same tail, so the owner returns 0 unconditionally — but it is `s32`, not
 * `void`, and spelling it `void` would drop a real (if constant) result.
 *
 * Pool map — the halfword at 0x02001bde is `0x0000` alignment and the two
 * words after it are literals the walk never reaches as instructions, both of
 * which objdump renders as plausible `subs`/`lsrs`:
 *   0x02001be0  0x03001ebc   the IWRAM workspace pointer
 *   0x02001be4  0x00000cc7   a byte offset into that workspace
 * The next prologue begins at 0x02001be8.
 *
 * All six call sites were resolved with
 * `bun tools/overlay_call_targets.ts resource_3b3 1aa8 1bde`, and the
 * per-target histogram is Func_0808a080 x3, Func_02001938 x2, Func_020019f0
 * x1:
 *   0x02001ab2 -> veneer 0x02b1c -> Func_0808a080   record fetch, index 0
 *   0x02001af2 -> prologue 0x01938 -> Func_02001938
 *   0x02001afc -> veneer 0x02b1c -> Func_0808a080   record fetch, index 8..11
 *   0x02001b04 -> prologue 0x01938 -> Func_02001938
 *   0x02001b18 -> veneer 0x02b1c -> Func_0808a080   record fetch, index 8..11
 *   0x02001b22 -> prologue 0x019f0 -> Func_020019f0
 * Func_020019f0 is the proximity/priority merge the sibling reconstruction converted
 * from this same sweep — so, as in resource_39e, one unrecorded owner calls
 * another, which is why the inventory reaches neither.
 *
 * FOUR OF THE SIX SITES ARE INSIDE TWO SEPARATE FOUR-ITERATION LOOPS, one site
 * each.  The loops are not mergeable even though both walk records 8..11 with
 * the same fetch: the first accumulates Func_02001938 over all four and the
 * second runs Func_020019f0 over all four, and the second only runs at all if
 * the first's total is non-zero.  Folding them into one loop body would move
 * the Func_020019f0 calls under a different condition.
 *
 * `lsls #28 / lsrs #30` is the two-bit field at bits 2-3 of the byte at +9 of
 * the sub-object at +80, and the mask that writes it back is spelled
 * `movs r2,#13 / negs r2` (0xfffffff3) — the same idiom resource_3b3:19f0
 * documents.  The field is copied from the reference object to the argument in
 * three separate places, and the copy at +21 rides along with the one at +9 in
 * the last of them.
 *
 * A SHARED STORE, NOT A SHARED CALL: the `b.n 0x02001bd0` at 0x02001aec lands
 * on the `strb r3,[r1,#0]` that the tail also falls into, with r1 and r3 set up
 * differently on the two paths (object+0x59 in the early arm, object+0x23 in
 * the tail).  One instruction serves two different writes.  This costs nothing
 * in the multiset — a `strb` is not a `bl` — so both writes are spelled out
 * rather than contorted into a shared `goto`, which would have to carry the
 * pointer as a variable and would read as one write where the source has two.
 *
 * Behaviour: when the workspace flag at +0xcc7 (read as a SIGNED byte, `lsls
 * #24 / asrs #24`) is 1, the argument simply inherits record 0's two-bit layer
 * and is marked settled.  Otherwise the argument is scored against record 0 and
 * records 8 through 11; if anything scored, the merge pass runs over 8..11.
 * Then depth decides: if the argument is in front of record 0 (+12 lower) it is
 * flagged at +0x23 bit 1 and unsettled, and if its layer is also below record
 * 0's it takes record 0's layer outright; if it is behind, bit 1 is cleared and
 * it is marked settled.  Anything not resolved by a layer takeover ends with
 * bit 0 of +0x23 set.
 */

u8 *Func_0808a080();           /* record fetch, returns the record */
s32 Func_02001938();           /* local: score two objects */
s32 Func_020019f0();           /* local: proximity/priority merge */

s32 Func_02001aa8(u8 *object)
{
    u8 *reference = Func_0808a080(0);
    u8 *flags;
    s32 index;
    s32 scored;

    if ((s8)(*(u8 **)0x03001ebc)[0xcc7] == 1) {
        (*(u8 **)(object + 80))[9] =
            (u8)(((*(u8 **)(reference + 80))[9] & 12)
                 | ((*(u8 **)(object + 80))[9] & ~12));
        object[0x59] |= 1;
        return 0;
    }

    scored = Func_02001938(object, reference);
    for (index = 8; index <= 11; index++)
        scored += Func_02001938(object, Func_0808a080(index));

    if (scored != 0) {
        for (index = 8; index <= 11; index++)
            Func_020019f0(object, Func_0808a080(index));
    }

    flags = object + 0x23;
    if (*(s32 *)(object + 12) < *(s32 *)(reference + 12)) {
        *flags |= 2;
        object[0x59] &= 0xfe;
        if ((((u32)(*(u8 **)(object + 80))[9] << 28) >> 30)
            < (((u32)(*(u8 **)(reference + 80))[9] << 28) >> 30)) {
            *flags &= 0xfe;
            (*(u8 **)(object + 80))[9] =
                (u8)(((*(u8 **)(reference + 80))[9] & 12)
                     | ((*(u8 **)(object + 80))[9] & ~12));
            (*(u8 **)(object + 80))[21] =
                (u8)(((*(u8 **)(reference + 80))[21] & 12)
                     | ((*(u8 **)(object + 80))[21] & ~12));
            scored = 1;
        }
    } else {
        *flags &= 0xfd;
        object[0x59] |= 1;
    }

    if (scored == 0) *flags |= 1;
    return 0;
}
