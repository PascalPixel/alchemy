#include "TYPES.H"

/* An event actor turns its back on the leader, hops three times, splits
   into eight linked copies of itself and leaps away in the leader's
   direction, then comes to rest with its motion cleared. */

struct HopSprite {
    u8 unknown_00[8];
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[30];
    s16 *resource;
};

struct HopActor {
    u8 unknown_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[3];
    u8 priority_flags;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    u8 unknown_30[8];
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unknown_44[12];
    struct HopSprite *sprite;
    u8 unknown_54;
    u8 mode;
    u8 unknown_56[4];
    u8 collision_flags;
    u8 unknown_5b[13];
    struct HopActor *linked;
    void *callback;
};

extern s32 gGameState[];

struct HopActor *Object_GetById(s32 id);
void Battle_Reset(void);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void Motion_SetVarCbAndRefresh(s32 id, s32 value);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void Motion_SetTargetPositionFromMagnitudeAngle(struct HopActor *actor, s32 magnitude, s32 angle);
void ObjectMotion_Launch(s32 id, s32 height, s32 frames);
void Object_CommitPosition(struct HopActor *actor);
struct HopActor *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void Animation_ApplyChildValuesFar(struct HopActor *actor, s32 value);
void Object_SetMode(struct HopActor *actor, s32 mode);
void ObjectDispatch_SetSingleChildField26Far(struct HopActor *actor, s32 value);
void *Object_ReplaceResourceEntry(struct HopSprite *sprite, void *resource);
void BattleFx_FinishAction(void);

void EventActor_RunHopAndLeapSequence(s32 arg)
{
    struct HopActor *actor;
    struct HopActor *copy;
    struct HopActor *previous;
    void *resource;
    s32 angle;
    s32 back;
    s32 hop;
    s32 kind;
    s32 value;
    s32 i;
    struct HopActor *copies[8];

    actor = Object_GetById(arg);
    angle = (Object_GetById(gGameState[125])->facing + 0x2000) & 0xc000;
    Battle_Reset();
    WaitFrames(10);
    Audio_PlayCue(173);
    Motion_SetVarCbAndRefresh(arg, 1);
    Audio_PlayCue(175);
    Motion_SetVarCbAndRefresh(arg, 1);
    back = angle + 0x8000;
    WaitFrames(20);
    ObjectMotion_ArmCallback(arg, back, 0);
    WaitFrames(10);
    actor->sprite->priority = 0;
    actor->facing = back;
    Object_GetById(arg)->collision_flags &= ~1;
    actor->mode = 2;
    hop = 0x100000;
    Motion_SetTargetPositionFromMagnitudeAngle(actor, hop, angle);
    Audio_PlayCue(152);
    ObjectMotion_Launch(arg, 4, 0);
    Object_CommitPosition(actor);
    Motion_SetTargetPositionFromMagnitudeAngle(actor, hop, angle);
    Audio_PlayCue(152);
    ObjectMotion_Launch(arg, 4, 0);
    Object_CommitPosition(actor);
    Motion_SetTargetPositionFromMagnitudeAngle(actor, hop, angle);
    Audio_PlayCue(152);
    ObjectMotion_Launch(arg, 4, 0);
    Object_CommitPosition(actor);
    WaitFrames(20);

    kind = *actor->sprite->resource;
    value = 9;
    if (kind == 90)
        value = 2;
    if (kind == 92)
        value = 10;
    if (kind == 91)
        value = 9;
    resource = NULL;
    previous = actor;
    for (i = 0; i <= 7; i++) {
        copy = Object_CreateFar(kind, actor->x, actor->y, actor->z);
        copies[i] = copy;
        if (copy != NULL) {
            copy->scale_y = 0xf000;
            copy->scale_x = 0xf000;
            copy->mode = 0;
            copy->priority_flags = 2;
            copy->collision_flags |= 1;
            copy->callback = (void *)0x08095349;
            copy->facing = actor->facing;
            copy->sprite->priority = 0;
            Animation_ApplyChildValuesFar(copy, value);
            Object_SetMode(copy, 0);
            ObjectDispatch_SetSingleChildField26Far(copy, 0);
            resource = Object_ReplaceResourceEntry(copy->sprite, resource);
            copy->linked = previous;
            previous = copy;
        }
    }

    Motion_SetTargetPositionFromMagnitudeAngle(actor, 0x400000, angle + 0x8000);
    Audio_PlayCue(136);
    ObjectMotion_Launch(arg, 12, 0);
    WaitFrames(24);
    actor->mode = 0;
    actor->velocity_x = 0;
    actor->velocity_z = 0;
    actor->velocity_y = 0;
    actor->target_x = 0x80000000;
    actor->target_z = 0x80000000;
    actor->target_y = 0x80000000;
    Object_SetMode(actor, 0);
    actor->sprite->priority = 2;
    BattleFx_FinishAction();
}
