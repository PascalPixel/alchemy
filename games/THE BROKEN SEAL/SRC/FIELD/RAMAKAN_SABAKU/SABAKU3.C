#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"
#include "RAMAKAN.H"
#include "GAME_STATE.H"
#include "SCENE_IDS.H"

void Effect_Spawn();

struct DustParams {
    u8 unknown_00[8];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_10[18];
    u16 angle;
    u8 unknown_24[4];
};

void Object_SetActionById();
s32 RamakanSabaku_EmitSandEffect(u8 *actor);
void BattleFx_SetWeightedResult();
void RamakanSabaku_UpdateTravelDust(void);
s32 Object_CheckMovementCollision(struct FieldActor *object, s32 *pos);

extern struct EventWork *gEventWork;
void RamakanSabaku_ClaimSandEffectVram();
void Engine_MapCopyCells();
void BattleFx_SetQueuedSoundAndPlay();
void RamakanSabaku_RaiseQuarterTriggers(void);
void RamakanSabaku_ApplyEntryState();

void RamakanSabaku_UpdateTravelDust(void)
{
    struct FieldActor *actor;
    struct EventWork *event;
    struct DustParams params;
    s32 dx;
    s32 z;
    s32 phase;
    actor = Object_GetById(gGameState.selected_actor);
    event = gEventWork;
    if (actor->target_x == (s32)0x80000000) {
        return;
    }
    gGameState.unknown_232++;
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

    target = Object_GetById(gGameState.selected_actor);
    best = 9;
    Engine_GameFlagSet(0x200);
    min = 0x100000;
    for (id = 9; id <= 12; id++) {
        u8 *other = Object_GetById(id);

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
    Engine_ActorSetAttachedEffect(0, 0x102);
    Engine_ActorStartRepeatedMotion(0, 2);
    Engine_EventWait(60);
    Engine_ActorSetAttachedEffect(0, 0x101);
    actor = Object_GetById(0);
    record = Object_GetById(0);
    *(u16 *)(actor + 6) = (*(u16 *)(record + 6) + 0x8000) & -0x1000;
    Engine_ActorSetAnimation(0, 5);
    Object_SetActionById(0, 24);
    Engine_ActorSetSpeed(0, 0x1999, 0xccc);
    record = Object_GetById(0);
    *(s32 *)(record + 108) = (s32)RamakanSabaku_EmitSandEffect;
    record = Object_GetById(best);
    if (record != 0) {
        Engine_ActorSetDestination(0, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_EventWait(60);
    Engine_ActorShowEmote(best, 0x104, 0);
    Engine_EventWait(60);
    Engine_ActorSetAttachedEffect(0, 0x100);
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
    if (Engine_GameFlagIsSet(0x90a) == 0) {
        rec8 = Engine_GameFlagIsSet(0x200);
        if (rec8 == 0) {
            Engine_GameFlagSet(0x200);
            SceneState_SetHalfwordB030(1);
            shown_addr = (u16 *)(p5 + 0xcba);
            shown = 0x258;
            *shown_addr = shown;
            record = Object_GetById(ACTOR_PARTY_LEADER);
            *(s32 *)(record + 36) = rec8;
            record = Object_GetById(ACTOR_PARTY_LEADER);
            *(s32 *)(record + 44) = rec8;
            record = Object_GetById(ACTOR_PARTY_LEADER);
            *(s32 *)(record + 56) = -0x80000000;
            record = Object_GetById(ACTOR_PARTY_LEADER);
            *(s32 *)(record + 64) = -0x80000000;
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
            Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 8, 0);
            Engine_EventWait(40);
            Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
            Engine_EventWait(40);
            *((u8 *)Object_GetById(0) + 90) &= 254;
            rect[0] = rec8;
            rect[1] = rec8;
            rect[2] = rec8;
            record = Object_GetById(ACTOR_PARTY_LEADER);
            Call3(Vector_AddPolarOffset, -0x100000, *(u16 *)(record + 6), (s32)rect);
            Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
            Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, rect[0] / 0x10000, rect[2] / 0x10000);
            Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
            Engine_EventWait(2);
            *((u8 *)Object_GetById(0) + 90) |= 1;
            Engine_EventWait(30);
            Engine_AudioPlayCue(148);
            Engine_ActorRunRepeatedMotion(8, 2);
            Engine_EventWait(20);
            Engine_ActorSetSpeed(8, 0x28000, 0x14000);
            Engine_ActorWalkToAndWait(8, 168, 104);
            Engine_ActorSetSpeed(8, 0x8000, 0x4000);
            Engine_ActorWalkToAndWait(8, 168, 92);
            *shown_addr = shown;
            SceneState_SetHalfwordB030(0);
        }
    }
}

/* Lamakan Desert: reset the area timers and, outside the town variant, open
 * the map cells of the stored area variant before applying the entry state. */
s32 RamakanSabaku_EnterArea(s32 a0, s32 a1)
{
    s32 v5;

    gGameState.unknown_22c = 0x258;
    gGameState.unknown_22e = 0;
    gGameState.unknown_230 = 0x119;
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku4) {
    } else {
        *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x100;
        RamakanSabaku_ClaimSandEffectVram();
        Engine_TaskAddCallback((s32)RamakanSabaku_RaiseQuarterTriggers, 0xc80);
        if (gGameState.scene == (s32)&SceneId_RamakanSabaku1) {
            Call6(Engine_MapCopyCells, 22, 7, 4, 2, 64, 126);
            Engine_MapCopyCells(8, 10, 4, 2, 68, 126);
            Engine_MapCopyCells(23, 21, 4, 2, 72, 126);
            Engine_MapCopyCells(16, 42, 4, 2, 76, 126);
            Engine_MapCopyCells(36, 44, 4, 2, 80, 126);
            Engine_MapCopyCells(14, 55, 4, 2, 84, 126);
        } else {
            if (gGameState.scene != (s32)&SceneId_RamakanSabaku2) {
                goto third_area;
            }
            Call6(Engine_MapCopyCells, 42, 5, 4, 2, 64, 126);
            Engine_MapCopyCells(20, 11, 4, 2, 68, 126);
            Engine_MapCopyCells(14, 12, 4, 2, 72, 126);
            Engine_MapCopyCells(56, 18, 4, 2, 76, 126);
            Engine_MapCopyCells(7, 22, 4, 2, 80, 126);
            Engine_MapCopyCells(44, 23, 4, 2, 84, 126);
            Engine_MapCopyCells(38, 24, 4, 2, 88, 126);
            Engine_MapCopyCells(26, 28, 4, 2, 92, 126);
            Engine_MapCopyCells(17, 35, 4, 2, 96, 126);
            Engine_MapCopyCells(50, 36, 4, 2, 100, 126);
            Engine_MapCopyCells(34, 43, 4, 2, 104, 126);
            Engine_MapCopyCells(6, 46, 4, 2, 108, 126);
            Engine_MapCopyCells(27, 55, 4, 2, 112, 126);
            Engine_MapCopyCells(43, 56, 4, 2, 116, 126);
        }
        goto apply_entry_state;
        third_area:;
        if (gGameState.scene == (s32)&SceneId_RamakanSabaku3) {
            v5 = 124;
            BattleFx_SetQueuedSoundAndPlay(169);
            Engine_MapCopyCells(8, 14, 4, 4, 64, v5);
            Engine_MapCopyCells(6, 18, 4, 4, 68, v5);
            Engine_MapCopyCells(10, 21, 4, 4, 72, v5);
        }
        apply_entry_state:;
        RamakanSabaku_ApplyEntryState();
    }
    return 0;
}

void FieldScene_RunScene3a5_02001874(void)
{
    u32 i;
    s32 record;

    Engine_ActorSetSpeed(8, 0x8000, 0x4000);
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorWalkToAndWait(8, 168, 96);
    Engine_ActorSetAnimation(8, 2);
}
