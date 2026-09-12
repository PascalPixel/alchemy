#include "types.h"

#define StagedActor_PushActorAhead Func_02003b90
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

extern u32 Data_0200c3d4[];
extern s16 Data_02000240[];

SceneRecord *Func_02007726(Position3 *, SceneRecord *);
SceneRecord *Func_02007750(Position3 *, SceneRecord *);
SceneRecord *Func_0200777c(Position3 *, SceneRecord *);

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

/* This overlay's own occupancy lookup for a cell. The record pointer the call
 * sites also load is spelled here, although the lookup itself uses only the
 * position. */

/* In-image direction table: sixteen packed steps, high half x, low half z. */

SceneRecord *Func_02007afc();   /* scene record for a subject handle */

s32 Func_02007b0e(SceneRecord *, Position3 *);  /* terrain probe */

void Func_02007acc(SceneRecord *, s32);         /* select presentation mode */

void Func_02007a2a(s32);                        /* wait n frames */

void Func_02007b12(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */

void Func_02007b22(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */

void Func_02007d60(s32);                        /* play a cue */

void Func_02007b36(SceneRecord *);              /* re-attach the camera */

void Func_02007d6e(s32);                        /* play a cue */

void Func_02007b1c(SceneRecord *, s32);         /* select presentation mode */

/*
 * The push interaction: probe the tile one step ahead of the subject and, if
 * something occupies it, slide it and the subject one step on. Only the
 * occupant's position is committed. The 360-byte owner includes its four-word
 * literal pool. Record fields are asserted only where written: facing at +6,
 * position at +8/+12/+16, state at +34, occupancy flag bit 0 at +89, speeds
 * at +48/+52, and the words cleared at +36/+44.
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
    subject = Func_02007afc(handle);

    dir = subject->facing >> 12;

    step = Data_0200c3d4[dir];
    pos.x = subject->x + (s32)(step & 0xffff0000);
    pos.y = subject->y;
    step <<= 16;
    pos.z = subject->z + (s32)step;

    target = Func_02007726(&pos, subject);
    if (target == 0) {
        return;
    }

    step = Data_0200c3d4[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    blocker = Func_02007750(&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    pos.x = target->x;
    pos.y = target->y + 0x100000;
    pos.z = target->z;

    blocker = Func_0200777c(&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = Data_0200c3d4[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    /* Terrain probe: signed, so only a positive code refuses the move. */
    if (Func_02007b0e(target, &pos) > 0) {
        return;
    }

    Func_02007acc(subject, 8);
    Func_02007a2a(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    Func_02007b12(target, pos.x, pos.y, pos.z);

    /* The same destination block, moved onto the subject this time. */
    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    Func_02007b22(subject, pos.x, pos.y, pos.z);

    Func_02007d60(0xee);
    Func_02007b36(target);
    Func_02007d6e(0x120);

    target->x = pos.x;
    target->z = pos.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    Func_02007b1c(subject, 1);
}
