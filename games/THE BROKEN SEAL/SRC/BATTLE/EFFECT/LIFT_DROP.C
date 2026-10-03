/*
 * Battle effect 11's lift: two item-break anchors carry the target while the
 * player steers it, drop it where it fits, and break into twenty shards.
 */
#include "TYPES.H"
#include "FX_SCENE.H"
#include "OBJDISP.H"

struct FxPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct FxObject {
    u8 unknown_00[8];
    s32 x;                      /* 0x08 */
    s32 y;
    s32 z;
    s32 ground;                 /* 0x14 */
    u8 unknown_18[10];
    u8 mode;                    /* 0x22 */
    u8 unknown_23;
    s32 velocity_x;             /* 0x24 */
    s32 velocity_y;             /* 0x28 */
    s32 velocity_z;             /* 0x2c */
    s32 speed;                  /* 0x30 */
    s32 lift;                   /* 0x34 */
    u8 unknown_38[12];
    s32 gravity;                /* 0x44 */
    u8 unknown_48[13];
    u8 flag;                    /* 0x55 */
    u8 unknown_56[4];
    u8 unknown_5a;
    u8 unknown_5b;
    u8 unknown_5c[12];
    struct FxObject *linked;    /* 0x68 */
    void *callback;             /* 0x6c */
};

extern struct BattleFxScene *gEffectWork;
extern const u8 BattleFx_SourceHoldScript[];
extern const u8 BattleFx_FragmentScript[];
extern volatile u32 gKeysHeld;
extern volatile u32 gKeyState;
extern volatile u32 gFrameCount;

void BattleEffect_InitializeSharedScene(void);
struct FxObject *BattleFx_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle);
void BattleFx_PrepareBufferInterpolation(void);
void WaitFrames(s32 frames);
void Object_SetMoveTargetFar(struct FxObject *object, s32 x, s32 y, s32 z);
void Object_CommitPosition(struct FxObject *object);
void ObjectGroup_ApplyRandomChildValues(void);
void BattleFx_SpawnFallingParticles(void);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 key);
void Scheduler_RemoveCallback(void (*callback)(void));
void AudioCommand_PlayFar(s32 cue);
void ObjectDispatch_SetSingleChildField26Far(struct FxObject *object, s32 value);
s32 BattleFx_GetCycledTableWord(u32 keys);
void Object_SetMode(struct FxObject *object, s32 mode);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct FxPosition *position);
s32 ScriptObject_CheckOverlapFar(struct FxObject *object, struct FxPosition *position);
s32 Object_CheckMovementCollision(struct FxObject *object, struct FxPosition *position);
struct FxObject *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
s32 Random16(void);
void Motion_SetTargetPositionFromMagnitudeAngle(struct FxObject *object, s32 magnitude, s32 angle);
void ObjectDispatch_ReleaseFar(struct FxObject *object);
void Animation_ApplyChildValuesFar(struct FxObject *object, s32 value);
void BattleFx_SpawnRadialParticleRing(struct FxObject *object);

/* Tests the object's movement from one step above the ground. */
static __inline__ s32 Object_ProbeRaised(struct FxObject *object, struct FxPosition *position)
{
    s32 hit;

    object->ground += 0x100000;
    hit = Object_CheckMovementCollision(object, position);
    object->ground -= 0x100000;
    return hit;
}

/* Lifts the target between two anchors and lets the keys steer it: each
   frame the anchors follow, and a direction is tried one step ahead. A step
   that is blocked shakes the anchors; one that fits sets the target down.
   Either way the anchors then break into twenty shards and the target takes
   its own script back, falling if the effect says so. */
void BattleFx_SteerLiftedTarget(s32 target_id)
{
    struct BattleFxScene *work;
    struct FxObject *source;
    struct FxObject *target;
    struct FxObject *shards[20];
    struct FxPosition position;
    struct FxObject *anchors[2];
    struct FxObject *object;
    struct FxObject *right;
    s32 i;
    s32 angle;
    s32 magnitude;
    struct FxPosition *p;

    work = gEffectWork;
    source = work->main_object;
    target = work->child;
    if (target == NULL)
        return;
    BattleEffect_InitializeSharedScene();
    source->linked = target;
    ObjectDispatch_InitializeFar((struct DispatchObject *)source, (u32)BattleFx_SourceHoldScript);
    /* FAKEMATCH: the position is reached through this pointer everywhere but
       in the steering loop's first part, which names the position itself;
       that keeps the pointer in r10 and gives the loop its own copy. */
    p = &position;
    p->x = work->x;
    p->y = work->y + 0x100000;
    p->z = work->z;
    object = BattleFx_SpawnItemBreakMode1(p->x + 0x200000, p->y, p->z, 0x8000);
    anchors[0] = object;
    right = BattleFx_SpawnItemBreakMode1(p->x - 0x200000, p->y, p->z, 0);
    anchors[1] = right;
    if (anchors[0] == NULL || right == NULL) {
        BattleFx_PrepareBufferInterpolation();
        return;
    }
    WaitFrames(15);
    p->x = target->x;
    p->y = target->y + 0x100000;
    p->z = target->z;
    Object_SetMoveTargetFar(object, p->x + 0x100000, p->y, p->z);
    Object_SetMoveTargetFar(right, p->x - 0x100000, p->y, p->z);
    Object_CommitPosition(object);
    Object_CommitPosition(right);
    object->x = p->x + 0x100000;
    object->velocity_x = 0;
    right->x = p->x - 0x100000;
    right->velocity_x = 0;
    target->callback = ObjectGroup_ApplyRandomChildValues;
    Scheduler_AddOrUpdateCallback(BattleFx_SpawnFallingParticles, 0xc80);
    AudioCommand_PlayFar(130);
    target->flag = 4;
    ObjectDispatch_SetSingleChildField26Far(target, 0);
    if (anchors[0] != NULL && anchors[1] != NULL) {
        while (target->y - target->ground <= 0x180000) {
            object->y += 0x6000;
            right->y += 0x6000;
            target->y += 0x6000;
            WaitFrames(1);
        }
    }
    anchors[0]->speed = 0x40000;
    anchors[0]->lift = 0x8000;
    anchors[1]->speed = 0x40000;
    anchors[1]->lift = 0x8000;
    target->speed = 0x6666;
    target->lift = 0x3333;
    target->unknown_5a = 0;
    target->mode = 2;
    for (;;) {
        WaitFrames(1);
        if ((gKeyState & 0x303) != 0)
            break;
        angle = (u16)BattleFx_GetCycledTableWord(gKeysHeld);
        if (angle == 0xffff) {
            position.x = target->x;
            position.y = target->y + 0x100000;
            position.z = target->z;
            Object_SetMoveTargetFar(anchors[0], position.x + 0x100000, position.y, position.z);
            Object_SetMoveTargetFar(anchors[1], position.x - 0x100000, position.y, position.z);
            Object_SetMode(anchors[0], 1);
            Object_SetMode(anchors[1], 1);
            continue;
        }
        position.x = target->x;
        position.y = target->y + 0x100000;
        position.z = target->z;
        Vector_AddPolarOffset(0x20000, angle, &position);
        Object_SetMoveTargetFar(anchors[0], position.x + 0x100000, position.y, position.z);
        Object_SetMoveTargetFar(anchors[1], position.x - 0x100000, position.y, position.z);
        Object_CommitPosition(anchors[0]);
        Object_CommitPosition(anchors[1]);
        position.x = target->x;
        position.y = target->ground;
        position.z = target->z;
        Vector_AddPolarOffset(0x100000, angle, &position);
        i = ScriptObject_CheckOverlapFar(target, &position);
        /* FAKEMATCH: entering the blocked path from both tests keeps it ahead
           of the drop path, as the ROM lays them out. */
        if (i != 0)
            goto blocked;
        if (Object_ProbeRaised(target, p) > 0) {
        blocked:
            Object_SetMode(anchors[0], 4);
            Object_SetMode(anchors[1], 4);
            if ((gFrameCount & 15) == 0)
                AudioCommand_PlayFar(114);
            continue;
        }
        {
            struct FxObject *second;
            s32 x;
            s32 z;

            AudioCommand_PlayFar(175);
            x = p->x;
            z = p->z;
            Object_SetMode(anchors[0], 4);
            Object_SetMode(anchors[1], 4);
            WaitFrames(15);
            target->unknown_5b = i;
            target->speed = 0x3333;
            target->lift = 0x3333;
            Object_SetMoveTargetFar(target, p->x, p->y, p->z);
            anchors[0]->speed = 0x3333;
            anchors[0]->lift = 0x3333;
            second = anchors[1];
            second->speed = 0x3333;
            second->lift = 0x3333;
            Object_SetMoveTargetFar(anchors[0], p->x + 0x100000, p->y, p->z);
            Object_SetMoveTargetFar(second, p->x - 0x100000, p->y, p->z);
            Object_CommitPosition(target);
            target->x = x;
            target->z = z;
            target->velocity_x = i;
            target->velocity_z = i;
            WaitFrames(10);
            break;
        }
    }
    Object_SetMode(anchors[0], 4);
    Object_SetMode(anchors[1], 4);
    Scheduler_RemoveCallback(BattleFx_SpawnFallingParticles);
    AudioCommand_PlayFar(135);
    WaitFrames(15);
    AudioCommand_PlayFar(135);
    WaitFrames(15);
    p->x = target->x;
    p->y = target->y + 0x100000;
    p->z = target->z;
    {
        struct FxObject **walk = shards;

        for (i = 0; i < 20; i++) {
            object = Object_Spawn(0x11d, p->x, p->y, p->z);
            *walk++ = object;
            if (object != NULL) {
                ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_FragmentScript);
                object->speed = Random16() + 0x20000;
                object->lift = 0x20000;
                object->flag = 0;
                magnitude = Random16() * 24 + 0x80000;
                Motion_SetTargetPositionFromMagnitudeAngle(object, magnitude, Random16());
            }
        }
    }
    AudioCommand_PlayFar(131);
    ObjectDispatch_ReleaseFar(anchors[0]);
    ObjectDispatch_ReleaseFar(anchors[1]);
    Animation_ApplyChildValuesFar(target, work->saved_palette);
    ObjectDispatch_InitializeFar((struct DispatchObject *)target, (u32)(work->saved_script));
    {
        /* FAKEMATCH: one zero, set before these stores, clears the mode
           byte and the source's callback from the same register. */
        s32 zero = 0;

        target->callback = work->saved_callback;
        target->flag = 3;
        target->velocity_y = 0xa0000;
        target->gravity = 0x3333;
        target->mode = zero;
        source->callback = (void *)zero;
    }
    Animation_ApplyChildValuesFar(source, 0);
    if ((s8)work->child_option != 0) {
        for (i = 0; i < 90 && target->velocity_y >= 0; i++)
            WaitFrames(1);
        WaitFrames(1);
        for (i = 0; i < 90 && target->velocity_y < 0; i++)
            WaitFrames(1);
        BattleFx_SpawnRadialParticleRing(target);
        BattleFx_PrepareBufferInterpolation();
        WaitFrames(30);
    } else {
        BattleFx_PrepareBufferInterpolation();
    }
}
