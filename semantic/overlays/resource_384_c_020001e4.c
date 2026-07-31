typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_384 owner at 0x020001e4, 124 bytes: the overlay's ROOT.  Publish
 * the scene id, then take one of three exits according to the scene's stored
 * sub-state.
 *
 * Complete owner: `push {r5, lr}` at 0x020001e4 through
 * `pop {r5} / pop {r1} / bx r1` at 0x0200024e-0x02000252.  The return address
 * is popped into r1, not r0 (HANDOVER section 0, epilogue rule), so r0
 * survives and IS the result -- and all three arms converge on
 * `movs r0, #0` at 0x0200024c, so this returns a constant 0 status.  No
 * argument register is read before being written.  The 20-byte literal pool at
 * 0x02000254-0x02000267 is inside the advertised span and after the epilogue.
 *
 * This owner is entry 0 of the exported-entry veneer table at image offset 0:
 * the first table word is 0x020081e5 = 0x01e4 + the Thumb bit under the proven
 * 0x02008000 base.  Working root-down, the overlay's call graph starts here.
 *
 * Five call sites, resolved with
 * `bun tools/overlay_call_targets.ts resource_384 01e4`:
 *   0x02000204 -> veneer 0x0260 -> Func_080770d0   x1
 *   0x02000210 -> veneer 0x0280 -> Func_0808a080   x4 (with 0x222, 0x232,
 *                                                     0x240)
 * The four Func_0808a080 sites have bit-identical `bl` halfwords
 * (`f000 f93f`) yet the listing prints four different callees 0x12 apart --
 * the signature of the `bl` decoding bug, and the cheapest possible proof that
 * they are one callee.
 *
 * THE DISPLACEMENT/VALUE TRAP, THREE ROLES FROM ONE REGISTER.  r2 is built as
 * `movs r2,#224 / lsls r2,#1` = 448 and used as a DISPLACEMENT into the
 * workspace; then `adds r2,#73` = 521 and that is STORED as the value; then
 * `subs r2,#71` = 450 and that is a DISPLACEMENT again, into a different base.
 * The 448/521 half is the pair HANDOVER already records verbatim from
 * resource_386:04e4 and resource_38c:04c8; the third step is new.  There is no
 * arithmetic relationship between the three numbers that matters -- 450 is not
 * "521 minus something meaningful", it is simply the next constant the encoder
 * could reach cheaply from the register it had.
 *
 * So the two workspace touches are:
 *  - `*(s32 *)(*(0x03001ebc) + 448) = 521` -- the scene/phase id slot
 *    documented in HANDOVER section 0, published on entry with a per-scene
 *    constant.  521 is the same id resource_3a2's 0x01ec and 0x0c30 owners
 *    publish.
 *  - `mode = *(s16 *)(0x02000240 + 450)` -- a signed halfword (`ldrsh`) out of
 *    the shared cross-overlay scene-record block.  0x02000240 is below the
 *    0x02008000 band on the two-sided pool-word test, so it is a RAM global
 *    and not in-image data.
 *
 * The three arms, none of which may be folded (the call multiset depends on
 * keeping them apart):
 *  - mode == 2: clear the story flag 0x12f and fall to the common return.
 *  - mode == 10: set bits 0x14 in byte +89 of slot 8's record.
 *  - otherwise: do the same to slots 8, 9 and 10 in turn.
 * The two-arm and three-arm halves are written out separately in the original
 * -- the mode == 10 arm is not a special case of the loop, it is its own copy
 * with an immediate mask where the general arm keeps the mask live in r5.
 *
 * Uncertainties.  Byte +89 of a scene record and the bit pair 0x14 are not
 * identified; they are set and never read here.  The slot ids 8/9/10 match the
 * ids the 0x02000094 script drives, which is consistent with them naming the
 * same three participants, but this owner does not prove it.  0x12f = 303 is
 * taken as a story-flag id because that is Func_080770d0's established role
 * across the semantic tree.
 */

/* Overlay imports (via the veneer table).  Old-style declarations: one import
 * name can be reached with different argument counts at different sites. */
void Func_080770d0();    /* clear a story flag */
u8 *Func_0808a080();     /* record for a scene slot id */

/* Shared cross-overlay scene-record block. */
#define SCENE_BLOCK_020001e4 ((u8 *)0x02000240)

s32 Func_020001e4(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    s16 mode;
    u8 *record;

    /* Scene/phase id published on entry. */
    *(s32 *)(workspace + 448) = 521;

    mode = *(s16 *)(SCENE_BLOCK_020001e4 + 450);

    if (mode == 2) {
        Func_080770d0(0x12f);
    } else if (mode == 10) {
        record = Func_0808a080(8);
        record[89] |= 20;
    } else {
        record = Func_0808a080(8);
        record[89] |= 20;
        record = Func_0808a080(9);
        record[89] |= 20;
        record = Func_0808a080(10);
        record[89] |= 20;
    }

    return 0;
}
