#include "types.h"

/*
 * resource_3ba owner at 0x020038f8, 360 bytes (0x020038f8-0x02003a5f):
 * 344 bytes of code plus the four-word literal pool at 0x02003a50, which ends
 * exactly where the next owner (0x02003a60) begins.  Nothing live crosses the
 * row boundary.
 *
 * Prologue `push {r5, r6, r7, lr}` plus the high-register saves
 * `mov r7, fp / mov r6, sl / mov r5, r9 / push {r5, r6, r7}` and
 * `mov r7, r8 / push {r7}`, then `sub sp, #12` for a three-word position
 * block.  The unwind at 0x02003a3e..0x02003a4e ends `pop {r0} ; bx r0`, so r0
 * holds the popped return address and the owner returns nothing.  Every early
 * exit branches to that same epilogue.  No argument register is read before
 * being written, so the owner takes no arguments.
 *
 * CALL ACCOUNTING.  Thirteen `bl` sites, all resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3ba --annotate`: Func_020038b0
 * x3, Func_08009080 x2, Func_08009150 x2, Func_080f9010 x2, and one each of
 * Func_0808a080, Func_080091d8, Func_080000c0 and Func_08009158.  The C below
 * reproduces that multiset exactly; the inventory's `calls=13` agrees.
 *
 * LINK BASE 0x02008000, proven for this overlay by the byte-exact sibling
 * `assets/code/resource_3ba_c_02000158.c` (pool word 0x0200804d =
 * Func_0200004c + the Thumb bit).  Applying the parity rule to the pool:
 *   0x0200c154  even -> in-image DATA at file offset 0x4154, the direction
 *                table indexed below;
 *   0x02000240  below the base -> a RAM global, the shared `s16[]` table that
 *                the byte-exact resource_36f/resource_371 sources declare.
 *                The index is built as 250 << 1 = 500, the same pointer word
 *                `*(void **)&Data_02000240[250]` that resource_36f reads and
 *                that this overlay's 0x02003a60 also uses.
 *
 * SHAPE — the push interaction.  Take the active subject, turn its facing
 * halfword into a 0..15 direction index (`>> 12`), and read one packed delta
 * word from Data_0200c154: the high half is the x step and the low half,
 * shifted up 16, is the z step, both 16.16.  Probe the cell one step ahead of
 * the subject; if something occupies it, that is the pushable target.  Then
 * three refusals, in order: the cell one step beyond the target is occupied by
 * something whose flag bit 0 at +0x59 is set; the cell directly above the
 * target (y + 0x100000) is likewise occupied; or the terrain probe
 * Func_080091d8 reports a positive code for the destination.  Otherwise mark
 * the target state 2, take control of the subject, wait 15 frames, move both
 * the target and the subject onto the destination with the same 0x3333 motion
 * rate, play two cues around re-attaching the camera to the target, commit the
 * target's x/z and clear its two motion words, and hand the subject back.
 *
 * UNCERTAINTIES.
 *  - Record fields asserted: the facing halfword at +6, the position words at
 *    +8/+12/+16, the state byte at +0x22, the occupancy flag bit 0 of the byte
 *    at +0x59, the motion-rate pair at +0x30/+0x34, and the two words cleared
 *    at +0x24/+0x2c.
 *  - Func_080091d8's result is tested with `bgt`, i.e. signed and strictly
 *    greater than zero, so a negative code does not abort.  Elsewhere in the
 *    tree the value 2 means blocked.
 *  - Func_020038b0 is this overlay's own byte-exact lookup
 *    (`assets/code/resource_3ba_c_020038b0.c`) and takes a single argument;
 *    all three sites also load r1 with a record pointer, which is not
 *    asserted as an argument.
 *  - The two Func_08009150 calls reuse the same position block; the second is
 *    reached with r0 reloaded from r8, so it moves the subject, not the target.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_020075cc();
u8 *Func_020071f6(s32 *);
u8 *Func_02007220(s32 *);
u8 *Func_0200724c(s32 *);
s32 Func_020075e6();
void Func_02007594();
void Func_020074fa();
void Func_020075da();
void Func_020075ea();
void Func_02007810();
void Func_020075fe();
void Func_0200781e();
void Func_020075e4();
                                /* scene record for a subject handle */
                                /* select presentation mode (record, mode) */
                                /* place the record at (x, y, z) */
                                /* re-attach the camera to a record */
                                /* terrain probe; positive result blocks */
                                /* wait n frames */
                                /* play a cue */

/* This overlay's own occupancy lookup; byte-exact source in assets/code. */

/* In-image direction table at file offset 0x4154 (0x0200c154 - 0x8000):
 * sixteen packed steps, high half x, low half z. */
extern u32 Data_0200c154[];

void Func_020038f8(void)
{
    u8 *subject;
    u8 *target;
    u8 *blocker;
    u32 step;
    s32 direction;
    s32 position[3];

    s32 permuted_13;
    s32 permuted_40;
    u32 permuted_43;
    s32 permuted_25;
    s32 permuted_27;
    subject = Func_020075cc(*(s32 *)(0x02000240 + 500));

    direction = *(u16 *)(subject + 6) >> 12;

    step = Data_0200c154[direction];
    position[0] = *(s32 *)(subject + 8) + (s32)(step & 0xffff0000);
    permuted_13 = *(s32 *)(subject + 12);
    position[1]  = permuted_13;
    position[2] = *(s32 *)(subject + 16) + (s32)(step << 16);

    target = Func_020071f6(position);
    if (target == 0) {
        return;
    }

    /* Is the cell one step beyond the target already taken? */
    step = Data_0200c154[direction];
    permuted_25 = *(s32 *)(target + 8) + (s32)(step & 0xffff0000);
    permuted_27 = *(s32 *)(target + 12);
    position[0]  = permuted_25;
    position[2] = *(s32 *)(target + 16) + (s32)(step << 16);
    position[1]  = permuted_27;

    blocker = Func_02007220(position);
    if (blocker != 0 && (*(blocker + 0x59) & 1) != 0) {
        return;
    }

    /* ...and the cell directly above the target? */
    position[0] = *(s32 *)(target + 8);
    position[1] = *(s32 *)(target + 12) + 0x100000;      /* 128 << 13 */
    position[2] = *(s32 *)(target + 16);

    blocker = Func_0200724c(position);
    if (blocker != 0 && (*(blocker + 0x59) & 1) != 0) {
        return;
    }

    *(target + 0x22) = 2;

    permuted_40 = *(s32 *)(target + 8) + (s32)(step & 0xffff0000);
    position[1] = *(s32 *)(target + 12);
    permuted_43 = Data_0200c154[direction];
    position[0]  = permuted_40;
    step  = permuted_43;
    position[2] = *(s32 *)(target + 16) + (s32)(step << 16);

    if (Func_020075e6(target, position) > 0) {
        return;
    }

    Func_02007594(subject, 8);
    Func_020074fa(15);

    Func_020075da(target, position[0], position[1], position[2]);
    *(s32 *)(target + 0x30) = 0x3333;
    *(s32 *)(target + 0x34) = 0x3333;

    *(s32 *)(subject + 0x30) = 0x3333;
    *(s32 *)(subject + 0x34) = 0x3333;
    Func_020075ea(subject, position[0], position[1], position[2]);

    Func_02007810(0xee);
    Func_020075fe(target);
    Func_0200781e(0x120);                                /* 144 << 1 */

    *(s32 *)(target + 8) = position[0];
    *(s32 *)(target + 16) = position[2];
    *(s32 *)(target + 0x24) = 0;
    *(s32 *)(target + 0x2c) = 0;

    Func_020075e4(subject, 1);
}
