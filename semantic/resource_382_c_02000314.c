typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 382 owner at 0x02000314 (144 bytes, 4 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x02000314 — which saves
 * r5,r6,r7,lr and then r8,r9,sl through r5,r6,r7 — every branch
 * (0x0200033c, 0x02000340, 0x02000356, 0x0200036c, 0x02000376, 0x02000384)
 * lands inside 0x02000314-0x02000394, and the single matching unwind is
 * `pop {r3, r5, r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 / pop {r5, r6, r7} /
 * pop {r1} / bx r1` at 0x02000396-0x020003a2.  So the executable extent is
 * exactly 0x02000314-0x020003a3 (144 bytes).  The epilogue pops into r1, not
 * r0, so by the epilogue rule r0 is the result — and r0 is set to 0 at
 * 0x02000394, which both exit paths pass through, so the owner returns 0.
 *
 * Pool map — the walk never reaches any of these as instructions:
 *   0x020003a4  0x03001e8c   the IWRAM pointer table
 *   0x020003a8  0x00000ea4   a byte offset, not an address
 *   0x020003aa  0x0000       alignment before the next prologue at 0x020003ac
 * 0x03001e8c is below the 0x02008000 link base and even, so it is an absolute
 * IWRAM address rather than an in-image offset; 0x00000ea4 is added to a
 * loaded pointer rather than dereferenced, which is what identifies it as a
 * displacement.  HANDOVER records 0x03001e8c as a pointer TABLE whose entry 12
 * (byte offset 48) is the well-known 0x03001ebc workspace, and this owner
 * loads exactly entries 0 and 12 — reading them as two unrelated globals would
 * hide that.
 *
 * Nine overlays carry a prologue at 0x0314 and five of them are one
 * transposed 3-D distance helper (see resource_385_c_02000314.c for the
 * byte-diff).  This is the largest of the four that are not: it is the only
 * one of the nine that saves high registers at all.
 *
 * All four call sites were resolved with
 * `bun tools/overlay_call_targets.ts resource_382 0314 03a4`, assigning the
 * i-th occurrence in the source to the i-th site in address order:
 *   0x02000344 -> veneer 0x020019e0 -> Func_0808a080
 *   0x02000350 -> prologue 0x02001674 -> Func_02001674
 *   0x0200035a -> veneer 0x020019e0 -> Func_0808a080
 *   0x02000390 -> prologue 0x02001674 -> Func_02001674
 * `overlay_show`'s printed targets are the pc-relative misreading and are not
 * used.  Two bit-identical `bl` encodings printing different callees is the
 * usual symptom, and it is present here.
 *
 * Func_0808a080 is the established record fetch (it returns the subject record
 * in resource_39f_c_02000e18.c), so its result is passed on as a pointer.
 * `movs r2,#188 / lsls r2,r2,#1` builds 376 as a displacement into the
 * workspace, the family-wide codegen habit HANDOVER warns about; it is read
 * signed with `ldrsh`.
 *
 * UNCERTAINTY: Func_02001674's own meaning is not resolved here.  Its first
 * result is only tested against zero (a guard that abandons the owner), and
 * its second call carries the 18/26 selector and the 0/1 flag built above.
 */

extern u8 *Data_03001e8c[];    /* IWRAM pointer table; [12] is the workspace */

u8 *Func_0808a080();           /* record fetch, returns the record */
s32 Func_02001674();           /* in-overlay, four arguments */

s32 Func_02000314(u8 *entity)
{
    u8 *base = Data_03001e8c[0];
    u8 *workspace = Data_03001e8c[12];
    u16 *state = (u16 *)(entity + 100);
    u8 *record;
    s32 selector;
    s32 flag;

    record = Func_0808a080((*state & 1) != 0 ? 15 : 14);
    if (Func_02001674(entity, record, 32, 0) != 0) {
        return 0;
    }

    record = Func_0808a080(0);

    selector = 18;
    flag = 0;
    if (*(s16 *)(workspace + 376) != 0 || base[0x0ea4] != 0) {
        selector = 26;
        if ((*state & 2) != 0) {
            flag = 1;
        }
    }

    Func_02001674(entity, record, selector, flag);
    return 0;
}
