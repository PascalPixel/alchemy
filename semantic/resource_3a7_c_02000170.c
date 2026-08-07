/*
 * resource_3a7 "push the object in front of the actor" step at 0x02000170, 332 bytes.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7,sl / mov r6,r9 / mov r5,r8 /
 * push {r5, r6, r7} / sub sp, #12` at 0x02000170 through the matching
 * `add sp,#12 / pop {r3, r5, r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 /
 * pop {r5, r6, r7} / pop {r0} / bx r0` at 0x0200029e.  The return address is
 * popped into r0, so the owner is `void`.  Three literal-pool words sit past the
 * epilogue inside the 332-byte span - 0x020002b0 (0x02009844), 0x020002b4
 * (0xffff0000) and 0x020002b8 (0x00003333); the control-flow walk reaches none
 * of them, and 0x020002ae is alignment.
 *
 * The owner takes no arguments (r0 is set to 0 before the first call).
 *
 * PACKED DIRECTION TABLE.  0x02009844 is even, so under the proven 0x02008000
 * link base it is in-image DATA at file offset 0x1844.  It is indexed by
 * `heading >> 12` (a 4-bit heading, 16 entries of 4 bytes) and each entry packs
 * the X step in the high half and the Z step in the low half - the documented
 * cross-overlay idiom.  This owner uses all three of its documented readings:
 * `>> 16` and `(s16)` to get the two integer steps, and `& 0xffff0000` /
 * `<< 16` to promote them back to 16.16 without a multiply.
 *
 * HALFWORD COORDINATE VIEWS.  The s16 at +10 and +18 are the integer parts of
 * the 16.16 words at +8 and +16.  Adding the integer step to the halfword view
 * and then `>> 4` is the 16-pixel tile-grid idiom, and it produces exactly the
 * cell coordinates Func_02000134 expects - the byte-exact sibling
 * assets/code/resource_3a7_c_02000134.c compares its two arguments against
 * `p[2] >> 20` and `p[4] >> 20`, i.e. the same cells derived from the 16.16
 * words directly.  That sibling also fixes byte +0x59 (89) as the "slot
 * occupied" flag and the slot table as `Data_03001ebc + 0x14`, entries 8..65.
 *
 * Shape:
 *   - take the player record (selector 0), read its heading and step one tile
 *     forward, and look up whatever occupies that cell;
 *   - refuse if the cell is empty, or if the occupant is one of the four scene
 *     entities 11..14, or if the cell BEYOND it is occupied by something whose
 *     +89 has bit 0 set;
 *   - build the occupant's pushed-to position (x + xStep, y, z + zStep in
 *     16.16) and ask Func_080091d8 whether the move is allowed; a positive
 *     result refuses;
 *   - otherwise play the push: set the pusher's mode, wait 15 then 238 frames,
 *     give both records the 0x3333 speed pair at +48/+52, move both to the
 *     target position, run the occupant's landing call, wait 288 frames, commit
 *     the occupant's 16.16 x and z, and clear the momentum fields on both
 *     (with 0x80000000 written to the pusher's +56 and +64).
 *
 * The byte +89 test at 0x020001b2 happens BEFORE the null test at 0x020001b8 in
 * the assembly.  It is redundant either way: Func_02000134 only ever returns a
 * record whose +89 is non-zero, and both tests jump to the same exit, so the
 * pair is exactly "the cell is empty".  Written as a short-circuit here so the C
 * does not dereference null.
 *
 * Call targets resolved with
 *   bun tools/overlay/overlay_show.ts resource_3a7 0170 -n 332 |
 *   bun tools/lib/overlay_call_targets.ts resource_3a7 --annotate
 * Per-target multiset over the 13 sites, matching the inventory's calls=13:
 *   Func_0808a080 x2 <- veneer 0x02001130
 *   Func_02000134 x2  (this overlay's byte-exact tile-occupant lookup)
 *   Func_080091d8 x1 <- veneer 0x020010d8
 *   Func_08009080 x2 <- veneer 0x02001098
 *   Func_080000c0 x1 <- veneer 0x02001088+  (frame wait)
 *   Func_080f9010 x2 <- veneer 0x02001150
 *   Func_08009150 x2 <- veneer 0x020010b8  (place a record at (x, y, z))
 *   Func_08009158 x1 <- veneer 0x020010c0
 * The second `Func_0808a080` is inside the four-iteration identity loop, so it
 * is one C call expression, not four.
 *
 * Uncertainties: the record fields written at the end (+36, +44, +56, +64) are
 * transcribed, not interpreted; 0x3333 at +48/+52 is a speed pair only by
 * analogy with the surrounding move; and the 238/288-frame waits are frame
 * counts, matching the byte-exact sibling's use of the same import with 0x50.
 */

/* Old-style declarations: overlay imports vary their argument count between
 * call sites in this overlay.  Those used in conditions need a return type. */
#include "types.h"

u8 *Func_020012b0();
u8 *Func_020002de();
u8 *Func_020012f4();
u8 *Func_02000322();
s32 Func_02001308();
void Func_020012d4();
void Func_020012b4();
void Func_0200139a();
void Func_02001312();
void Func_02001322();
void Func_02001330();
void Func_020013c8();
void Func_02001334();
                        /* scene-entity record by selector */
                        /* tile-occupant lookup (cellX, cellZ); byte-exact */
                        /* move permitted? (record, position); > 0 refuses */
                        /* set a record's mode */
                        /* frame wait */
                        /* scripted delay, in frames */
                        /* place a record at (x, y, z) */
                        /* landing/settle call for a moved record */

/* Packed direction steps at file offset 0x1844 (pool word 0x02009844 under the
 * proven 0x02008000 link base): X step in the high half, Z step in the low. */
extern s32 Data_02009844[16];

void Func_02000170(void)
{
    s32 permuted_46;
    u8 *pusher = Func_020012b0(0);
    s32 heading = *(u16 *)(pusher + 6) >> 12;
    s32 step = Data_02009844[heading];
    u8 *target;
    u8 *beyond;
    s32 position[3];
    u32 index;

    target = Func_020002de((*(s16 *)(pusher + 10) + (step >> 16)) >> 4,
                           (*(s16 *)(pusher + 18) + (s16)step) >> 4);

    if (target == 0 || target[89] == 0) {
        return;
    }

    /* Refuse to push any of the four scripted scene entities. */
    for (index = 0; index <= 3; index++) {
        if (target == Func_020012f4(index + 11)) {
            return;
        }
    }

    step = Data_02009844[heading];
    beyond = Func_02000322((*(s16 *)(target + 10) + (step >> 16)) >> 4,
                           (*(s16 *)(target + 18) + (s16)step) >> 4);

    if (beyond != 0 && (beyond[89] & 1) != 0) {
        return;
    }

    target[34] = 2;

    position[0] = *(s32 *)(target + 8) + (step & (s32)0xffff0000);
    step = Data_02009844[heading];
    position[1] = *(s32 *)(target + 12);
    position[2] = *(s32 *)(target + 16) + (step << 16);

    if (Func_02001308(target, position) > 0) {
        return;
    }

    Func_020012d4(pusher, 8);
    Func_020012b4(15);
    Func_0200139a(238);

    *(s32 *)(target + 48) = 0x3333;
    *(s32 *)(target + 52) = 0x3333;
    Func_02001312(target, position[0], position[1], position[2]);

    *(s32 *)(pusher + 48) = 0x3333;
    *(s32 *)(pusher + 52) = 0x3333;
    Func_02001322(pusher, position[0], position[1], position[2]);

    Func_02001330(target);
    Func_020013c8(288);

    permuted_46 = position[0];
    *(s32 *)(target + 16) = position[2];
    *(s32 *)(target + 8) = permuted_46;

    *(s32 *)(target + 36) = 0;
    *(s32 *)(pusher + 36) = 0;
    *(s32 *)(pusher + 44) = 0;
    *(s32 *)(pusher + 56) = (s32)0x80000000;
    *(s32 *)(target + 44) = 0;
    *(s32 *)(pusher + 64) = (s32)0x80000000;

    Func_02001334(pusher, 1);
}
