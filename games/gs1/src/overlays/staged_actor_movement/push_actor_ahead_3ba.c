#include "types.h"

typedef struct SceneRecord {
    u8 pad_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_14[14];
    u8 state;
    u8 pad_23;
    s32 motion_24;
    u8 pad_28[4];
    s32 motion_2c;
    s32 rate_x;
    s32 rate_z;
    u8 pad_38[33];
    u8 flags;
} SceneRecord;

typedef struct Position3 {
    s32 x;
    s32 y;
    s32 z;
} Position3;

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
 * x3, Object_SetMode x2, Object_SetPosition x2, Audio_PlayCue x2, and one each of
 * Scene_GetRecord, Object_CheckMovementCollision, Func_080000c0 and Object_CommitPosition.  The C below
 * reproduces that multiset exactly; the inventory's `calls=13` agrees.
 *
 * LINK BASE 0x02008000, proven for this overlay by the byte-exact sibling
 * `games/gs1/assets/code/resource_3ba_c_02000158.c` (pool word 0x0200804d =
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
 * Object_CheckMovementCollision reports a positive code for the destination.  Otherwise mark
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
 *  - Object_CheckMovementCollision's result is tested with `bgt`, i.e. signed and strictly
 *    greater than zero, so a negative code does not abort.  Elsewhere in the
 *    tree the value 2 means blocked.
 *  - Func_020038b0 is this overlay's own byte-exact lookup
 *    (`games/gs1/assets/code/resource_3ba_c_020038b0.c`) and takes a single argument;
 *    all three sites also load r1 with a record pointer, which is not
 *    asserted as an argument.
 *  - The two Object_SetPosition calls reuse the same position block; the second is
 *    reached with r0 reloaded from r8, so it moves the subject, not the target.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
SceneRecord *Func_020075cc();
SceneRecord *Func_020071f6(Position3 *, SceneRecord *);
SceneRecord *Func_02007220(Position3 *, SceneRecord *);
SceneRecord *Func_0200724c(Position3 *, SceneRecord *);
s32 Func_020075e6(SceneRecord *, Position3 *);
void Func_02007594(SceneRecord *, s32);
void Func_020074fa(s32);
void Func_020075da(SceneRecord *, s32, s32, s32);
void Func_020075ea(SceneRecord *, s32, s32, s32);
void Func_02007810(s32);
void Func_020075fe(SceneRecord *);
void Func_0200781e(s32);
void Func_020075e4(SceneRecord *, s32);
                                /* scene record for a subject handle */
                                /* select presentation mode (record, mode) */
                                /* place the record at (x, y, z) */
                                /* re-attach the camera to a record */
                                /* terrain probe; positive result blocks */
                                /* wait n frames */
                                /* play a cue */

/* This overlay's own occupancy lookup; byte-exact source in games/gs1/assets/code. */

/* In-image dir table at file offset 0x4154 (0x0200c154 - 0x8000):
 * sixteen packed steps, high half x, low half z. */
extern u32 Data_0200c154[];
extern s16 Data_02000240[];

void StagedActor_PushActorAhead(void)
{
    SceneRecord *subject;
    SceneRecord *target;
    SceneRecord *blocker;
    u32 step;
    u32 dir;
    Position3 pos;
    u32 idx = 250;
    s32 zero;
    s32 handle;

    handle = *(s32 *)((u8 *)Data_02000240 + (idx << 1));
    subject = Func_020075cc(handle);

    dir = subject->facing >> 12;

    step = Data_0200c154[dir];
    pos.x = subject->x + (s32)(step & 0xffff0000);
    pos.y = subject->y;
    step <<= 16;
    pos.z = subject->z + (s32)step;

    target = Func_020071f6(&pos, subject);
    if (target == 0) {
        return;
    }

    /* Is the cell one step beyond the target already taken? */
    step = Data_0200c154[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    blocker = Func_02007220(&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    /* ...and the cell directly above the target? */
    pos.x = target->x;
    pos.y = target->y + 0x100000;      /* 128 << 13 */
    pos.z = target->z;

    blocker = Func_0200724c(&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = Data_0200c154[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    if (Func_020075e6(target, &pos) > 0) {
        return;
    }

    Func_02007594(subject, 8);
    Func_020074fa(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    Func_020075da(target, pos.x, pos.y, pos.z);

    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    Func_020075ea(subject, pos.x, pos.y, pos.z);

    Func_02007810(0xee);
    Func_020075fe(target);
    Func_0200781e(0x120);                                /* 144 << 1 */

    target->x = pos.x;
    target->z = pos.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    Func_020075e4(subject, 1);
}
