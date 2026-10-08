#include "EDITION.H"
#include "BABI.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
#include "FIELD_EFFECT.H"

#define FIELD_STAGED_ACTOR_IMPORTS
extern u8 *gActorEffectWork;

s32 SceneState_ApplyArgMode0AndReturnZero(s32 no)
{
    Engine_ActorSetSpriteFlags(no, 0);
    return 0;
}

s32 SceneActor_MoveActorZeroToTarget(const Target_02000cd0 *target)
{
    Actor_02000cd0 *actor = Object_GetById(ACTOR_PARTY_LEADER);
    u8 saved = actor->flags;
    s32 probe[3];

    probe[0] = (actor->x & 0xfff00000) + 0x00080000;
    probe[1] = actor->y;
    probe[2] = (actor->z & 0xfff00000) + 0x00080000;

    Vector_AddPolarOffset(0x00100000, (actor->tag + 0x2000) & 0xc000, probe);

    if (Object_CheckMovementCollision(actor, probe) == 1) {
        goto refuse;
    }
    if (Object_CheckMovementCollision(actor, target) != 0) {
        goto refuse;
    }

    Engine_EventBegin();
    Object_SetMode(actor, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(152);
    Object_SetMode(actor, 7);

    actor->speedX = 0x00030000;
    actor->speedY = 0x00020000;
    actor->speedZ = 0x00040000;
    actor->flags &= (u8)0x7e;

    Engine_ActorSetSpriteFlags(actor, 0);
#if EDITION_INTERNATIONAL
    Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, ((target->x.fixed >> 20) << 4) + 8, ((target->z.fixed >> 20) << 4) + 8);
#else
    Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, target->x.part.pixel, target->z.part.pixel);
#endif
    Object_SetMode(actor, 6);
    Engine_ActorSetSpriteFlags(actor, 1);
    Engine_TaskWait(6);

    actor->flags = saved;
    Engine_EventEnd();
    return 0;

refuse:
    return 1;
}

void SceneActor_PassRaisedPointOfActorZero(void)
{
    s32 pos[3];
    struct FieldActor *p = Object_GetById(ACTOR_PARTY_LEADER);

    pos[0] = p->x.fixed;
    pos[1] = p->y.fixed;
    pos[2] = p->z.fixed + 0x200000;
    SceneActor_MoveActorZeroToTarget(pos);
}

void SceneActor_PassActorZeroOffsetPoint(void)
{
    s32 pos[3];
    struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

    pos[0] = actor->x.fixed;
    pos[1] = actor->y.fixed;
    pos[2] = actor->z.fixed + 0xFFE00000;
    SceneActor_MoveActorZeroToTarget(pos);
}

void BabiChika_UpdateTrackedActor(void)
{
    struct FieldActor *actor;
    u8 *work;
    s32 limit;

    actor = Object_GetById(0);
    work = gActorEffectWork;
    limit = 0;
    if (gGameState.scene == (s32)&SceneId_BabiChika1) {
        switch (gGameState.entrance) {
        case 3:
        case 4:
            limit = 94;
            break;
        case 8:
        case 9:
            limit = 74;
            break;
        case 12:
        case 13:
            limit = 118;
            break;
        }
    } else if (gGameState.entrance == 12) {
        limit = 93;
    }
    if ((actor->z.fixed >> 19) <= limit) {
        *(s32 *)(work + 24) = 0;
    } else {
        *(s32 *)(work + 24) = (s32)actor;
    }
}

s32 SceneActor_SetFlagBitByRelativeDepth(struct Actor_02000ec8 *actor)
{
    struct Actor_02000ec8 *ref;
    u8 *fp;
    u8 flag;
    ref = Object_GetById(ACTOR_PARTY_LEADER);
    fp = &actor->flatla3;
    flag = *fp | 2;
    *fp = flag;
    if (ref->z < actor->z) {
        s32 lim = actor->z - ref->z;
        s32 ay;
        lim += 0x00040000;
        ay = actor->y;
        ay += lim;
        if (ref->y <= ay) {
            flag &= 0xfd;
            *fp = flag;
        }
    }
    return 0;
}

void SceneState_SwapSlotPairByRank(s32 first, s32 second)
{
    struct Slot02000f10 *a = Object_GetById(first);
    struct Slot02000f10 *b = Object_GetById(second);

    if (a->rank <= b->rank) {
        s32 t;

        t = a->x;    a->x    = b->x;    b->x    = t;
        t = a->y;    a->y    = b->y;    b->y    = t;
        t = a->rank; a->rank = b->rank; b->rank = t;
        Engine_TaskWait(1);
    }
}

s32 BabiChika_UpdateFlickerEffect(struct FieldActor *object)
{
    struct EffectOptions options;
    s32 x;
    s32 z;

    if (gFrameCount & 2)
        Object_SetMode(object, 1);
    else
        Object_SetMode(object, 2);
    if (gFrameCount & 3)
        return 0;
    options.start_scale_x = 0x4ccc;
    options.start_scale_y = 0x4ccc;
    options.palette = 5;
    x = object->x.fixed + ((((u32)(Engine_RandomNext() * 7) >> 16) - 3) << 16);
    z = object->z.fixed + ((((u32)(Engine_RandomNext() * 7) >> 16) - 3) << 16);
    Effect_Spawn(x, object->y.fixed + 0x100000, z, 0, 0, 0, 0x90001, &options);
    return 0;
}

s32 SceneActor_CopyActor8PositionWhenAtRow10(Record *record)
{
    Record *ref = Object_GetById(ACTOR_PARTY_LEADER);

    if (ref->w12 > -0x300000
        && (Object_GetById(8)->z.fixed >> 20) == 10) {
        record->w8 = Object_GetById(8)->x.fixed;
        record->w12 = -0x200000;
        record->w16 = Object_GetById(8)->z.fixed;
    } else {
        record->w8 = 0;
        record->w12 = 0;
        record->w16 = 0;
    }
    return 0;
}

void SceneActor_WaitValueBelowLimit(struct Track02001038 *track)
{
    s32 cnt = 60;
    s32 limit;

    for (;;) {
        if (cnt != 0) {
            s32 value;

            Engine_TaskWait(1);
            value = track->value;
            limit = track->limit;
            cnt--;
            if (value <= limit) {
                break;
            }
            continue;
        }
        limit = track->limit;
        break;
    }
    track->state = 0;
    track->value = limit;
    track->mark = (s32) 0x80000000;
}

void Effect_AdvanceMotion(struct MotionEffect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Math_Divide(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

void BabiChika_RunLeaderSpray(void)
{
    struct FieldActor *leader = Object_GetById(0);
    struct EffectOptions options;
    u32 i;

    Engine_EventBegin();
    Object_SetMode(leader, 6);
    ObjectMotion_WaitForAnimationChange(0);
    Object_SetMode(leader, 1);
    Engine_ActorSetSpriteFlags(leader, 0);
    leader->motion_flags |= 2;
    Engine_AudioPlayCue(152);
    leader->velocity_y = 0x40000;
    Engine_ObjectSetPosition(leader, leader->x.fixed, leader->y.fixed, leader->z.fixed + 0xc0000);
    Engine_TaskWait(6);
    leader->motion_flags = 0;
    options.update = (void (*)(union FieldObject *))Effect_AdvanceMotion;
    Engine_AudioPlayCue(127);
    for (i = 0; i < 8; i++) {
        leader->y.fixed -= 0x20000;
        leader->target_y = leader->y.fixed;
        Engine_TaskWait(1);
        if (i & 1) {
            s32 m = (u32)Engine_RandomNext() % 10 - 5;
            s32 vx = m * 0x3332;
            s32 vz;

            vz = (u32)Engine_RandomNext() % 10 * -0x1999 - 0x7ffd;
            Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, vx, 0, vz, 0x1000001, &options);
        }
    }
    Engine_ActorSetSpriteFlags(leader, 1);
    leader->motion_flags = 3;
    Engine_EventEnd();
}
