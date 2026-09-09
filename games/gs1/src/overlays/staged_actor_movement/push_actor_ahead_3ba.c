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

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
SceneRecord *Func_020075cc();   /* scene record for a subject handle */
/* This overlay's own occupancy lookup for a cell. The record pointer the call
 * sites also load is spelled here, although the lookup itself uses only the
 * position. */
SceneRecord *Func_020071f6(Position3 *, SceneRecord *);
SceneRecord *Func_02007220(Position3 *, SceneRecord *);
SceneRecord *Func_0200724c(Position3 *, SceneRecord *);
s32 Func_020075e6(SceneRecord *, Position3 *);  /* terrain probe */
void Func_02007594(SceneRecord *, s32);         /* select presentation mode */
void Func_020074fa(s32);                        /* wait n frames */
void Func_020075da(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */
void Func_020075ea(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */
void Func_02007810(s32);                        /* play a cue */
void Func_020075fe(SceneRecord *);              /* re-attach the camera */
void Func_0200781e(s32);                        /* play a cue */
void Func_020075e4(SceneRecord *, s32);         /* select presentation mode */

/* In-image direction table: sixteen packed steps, high half x, low half z. */
extern u32 Data_0200c154[];
extern s16 Data_02000240[];

/*
 * The push interaction: probe the cell one step ahead of the subject and, if
 * something occupies it, slide it and the subject one step on. The 360-byte
 * owner includes its four-word literal pool, which ends where the next owner
 * begins. Record fields are asserted only where written -- facing at +6,
 * position at +8/+12/+16, state at +0x22, occupancy flag bit 0 at +0x59 -- and
 * the terrain probe is tested signed, so only a positive code refuses.
 */
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

    /* The same destination block, moved onto the subject this time. */
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
