#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"
#include "RAMAKAN.H"

void Effect_Spawn();

struct DustParams {
    u8 unknown_00[8];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_10[18];
    u16 angle;
    u8 unknown_24[4];
};

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

void Object_SetActionById();
s32 RamakanSabaku_EmitSandEffect(u8 *actor);
void BattleFx_SetWeightedResult();

void RamakanSabaku_UpdateTravelDust(void);
s32 Object_CheckMovementCollision(struct FieldActor *object, s32 *pos);

void RamakanSabaku_UpdateTravelDust(void)
{
    struct FieldActor *actor;
    struct EventWork *event;
    struct DustParams params;
    s32 dx;
    s32 z;
    s32 phase;
    union GameStateRows *rows = (union GameStateRows *)&gGameState;

    actor = Object_GetById(rows->words[125]);
    event = gEventWork;
    if (actor->target_x == (s32)0x80000000) {
        return;
    }
    rows->halves[281][0]++;
    if (event->touched_trigger == 30) {
        return;
    }
    if (actor->speed <= 0x10000) {
        if ((gFrameCount & 15) != 0) {
            return;
        }
        params.scale_x = 0x8000;
        params.scale_y = 0x8000;
        dx = 0;
        params.angle = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        if (actor->facing != 0 && actor->facing != 0x8000) {
            z = actor->z.fixed;
            dx = 0x20000 - ((((z >> 20) & 1) * 5) << 16);
        } else {
            z = actor->z.fixed;
        }
        Effect_Spawn(actor->x.fixed + dx, actor->y.fixed, z, 0, 0, 0, 0x880001, &params);
    } else {
        phase = *(volatile u32 *)&gFrameCount & 7;
        if (phase != 0) {
            return;
        }
        /* FAKEMATCH: the reference reads the frame counter again and discards it */
        *(volatile u32 *)&gFrameCount;
        params.scale_x = 0xcccc;
        params.scale_y = 0xcccc;
        params.angle = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        Effect_Spawn(actor->x.fixed, actor->y.fixed + 0x20000, actor->z.fixed, 0,
                     ((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999, phase, 0x880001, &params);
    }
}

/* Lamakan Desert: face the nearest of actors 9 to 12, then walk the leader
 * towards it with the search emote. */
void RamakanSabaku_FaceNearestActor(void)
{
    u8 *target;
    u8 *actor;
    u8 *record;
    s32 id;
    s32 best;
    s32 min;
    s32 dx;
    s32 dz;

    target = Actor_Get(gGameState.selected_actor);
    best = 9;
    GameFlag_Set(0x200);
    min = 0x100000;
    for (id = 9; id <= 12; id++) {
        u8 *other = Actor_Get(id);

        if (other != 0) {
            dx = (*(s32 *)(target + 8) - *(s32 *)(other + 8)) / 0x10000;
            dz = (*(s32 *)(target + 16) - *(s32 *)(other + 16)) / 0x10000;
            {
                s32 ax = dx;

                if (ax < 0) {
                    ax = -ax;
                }
                if (dz < 0) {
                    dz = -dz;
                }
                if (ax + dz < min) {
                    best = id;
                    min = ax + dz;
                }
            }
        }
    }
    Engine_ActorSetAnimation(0, 1);
    *((u8 *)Object_GetById(0) + 90) &= 254;
    Engine_ActorFaceActor(0, best, 0);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(0, 0x102);
    Engine_ActorStartRepeatedMotion(0, 2);
    Engine_EventWait(60);
    Actor_SetAttachedEffect(0, 0x101);
    actor = Actor_Get(0);
    record = Object_GetById(0);
    *(u16 *)(actor + 6) = (*(u16 *)(record + 6) + 0x8000) & -0x1000;
    Engine_ActorSetAnimation(0, 5);
    Object_SetActionById(0, 24);
    Actor_SetSpeed(0, 0x1999, 0xccc);
    record = Object_GetById(0);
    *(s32 *)(record + 108) = (s32)RamakanSabaku_EmitSandEffect;
    record = Actor_Get(best);
    if (record != 0) {
        Engine_ActorSetDestination(0, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_EventWait(60);
    Actor_ShowEmote(best, 0x104, 0);
    Engine_EventWait(60);
    Actor_SetAttachedEffect(0, 0x100);
    *((u8 *)Object_GetById(0) + 90) |= 1;
    record = Object_GetById(0);
    *(s32 *)(record + 108) = 0;
    BattleFx_SetWeightedResult(53, 4);
}

void RamakanSabaku_UpdateDustAndProbe(void)
{
    struct FieldActor *center;
    s32 id;
    struct FieldActor *actor;
    s32 pos[3];
    struct DustParams params;
    s32 hit;
    s32 phase;
    s32 x;

    RamakanSabaku_UpdateTravelDust();
    {
        struct EventWork *event = gEventWork;

        id = gGameState.selected_actor;
        center = event->view_center;
    }
    actor = Object_GetById(id);
    pos[0] = actor->x.fixed;
    pos[1] = actor->y.fixed;
    pos[2] = actor->z.fixed + 0x18000;
    hit = Object_CheckMovementCollision(actor, pos);
    phase = gFrameCount & 4;
    if (phase == 0) {
        params.angle = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        x = actor->x.fixed + (((u32)(Engine_RandomNext() * 12) >> 16) << 16) - 0x60000;
        Effect_Spawn(x, actor->y.fixed, actor->z.fixed, 0, phase,
                     ((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999 + 0x7ffd, 0x800000, &params);
    }
    if (hit < 0) {
        Call2((void (*)())Engine_ActorSetAttachedEffect, id, 0x102);
        Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed, actor->z.fixed + 0x80000);
        Object_SetMode(actor, 7);
        Engine_ObjectCommitPosition(actor);
        do {
            Engine_TaskWait(1);
        } while (actor->y.fixed != *(s32 *)((u8 *)actor + 20));
        Object_SetMode(actor, 6);
        Engine_TaskWait(3);
        return;
    }
    pos[0] = actor->x.fixed;
    pos[1] = actor->y.fixed;
    pos[2] = actor->z.fixed + 0x80000;
    hit = Object_CheckMovementCollision(actor, pos);
    if (hit > 0) {
        return;
    }
    pos[0] = actor->x.fixed + 0x5b333;
    pos[1] = actor->y.fixed;
    pos[2] = actor->z.fixed + 0x5b333;
    hit = Object_CheckMovementCollision(actor, pos);
    if (hit > 0) {
        return;
    }
    pos[0] = actor->x.fixed - 0x5b333;
    pos[1] = actor->y.fixed;
    pos[2] = actor->z.fixed + 0x5b333;
    hit = Object_CheckMovementCollision(actor, pos);
    if (hit > 0) {
        return;
    }
    center->z.fixed += 0x18000;
    actor->z.fixed += 0x18000;
}

void FieldScene_RunScene3a5_020014b0(void)
{
    s32 rec8;
    s32 record;
    s32 rect[3];
    s32 shown;
    u16 *shown_addr;
    u8 *p5;

    p5 = gWork;
    RamakanSabaku_UpdateTravelDust();
    if (GameFlag_IsSet(0x90a) == 0) {
        rec8 = GameFlag_IsSet(0x200);
        if (rec8 == 0) {
            GameFlag_Set(0x200);
            SceneState_SetHalfwordB030(1);
            shown_addr = (u16 *)(p5 + 0xcba);
            shown = 0x258;
            *shown_addr = shown;
            record = Object_GetById(ACTOR_PARTY_LEADER);
            *(s32 *)(record + 36) = rec8;
            record = Object_GetById(ACTOR_PARTY_LEADER);
            *(s32 *)(record + 44) = rec8;
            record = Actor_Get(ACTOR_PARTY_LEADER);
            *(s32 *)(record + 56) = -0x80000000;
            record = Actor_Get(ACTOR_PARTY_LEADER);
            *(s32 *)(record + 64) = -0x80000000;
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
            Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
            Event_Wait(40);
            Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
            Event_Wait(40);
            *((u8 *)Object_GetById(0) + 90) &= 254;
            rect[0] = rec8;
            rect[1] = rec8;
            rect[2] = rec8;
            record = Actor_Get(ACTOR_PARTY_LEADER);
            Call3(Vector_AddPolarOffset, -0x100000, *(u16 *)(record + 6), (s32)rect);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
            Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, rect[0] / 0x10000, rect[2] / 0x10000);
            Actor_WaitForMove(ACTOR_PARTY_LEADER);
            Event_Wait(2);
            *((u8 *)Object_GetById(0) + 90) |= 1;
            Event_Wait(30);
            Audio_PlayCue(148);
            Actor_RunRepeatedMotion(8, 2);
            Event_Wait(20);
            Actor_SetSpeed(8, 0x28000, 0x14000);
            Actor_WalkToAndWait(8, 168, 104);
            Actor_SetSpeed(8, 0x8000, 0x4000);
            Actor_WalkToAndWait(8, 168, 92);
            *shown_addr = shown;
            SceneState_SetHalfwordB030(0);
        }
    }
}
