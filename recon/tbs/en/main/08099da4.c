/* DRAFT of Battle_unk3_2: the effect that lets the player steer a lifted
   target between two item-break anchors, drop it where it fits, and then
   break the anchors into twenty shards.
   Not exact: written from the listing, the frame and the roles of r7, r8,
   r10 and r11 as the reference has them. What gave them: the position is
   reached through a function-level pointer everywhere but in the steering
   loop's first part; the anchors are an array re-read after each call; the
   second anchor is copied to a variable before the loop; a fits flag orders
   the blocked and drop paths; one variable holds the overlap result and
   the counters. Remaining: the reference keeps the first part's anchors in
   r6 and r5 and the lift step in r1, saved around the call; here the step
   outranks the anchors, takes r5, and sends the first anchor to r8. */
#include "TYPES.H"

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

struct EffectWork {
    u8 unknown_00[4];
    s32 x;
    s32 y;
    s32 z;
    struct FxObject *source;    /* 0x10 */
    struct FxObject *target;    /* 0x14 */
    u8 unknown_18[0x1c];
    u8 falls;                   /* 0x34 */
    u8 unknown_35[3];
    void *callback;             /* 0x38 */
    const u8 *script;           /* 0x3c */
    u8 unknown_40[4];
    u8 palette;                 /* 0x44 */
};

extern struct EffectWork *gEffectWork;
extern const u8 Data_0809f0bc[];
extern const u8 BattleFx_FragmentScript[];
extern volatile u32 gKeysHeld;
extern volatile u32 gKeyState;
extern volatile u32 gFrameCount;

void BattleEffect_InitializeSharedScene(void);
void ObjectDispatch_InitializeFar(struct FxObject *object, const u8 *script);
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

void Battle_unk3_2(void)
{
    struct EffectWork *work;
    struct FxObject *source;
    struct FxObject *target;
    struct FxObject *shards[20];
    struct FxPosition position;
    struct FxObject *anchors[2];
    struct FxObject *left;
    struct FxObject *right;
    struct FxObject *second;
    struct FxObject *shard;
    struct FxObject **walk;
    s32 i;
    s32 angle;
    s32 magnitude;
    s32 fits;
    struct FxPosition *p;

    work = gEffectWork;
    source = work->source;
    target = work->target;
    if (target == NULL)
        return;
    BattleEffect_InitializeSharedScene();
    source->linked = target;
    ObjectDispatch_InitializeFar(source, Data_0809f0bc);
    p = &position;
    p->x = work->x;
    p->y = work->y + 0x100000;
    p->z = work->z;
    left = BattleFx_SpawnItemBreakMode1(p->x + 0x200000, p->y, p->z, 0x8000);
    anchors[0] = left;
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
    Object_SetMoveTargetFar(left, p->x + 0x100000, p->y, p->z);
    Object_SetMoveTargetFar(right, p->x - 0x100000, p->y, p->z);
    Object_CommitPosition(left);
    Object_CommitPosition(right);
    left->x = p->x + 0x100000;
    left->velocity_x = 0;
    right->x = p->x - 0x100000;
    right->velocity_x = 0;
    target->callback = ObjectGroup_ApplyRandomChildValues;
    Scheduler_AddOrUpdateCallback(BattleFx_SpawnFallingParticles, 0xc80);
    AudioCommand_PlayFar(130);
    target->flag = 4;
    ObjectDispatch_SetSingleChildField26Far(target, 0);
    if (anchors[0] != NULL && anchors[1] != NULL) {
        while (target->y - target->ground <= 0x180000) {
            left->y += 0x6000;
            right->y += 0x6000;
            target->y += 0x6000;
            WaitFrames(1);
        }
    }
    anchors[0]->speed = 0x40000;
    anchors[0]->lift = 0x8000;
    second = anchors[1];
    second->speed = 0x40000;
    second->lift = 0x8000;
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
        fits = 0;
        if (i == 0) {
            s32 hit;

            target->ground += 0x100000;
            hit = Object_CheckMovementCollision(target, p);
            target->ground -= 0x100000;
            if (hit <= 0)
                fits = 1;
        }
        if (fits == 0) {
            Object_SetMode(anchors[0], 4);
            Object_SetMode(anchors[1], 4);
            if ((gFrameCount & 15) == 0)
                AudioCommand_PlayFar(114);
        } else {
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
    walk = shards;
    for (i = 19; i >= 0; i--) {
        shard = Object_Spawn(0x11d, p->x, p->y, p->z);
        *walk++ = shard;
        if (shard != NULL) {
            ObjectDispatch_InitializeFar(shard, BattleFx_FragmentScript);
            shard->speed = Random16() + 0x20000;
            shard->lift = 0x20000;
            shard->flag = 0;
            magnitude = Random16() * 24 + 0x80000;
            Motion_SetTargetPositionFromMagnitudeAngle(shard, magnitude, Random16());
        }
    }
    AudioCommand_PlayFar(131);
    ObjectDispatch_ReleaseFar(anchors[0]);
    ObjectDispatch_ReleaseFar(anchors[1]);
    Animation_ApplyChildValuesFar(target, work->palette);
    ObjectDispatch_InitializeFar(target, work->script);
    target->callback = work->callback;
    target->flag = 3;
    target->velocity_y = 0xa0000;
    target->gravity = 0x3333;
    target->mode = 0;
    source->callback = 0;
    Animation_ApplyChildValuesFar(source, 0);
    if ((s8)work->falls != 0) {
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
