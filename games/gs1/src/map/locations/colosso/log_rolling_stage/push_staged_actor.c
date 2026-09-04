#include "colosso_log_rolling_stage.h"

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
SceneRecord *Func_0200905c();
SceneRecord *Func_02008c56(Position3 *, SceneRecord *);
SceneRecord *Func_02008c80(Position3 *, SceneRecord *);
SceneRecord *Func_02008cac(Position3 *, SceneRecord *);
s32 Func_0200905e(SceneRecord *, Position3 *);
void Func_02008ffc(SceneRecord *, s32);
void Func_02008f62(s32);
void Func_02009042(SceneRecord *, s32, s32, s32);
void Func_02009052(SceneRecord *, s32, s32, s32);
void Func_020092e8(s32);
void Func_02009066(SceneRecord *);
void Func_020092f6(s32);
void Func_0200904c(SceneRecord *, s32);
                                /* scene record for a subject handle */
                                /* select presentation mode (record, mode) */
                                /* place the record at (x, y, z) */
                                /* re-attach the camera to a record */
                                /* terrain probe; positive result blocks */
                                /* wait n frames */
                                /* play a cue */

/* This overlay's own byte-exact occupancy lookup. */

/* In-image direction table at file offset 0x4154 (0x0200cfc0 - 0x8000):
 * sixteen packed steps, high half x, low half z. */
extern u32 Data_0200cfc0[];
extern s16 Data_02000240[];

void ColossoLogRollingStage_PushStagedActor(void)
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
    subject = Func_0200905c(subject_handle);

    direction = subject->facing >> 12;

    step = Data_0200cfc0[direction];
    position.x = subject->x + (s32)(step & 0xffff0000);
    position.y = subject->y;
    step <<= 16;
    position.z = subject->z + (s32)step;

    target = Func_02008c56(&position, subject);
    if (target == 0) {
        return;
    }

    /* Is the cell one step beyond the target already taken? */
    step = Data_0200cfc0[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    blocker = Func_02008c80(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    /* ...and the cell directly above the target? */
    position.x = target->x;
    position.y = target->y + 0x100000;      /* 128 << 13 */
    position.z = target->z;

    blocker = Func_02008cac(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = Data_0200cfc0[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    if (Func_0200905e(target, &position) > 0) {
        return;
    }

    Func_02008ffc(subject, 8);
    Func_02008f62(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    Func_02009042(target, position.x, position.y, position.z);

    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    Func_02009052(subject, position.x, position.y, position.z);

    Func_020092e8(0xee);
    Func_02009066(target);
    Func_020092f6(0x120);                                /* 144 << 1 */

    target->x = position.x;
    target->z = position.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    Func_0200904c(subject, 1);
}
