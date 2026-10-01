/* Moving and positioning actor zero. */
#include "BABI.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
#include "FIELD_EFFECT.H"

#define FIELD_STAGED_ACTOR_IMPORTS
extern u8 *gActorEffectWork;

/*
 * resource_3c4 @ 0x02001f70 (84 bytes: 72 code + alignment + two pool words).
 *
 * Publishes selector 0x974 for slot 17 and 0x975 for slot 18, choosing a
 * different publisher for each depending on whether that slot's +8 word sits
 * at 12.20 row 45 and 46 respectively.  `asrs #20` makes both tests signed.
 * Both pool words are selectors, not addresses.
 *
 * `pop {r0} ; bx r0` return: void.
 */
s32 SceneState_ApplyArgMode0AndReturnZero(s32 no)
{
    Engine_ActorSetSpriteFlags(no, 0);
    return 0;
}

/*
 * Tries to move actor 0 onto the caller's target.  It builds a three-word
 * 12.20 probe from the record's own position -- the horizontal words snapped
 * to their whole-unit grid and lifted by half a unit -- asks the collision
 * service about it, and refuses when either the probe or the target is
 * rejected.  Returns 1 when refused and 0 when the move ran.  The 248-byte
 * owner includes its one pool word.
 */
s32 SceneActor_MoveActorZeroToTarget(const Target_02000cd0 *target)
{
    Actor_02000cd0 *actor = Actor_Get(ACTOR_PARTY_LEADER);
    u8 saved = actor->flags;
    s32 probe[3];

    probe[0] = (actor->x & (s32)0xfff00000) + 0x00080000;
    probe[1] = actor->y;
    probe[2] = (actor->z & (s32)0xfff00000) + 0x00080000;

    Vector_AddPolarOffset(0x00100000, (actor->tag + 0x2000) & 0xc000, probe);

    /* Both guards branch to one shared exit placed after the body.  Writing
     * `return 1` twice would put an inline copy near the top instead. */
    if (Object_CheckMovementCollision(actor, probe) == 1) {
        goto refuse;
    }
    if (Object_CheckMovementCollision(actor, target) != 0) {
        goto refuse;
    }

    Engine_EventBegin();
    Object_SetMode(actor, 6);
    WaitFrames(6);
    Audio_PlayCue(152);
    Object_SetMode(actor, 7);

    actor->speedX = 0x00030000;
    actor->speedY = 0x00020000;
    actor->speedZ = 0x00040000;
    actor->flags &= (u8)0x7e;   /* masks the byte re-read here, not `saved` */

    Engine_ActorSetSpriteFlags(actor, 0);
#if defined(TBS_EDITION_JA)
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, target->x.part.pixel, target->z.part.pixel);
#else
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, ((target->x.fixed >> 20) << 4) + 8, ((target->z.fixed >> 20) << 4) + 8);
#endif
    Object_SetMode(actor, 6);
    Engine_ActorSetSpriteFlags(actor, 1);
    WaitFrames(6);

    actor->flags = saved;
    Engine_EventEnd();
    return 0;

refuse:
    return 1;
}

void SceneActor_PassRaisedPointOfActorZero(void)
{
    s32 pos[3];
    struct Actor_02000dc8 *p = Actor_Get(ACTOR_PARTY_LEADER);

    pos[0] = p->f08;
    pos[1] = p->f0c;
    pos[2] = p->f10 + 0x200000;
    SceneActor_MoveActorZeroToTarget(pos);
}

void SceneActor_PassActorZeroOffsetPoint(void)
{
    s32 pos[3];
    struct Actor_02000dc8 *actor = Actor_Get(ACTOR_PARTY_LEADER);

    pos[0] = actor->f08;
    pos[1] = actor->f0c;
    pos[2] = actor->f10 + 0xFFE00000;
    SceneActor_MoveActorZeroToTarget(pos);
}

/* Babi's tunnel: publish the player actor at +24 of the event work once it
 * has passed the row limit of the current area and story step. */
void BabiChika_UpdateTrackedActor(void)
{
    u8 *actor;
    u8 *work;
    s32 limit;

    actor = (u8 *)Object_GetById(0);
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
    if ((*(s32 *)(actor + 16) >> 19) <= limit) {
        *(s32 *)(work + 24) = 0;
    } else {
        *(s32 *)(work + 24) = (s32)actor;
    }
}

/* Depth flags and slot ranks. */
s32 SceneActor_SetFlagBitByRelativeDepth(struct Actor_02000ec8 *actor)
{
    struct Actor_02000ec8 *ref;
    u8 *fp;
    u8 flag;
    ref = Actor_Get(ACTOR_PARTY_LEADER);
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
    struct Slot02000f10 *a = Actor_Get(first);
    struct Slot02000f10 *b = Actor_Get(second);

    if (a->rank <= b->rank) {
        s32 t;

        t = a->x;    a->x    = b->x;    b->x    = t;
        t = a->y;    a->y    = b->y;    b->y    = t;
        t = a->rank; a->rank = b->rank; b->rank = t;
        WaitFrames(1);
    }
}

/* Switch the object between animations 1 and 2 every two frames, and every fourth frame spawn a small palette-5 effect 16 pixels up its y axis, within three pixels of it in x and z. */
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

/* Waits and effect motion. */
s32 SceneActor_CopyActor8PositionWhenAtRow10(Record *record)
{
    Record *ref = Actor_Get(ACTOR_PARTY_LEADER);

    if (ref->w12 > (s32)0xffd00000
        && (((Record* (*)())Object_GetById)(8)->w16 >> 20) == 10) {
        record->w8 = ((Record* (*)())Object_GetById)(8)->w8;
        record->w12 = (s32)0xffe00000;
        record->w16 = ((Record* (*)())Object_GetById)(8)->w16;
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

            WaitFrames(1);
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

/*
 * The decay of the Z velocity stays a signed divide by sixteen: that shape is
 * what reproduces the negative bias and arithmetic shift in the reference.
 */
void Effect_AdvanceMotion(struct MotionEffect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* This block orders the Z load after the Y store; do not flatten it. */
    /* FAKEMATCH: removing this one-pass block changes instruction scheduling; see its retained draft. */
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
