#include "TYPES.H"
#include "IWRAM_CALL.H"
#define gGameState Data_02000240
#define gEventWork Data_03001ebc

/* main:08093c00 Field_TryJumpForward - hand-written draft, 57 of 276
   halfwords differ, all one register swap: the ROM keeps the leader in r6
   and the tile mask and rise step in r5, where this C allocates them the
   other way round. The code is otherwise the ROM's.

   Jumps the leader two tiles forward when the tile ahead is blocked but
   the one beyond is free: the leader hops over, lands, and a ledge target
   in front shakes as it goes; landing on a hole tile repeats the jump.
   Returns 0 after a jump, -1 when the way is blocked. */

struct JumpSprite {
    u8 unknown_00[38];
    u8 child_mode;
};

struct JumpActor {
    u8 unknown_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    s32 shadow_y;
    u8 unknown_18[10];
    u8 layer;
    u8 unknown_23[5];
    s32 gravity;
    u8 unknown_2c[4];
    s32 speed;
    s32 acceleration;
    u8 unknown_38[24];
    struct JumpSprite *sprite;
    u8 kind;
    u8 motion_flags;
};

struct JumpWork {
    u8 unknown_000[432];
    s32 step_rate;
    s32 steps;
};

struct JumpPosition {
    s32 x;
    s32 y;
    s32 z;
};

extern s32 gGameState[];
extern struct JumpWork *gEventWork;

struct JumpActor *Object_GetById(s32 id);
void Vector_AddPolarOffset(s32 distance, s32 angle, struct JumpPosition *position);
s32 Object_CheckMovementCollision(struct JumpActor *actor, struct JumpPosition *position);
void Battle_Reset(void);
void Object_SetMode(struct JumpActor *actor, s32 mode);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void ObjectDispatch_SetSingleChildField26Far(struct JumpActor *actor, s32 value);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
struct JumpActor *Object_FindNearestFacingTarget(struct JumpActor *actor, s32 kind);
void BattleFx_FinishAction(void);
s32 Func_080091b0(s32 layer, s32 x, s32 z);

s32 Field_TryJumpForward(void)
{
    struct JumpActor *leader = Object_GetById(gGameState[125]);
    s32 result = -1;
    s32 angle = (leader->facing + 0x2000) & 0xc000;
    u8 *flags = &leader->motion_flags;
    u8 saved = *flags;
    struct JumpWork *work = gEventWork;
    s32 child_mode = 1;
    struct JumpPosition position;
    struct JumpPosition *pos;
    struct JumpActor *target;
    s32 rise;

    pos = &position;
again:
    rise = 0xfff00000;
    pos->x = (leader->x & rise) + 0x80000;
    pos->y = leader->y;
    pos->z = (leader->z & rise) + 0x80000;
    Vector_AddPolarOffset(0x100000, angle, pos);
    if (Object_CheckMovementCollision(leader, pos) == 1)
        return -1;
    pos->x = (leader->x & rise) + 0x80000;
    pos->y = leader->y;
    pos->z = (leader->z & rise) + 0x80000;
    Vector_AddPolarOffset(0x200000, angle, pos);
    if (Object_CheckMovementCollision(leader, pos) != 0)
        goto done;
    if (leader->kind == 1)
        child_mode = leader->sprite->child_mode;
    Battle_Reset();
    Object_SetMode(leader, 6);
    WaitFrames(6);
    Audio_PlayCue(152);
    Object_SetMode(leader, 7);
    leader->speed = 0x30000;
    leader->acceleration = 0x20000;
    leader->gravity = 0x40000;
    *flags &= 0x7e;
    ObjectDispatch_SetSingleChildField26Far(leader, child_mode & 0xfe);
    ObjectMotion_SetPositionAndCommit(gGameState[125], *(s16 *)((u8 *)pos + 2), *(s16 *)((u8 *)pos + 10));
    Object_SetMode(leader, 6);
    ObjectDispatch_SetSingleChildField26Far(leader, child_mode);
    if ((target = Object_FindNearestFacingTarget(leader, 207)) != NULL
        || (target = Object_FindNearestFacingTarget(leader, 205)) != NULL) {
        Object_SetMode(target, 7);
        rise = 0xffff0000;
        leader->y += rise;
        leader->shadow_y += rise;
        WaitFrames(2);
        leader->y += rise;
        leader->shadow_y += rise;
        WaitFrames(10);
        rise = 0x10000;
        leader->y += rise;
        leader->shadow_y += rise;
        WaitFrames(4);
        leader->y += rise;
        leader->shadow_y += rise;
    } else {
        WaitFrames(6);
    }
    *flags = saved;
    BattleFx_FinishAction();
    if (work != NULL)
        work->steps += Iwram_MulQ16(work->step_rate, 0x200000);
    if (Func_080091b0(leader->layer, pos->x, pos->z) == 0xf9) {
        Object_SetMode(leader, 1);
        WaitFrames(6);
        goto again;
    }
    result = 0;
done:
    return result;
}
