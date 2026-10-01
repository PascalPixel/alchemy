/* NONMATCHING: 552/552 bytes, score 145 (17 register-only halfwords) as
 * RamakanSabaku_SweepBack (2026-09-29), meant for
 * FIELD/RAMAKAN_SABAKU/SWEPT_BACK.C. The safe-point tables now carry labels
 * in the listing (RamakanSabaku_SafePoints1/2/Other); names replace the
 * address constants of the earlier draft, whose hypothesis log is in the
 * history before this commit. Remaining: countdown r7 and byte offset r6
 * are swapped, and the hold/timer scratch registers r2/r3 swap. A 300 s
 * permute from this text found nothing; a for-loop over i is far worse;
 * local declaration order has no effect. */
#include "RAMAKAN.H"

/* Preserve the original measured FIELD_EVENT adapter context of this draft. */
static inline void Event_Wait(s32 frames)
{
    Engine_EventWait(frames);
}

#include "FIELD_EFFECT.H"

/* Where the sand leaves the party in each area, as x and z pairs. */
extern const s32 RamakanSabaku_SafePoints1[];
extern const s32 RamakanSabaku_SafePoints2[];
extern const s32 RamakanSabaku_SafePointsOther[];

s32 RamakanSabaku_CalculatePlanarDistance(s32 *position_a, const s32 *position_b);
void OverlayObject_WaitUntilField12BelowLimit(struct FieldActor *object, s32 limit);

struct SafePoint {
    s32 x;
    s32 z;
};

/* The sand has swallowed the leader: set it down at the nearest safe point
   of this area, with a puff of sand as it lands, and hold the crossing's
   progress down while the party recovers. */
void RamakanSabaku_SweepBack(void)
{
    u8 *work;
    s32 frames;
    s32 best;
    s32 count;
    const s32 *points;
    s32 left;
    s32 offset;
    const s32 *point;
    s32 nearest;
    s32 distance;
    struct FieldActor *leader;
    struct EffectOptions options;
    struct EffectOptions puff;
    s16 *progress;
    u16 *hold;
    s32 zero;
    s32 hold_frames = 600;

    work = gWork;
    frames = 60;
    best = 0xf00000;
    GameFlag_Set(0x200);
    SceneState_SetHalfwordB030(1);
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku1) {
        count = 3;
        points = RamakanSabaku_SafePoints1;
    } else if (gGameState.scene == (s32)&SceneId_RamakanSabaku2) {
        count = 5;
        points = RamakanSabaku_SafePoints2;
    } else {
        count = 2;
        points = RamakanSabaku_SafePointsOther;
    }
    left = count;
    point = points;
    if (count != 0) {
        offset = 0;
        do {
            distance = RamakanSabaku_CalculatePlanarDistance(&Actor_Get(ACTOR_PARTY_LEADER)->x.fixed, point);
            if (distance <= best) {
                best = distance;
                nearest = left - count;
            }
            offset += 8;
            point = (const s32 *)((const u8 *)points + offset);
        } while (--count != 0);
    }
    nearest <<= 1;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    {
        struct FieldActor *actor = Actor_Get(ACTOR_PARTY_LEADER);
        const struct SafePoint *spot = (const struct SafePoint *)&points[nearest];

        Engine_ObjectSetPosition(actor, spot->x, 0, spot->z);
    }
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_y = 0x60000;
    Audio_PlayCue(152);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    OverlayObject_WaitUntilField12BelowLimit(leader, Actor_Get(ACTOR_PARTY_LEADER)->y.fixed);
    Audio_PlayCue(241);
    {
        struct FieldActor *actor = Actor_Get(ACTOR_PARTY_LEADER);

        options.type = 214;
        options.start_scale_x = 0x8000;
        options.start_scale_y = 0xcccc;
        options.target_scale_x = 0x10000;
        options.target_scale_y = 0x13333;
        Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, 0, 0,
                     EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET, &options);
    }
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x104, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 18);
    hold = (u16 *)(work + 0xcba);
    progress = (s16 *)((u8 *)&gGameState + 0x232);
    zero = 0;
    do {
        *hold = hold_frames;
        frames--;
        if (*progress != 0) {
            *progress -= 5;
            if (*progress <= 0) {
                *progress = zero;
            } else if (frames == 0) {
                frames = 1;
            }
        }
        Engine_TaskWait(1);
    } while (frames != 0);
    {
        struct FieldActor *actor = Actor_Get(ACTOR_PARTY_LEADER);
        s32 scale;

        puff.type = 214;
        /* FAKEMATCH: share one x-scale local across the start-y store. */
        scale = 0x8000;
        puff.start_scale_y = 0xcccc;
        puff.start_scale_x = scale;
        puff.target_scale_x = scale;
        puff.target_scale_y = 0x13333;
        Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, frames, frames,
                     EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET, &puff);
    }
    Audio_PlayCue(0x120);
    Audio_PlayCue(152);
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_y = 0x60000;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(10);
    *(u16 *)(work + 0xcba) = frames;
    SceneState_SetHalfwordB030(0);
}
