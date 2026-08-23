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
 * resource_3bb owner at 0x02003b90, 360 bytes (0x02003b90-0x02003cf7):
 * 344 bytes of code plus the four-word literal pool at 0x02003ce8.
 *
 * Prologue at 0x02003b90 saves r5, r6, r7, lr and then fp, sl, r9 and r8
 * before `sub sp, #12`; the matching epilogue at 0x02003cd6 restores all of
 * them and ends `pop {r0} / bx r0`, so r0 holds the popped return address and
 * the owner is void.  The twelve bytes of frame are the three-word position
 * scratch that every probe and every move uses; r7 aliases it.  The high
 * registers are caches: r9 = the direction table, sl = 0xffff0000, r8 = the
 * subject record, fp = the constant 0 stored at the end.
 *
 * All 13 `bl` sites are placed and reach seven distinct callees, matching the
 * inventory row's calls=13.  Targets come from cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * (target offset = stored displacement + 2), never from the disassembler's
 * annotations - it prints the three identical Func_02003b48 halfwords, and the
 * two identical Object_SetPosition halfwords, as different callees.  0x3f50 ->
 * Scene_GetRecord, 0x3e98 -> Object_CheckMovementCollision, 0x3e48 -> Object_SetMode (twice),
 * 0x3da0 -> Func_080000c0, 0x3e78 -> Object_SetPosition (twice), 0x40b0 ->
 * Audio_PlayCue (twice), plus three calls to this overlay's own
 * Func_02003b48, whose byte-exact source is
 * games/gs1/assets/code/resource_3bb_c_02003b48.c.
 *
 * That byte-exact sibling settles the interface: Func_02003b48 takes the
 * three-word position and returns the occupying slot pointer or 0.  All three
 * sites here also load r1 with a record pointer; since the callee's own
 * reconstructed source takes a single parameter, the extra register is not
 * asserted as an argument.
 *
 * The pool word 0x02000240 is below the overlay's 0x02008000 link base (base
 * witnessed by 0x02008715 = Func_02000714 + 1 in the byte-exact sibling
 * games/gs1/assets/code/resource_3bb_c_02000950.c), so it is a RAM global holding the
 * active subject id at +500 (250 << 1).  0x0200c3d4 is inside the link band
 * and is therefore in-image data at file offset 0x43d4: a sixteen-entry table
 * indexed by the top nibble of the subject's facing halfword at +6.  Each
 * entry packs two signed 16-bit steps - the high half is added to x directly
 * (`& 0xffff0000`) and the low half is shifted up 16 and added to z.
 *
 * Shape: a push.  Find what occupies the tile one step ahead of the subject;
 * if nothing does, stop.  Refuse the push when the tile beyond that occupant,
 * or the tile above it, is occupied by something whose flag byte at +89 has
 * bit 0 set.  Otherwise mark the occupant with state 2, ask Object_CheckMovementCollision
 * whether the destination is legal (a result greater than zero refuses), and
 * play the slide: both records get 0x3333 in their +48 and +52 speed fields,
 * both are moved with Object_SetPosition, the two sound ids 238 and 288 are played
 * around Object_CommitPosition, and only the occupant's x and z are committed while
 * its +36 and +44 are cleared and the subject is put back into pose 1.
 *
 * Uncertainties: only the record fields at +6, +8, +12, +16, +34, +36,
 * +44, +48, +52 and +89 are asserted.  Both Object_SetPosition calls are issued
 * with the *same* destination, which is what the instructions do - the
 * scratch is not recomputed between them - and the subject's own record
 * position is never written back, so only the occupant's move is committed.
 * The sign of Object_CheckMovementCollision's result is tested with `bgt`, so it is signed.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
SceneRecord *Func_02007afc();
SceneRecord *Func_02007726(Position3 *, SceneRecord *);
SceneRecord *Func_02007750(Position3 *, SceneRecord *);
SceneRecord *Func_0200777c(Position3 *, SceneRecord *);
s32 Func_02007b0e(SceneRecord *, Position3 *);
void Func_02007acc(SceneRecord *, s32);
void Func_02007a2a(s32);
void Func_02007b12(SceneRecord *, s32, s32, s32);
void Func_02007b22(SceneRecord *, s32, s32, s32);
void Func_02007d60(s32);
void Func_02007b36(SceneRecord *);
void Func_02007d6e(s32);
void Func_02007b1c(SceneRecord *, s32);

                     

                     

/* This overlay's own lookup; byte-exact source in games/gs1/assets/code. */

/* In-image direction table at file offset 0x43d4, sixteen packed steps. */
extern u32 Data_0200c3d4[];
extern s16 Data_02000240[];

void Func_02003b90(void)
{
    SceneRecord *subject;
    SceneRecord *target;
    SceneRecord *blocker;
    u32 step;
    u32 direction;
    Position3 position;
    u32 data_index = 250;
    s32 zero;
    s32 subject_handle;

    subject_handle = *(s32 *)((u8 *)Data_02000240 + (data_index << 1));
    subject = Func_02007afc(subject_handle);

    direction = subject->facing >> 12;

    step = Data_0200c3d4[direction];
    position.x = subject->x + (s32)(step & 0xffff0000);
    position.y = subject->y;
    step <<= 16;
    position.z = subject->z + (s32)step;

    target = Func_02007726(&position, subject);
    if (target == 0) {
        return;
    }

    step = Data_0200c3d4[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    blocker = Func_02007750(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    position.x = target->x;
    position.y = target->y + 0x100000;
    position.z = target->z;

    blocker = Func_0200777c(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = Data_0200c3d4[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    if (Func_02007b0e(target, &position) > 0) {
        return;
    }

    Func_02007acc(subject, 8);
    Func_02007a2a(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    Func_02007b12(target, position.x, position.y, position.z);

    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    Func_02007b22(subject, position.x, position.y, position.z);

    Func_02007d60(0xee);
    Func_02007b36(target);
    Func_02007d6e(0x120);

    target->x = position.x;
    target->z = position.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    Func_02007b1c(subject, 1);
}
