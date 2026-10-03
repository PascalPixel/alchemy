#include "YAMA.H"
#include "TYPES.H"
#include "ARUTIN.H"
#include "CALL.H"

void SceneMotion_UpdateTimedActor(struct SceneMotion *work);

extern u8 MsgArutinWatchFallingRocks[];

extern const struct SceneEvent gArutinYamaEventsOther[];
extern const struct SceneEvent gArutinYamaEvents1[];
extern const struct SceneEvent gArutinYamaEvents2[];
extern const struct SceneEvent gArutinYamaEvents3[];
extern const struct SceneEvent gArutinYamaEvents4[];
extern const struct SceneEvent gArutinYamaEvents5[];
extern const struct SceneEvent gArutinYamaEvents6[];
extern const struct SceneEvent gArutinYamaEvents7[];
extern const struct SceneEvent gArutinYamaEvents8[];
extern const struct SceneEvent gArutinYamaEvents9[];
extern const struct SceneEvent gArutinYamaEvents10[];
extern const struct SceneEvent gArutinYamaEvents11[];

extern void Engine_WorkSetValuesIfNonNegative();
extern void Engine_AudioPlayCue(s32);

struct ActorMotion {
    u8 pad0[100];
    u16 step[2];
    s32 phase;
    s32 callback;
};

void Engine_EventBegin();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_AudioPlayCue();
void Engine_ActorRunRepeatedMotion();
void Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_ActorMoveToAndWait();
void Engine_WorkSetValuesIfNonNegative();
void Engine_ActorFaceDirection();
void FieldScene_RunActorTenFourStepSequence();
void Engine_MapCopyCellAttributes();
void Engine_ActorSetSpriteFlags();
void Engine_ObjectSetTargetAndCallback();
void Engine_EventEnd();

struct Half { u16 v; };

extern u8 ArutinYama_ActorScript;

void FieldScene_RunSharedSetPiece(s32 a0)
{
    struct FieldActor *actor0;
    struct FieldActor *actor8;
    struct FieldActor *actor9;
    struct FieldActor *actor10;
    struct FieldActor *actor;

    actor0 = Object_GetById(ACTOR_PARTY_LEADER);
    actor8 = Object_GetById(8);
    actor9 = Object_GetById(9);
    actor10 = Object_GetById(10);
    Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_EventWait(40);
    Engine_CameraSetSpeed(0x10000, 0x2000);
    Engine_CameraMoveTo(0x3100000, -1, 0x740000, 1);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
    Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, 0x318, 140);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xc000, 100);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
    Engine_AudioPlayCue(183);
    Engine_WorkSetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Engine_EventWait(20);
    actor9->scale_x = 0x13333;
    actor9->scale_y = 0x13333;
    actor9->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    actor9->update = (void (*)(union FieldObject *))SceneActor_CopyActor8PositionWithFixedY;
    Engine_ActorSetAnimation(8, 4);
    *(s32 *)((u8 *)actor8 + 68) = 0x8000;
    actor8->x.fixed = 0x3120000;
    actor8->y.fixed = 0x200000;
    actor8->z.fixed = 0x5a0000;
    actor8->scale_x = 0x20000;
    actor8->scale_y = 0x20000;
    Engine_EventWait(10);
    Engine_AudioPlayCue(183);
    Engine_WorkSetValuesIfNonNegative(0x40000, 0x20000, 0x10000);
    Engine_EventWait(20);
    actor10->x.fixed += 0xe0000;
    actor10->y.fixed += -0x80000;
    actor10->sprite->rotation = 0xc000;
    Engine_AudioPlayCue(107);
    Engine_WorkSetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x102, 80);
    Engine_AudioPlayCue(55);
    Engine_WorkSetValuesIfNonNegative(0x10000, 0x30000, 0x10000);
    Engine_ActorSetSpritePriority(8, 0);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 0);
    Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
    *(u16 *)((u8 *)actor0 + 100) = 0;
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, ArutinYama_LeaderRideScript);
    if (Engine_GameFlagIsSet(0x205) != 0) {
        Engine_ActorSetPosition(ACTOR_GERALD, 0x36e0000, 0x2100000);
        actor = Object_GetById(ACTOR_GERALD);
        actor->facing = 0x5000;
    }
    Engine_CameraSetSpeed(0x14000, 0x2800);
    Engine_CameraMoveTo(0x3120000, -1, 0x22c0000, 1);
    Engine_EventWait(a0);
    Engine_ActorSetSpritePriority(8, 1);
    Engine_ActorSetSpeed(8, 0x195c2, 0xcae1);
    *(u16 *)((u8 *)actor8 + 100) = 0;
    Engine_ActorEnableActionCallback(8, ArutinYama_LogRideScript);
    do {
        Engine_TaskWait(1);
    } while (*(s16 *)((u8 *)actor0 + 100) == 0);
    Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0);
    do {
        Engine_TaskWait(1);
    } while (*(s16 *)((u8 *)actor8 + 100) == 0);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Object_GetById(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Engine_AudioPlayCue(0x121);
    Engine_WorkSetValuesIfNonNegative(-1, -1, 0xe666);
    actor9->x.fixed = 0x3120000;
    actor9->update = NULL;
    actor9->z.fixed = 0x26a0000;
    actor9->y.fixed = -0x400000;
    Engine_ActorSetSpeed(8, 0x19999, 0xcccc);
    *(s32 *)((u8 *)actor8 + 68) = 0x1999;
    *(s32 *)((u8 *)actor8 + 72) = 0x3333;
    actor8->velocity_y = 0x40000;
    Engine_ActorMoveToAndWait(8, 0x312, 0x25c);
    Engine_ActorSetSpeed(8, 0x33333, 0x19999);
    Engine_ActorSetDestination(8, 0x312, 0x284);
    Engine_EventWait(15);
    Engine_WorkSetValuesIfNonNegative(0x50000, 0x70000, 0x10000);
    Engine_MapCopyCellsTo(25, 36, 43, 36, 11, 9);
    Engine_MapCopyCellAttributes(25, 35, 10, 5, 43, 35);
    Engine_ActorSetPosition(8, 0, 0);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_TaskAddCallback(FieldScene_RunScene3a4SequenceG, 0xc80);
    Engine_EventWait(80);
    Engine_TaskRemoveCallback(FieldScene_RunScene3a4SequenceG);
    Engine_EventWait(60);
    Engine_AudioPlayCue(17);
    Engine_WorkSetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(120);
    if (Engine_GameFlagIsSet(0x205) != 0) {
        Engine_ActorSetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
        Engine_ActorWalkTo(ACTOR_GERALD, 0x338, 0x22e);
    }
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 0x356, 0x248);
    if (Engine_GameFlagIsSet(0x205) != 0) {
        Engine_ActorSetAnimation(ACTOR_GERALD, 1);
        Engine_ActorFaceDirection(ACTOR_GERALD, 0x4000, 0);
    }
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x6000, 40);
    Engine_ActorShowEmote(ACTOR_GERALD, 0x102, 0);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Engine_CameraMoveTo(0x3140000, -0x400000, 0x2620000, 1);
    Engine_CameraWaitForMove();
    Engine_AudioPlayCue(148);
    Engine_EventWait(240);
    if (Engine_GameFlagIsSet(0x205) != 0) {
        Engine_CameraSetSpeed(0x40000, 0x8000);
        Engine_CameraMoveTo(0x3560000, 0, 0x2480000, 1);
        Engine_CameraWaitForMove();
        Engine_ActorWalkToAndWait(ACTOR_GERALD, 0x348, 0x228);
        Engine_ActorWalkToAndWait(ACTOR_GERALD, 0x356, 0x232);
        Engine_ActorSetAnimation(ACTOR_GERALD, 2);
        actor = Object_GetById(ACTOR_PARTY_LEADER);
        if (actor != NULL) {
            Engine_ActorSetDestination(ACTOR_GERALD, actor->x.part.pixel, actor->z.part.pixel);
        }
        Engine_ActorWaitForMove(ACTOR_GERALD);
        Engine_ActorSetPosition(ACTOR_GERALD, 0, 0);
    }
    Audio_PlayCueFromEventWork();
    Engine_GameFlagSet(0x908);
}

/* The falling-rocks sign on the mountain path. Unless flag 0x908 or 0xf14
 * is set, it sets flag 0x205 and plays Gerald's scene at the wall, then the
 * shared set piece. */
void FieldScene_RunFallingRocksWarning(void)
{
    u8 *record;
    s32 msg;

    Engine_EventBegin();
    msg = (s32)MsgArutinWatchFallingRocks;
    Engine_MessageShowCentered(msg, 1);
    if (Engine_GameFlagIsSet(0x908) == 0 && Engine_GameFlagIsSet(0xf14) == 0) {
        Engine_GameFlagSet(0x205);
        Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
        Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 0x316, 140);
        Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 0x30c, 140);
        Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if (record != 0) {
            Engine_ActorSetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Engine_ActorSetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
        Engine_ActorWalkToAndWait(ACTOR_GERALD, 0x320, 140);
        Engine_ActorFaceDirection(ACTOR_GERALD, 0xc000, 20);
        Engine_EventSetMessage(msg + 1);
        Engine_ActorSetAnimation(ACTOR_GERALD, 4);
        Engine_EventWait(20);
        Engine_EventShowMessageAndWait(ACTOR_GERALD, 0, 10);
        Engine_ActorJump(ACTOR_GERALD, 6, 0);
        Engine_ActorSetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
        ((u8 *)Object_GetById(ACTOR_GERALD))[90] &= 254;
        Engine_ActorWalkToAndWait(ACTOR_GERALD, 0x318, 110);
        Engine_EventWait(1);
        ((u8 *)Object_GetById(ACTOR_GERALD))[90] |= 1;
        Engine_AudioPlayCue(161);
        Engine_WorkSetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
        ((u8 *)Object_GetById(ACTOR_GERALD))[90] &= 254;
        Engine_ActorWalkToAndWait(ACTOR_GERALD, 0x318, 120);
        Engine_EventWait(1);
        {
            u8 *record = (u8 *)Object_GetById(ACTOR_GERALD);
            /* FAKEMATCH: a result temporary, not a compound or-assign: the
             * reference merges the byte into the mask's register, which the
             * two-address ORR does only when the result is its own object.
             * 2026-10-02: both record[90] |= 1 and the plain assignment
             * record[90] = record[90] | 1 put the result in r3, not r6. */
            u8 merged = (u8)(record[90] | 1);

            record[90] = merged;
        }
        Engine_WorkSetValuesIfNonNegative(-1, -1, 0xe666);
        Engine_EventWait(80);
        Engine_AudioPlayCue(141);
        Engine_WorkSetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        Engine_EventWait(40);
        Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
        Engine_ActorShowEmote(ACTOR_GERALD, 0x101, 60);
        Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
        Engine_ActorFaceDirection(ACTOR_GERALD, 0, 20);
        Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        Engine_ActorFaceDirection(ACTOR_GERALD, 0x8000, 40);
        Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
        Engine_ActorFaceDirection(ACTOR_GERALD, 0, 40);
        Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Engine_ActorFaceDirection(ACTOR_GERALD, 0xc000, 40);
        Engine_ActorShowEmote(ACTOR_GERALD, 0x102, 60);
        Engine_ActorFaceDirection(ACTOR_GERALD, 0x4000, 20);
        Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
        Engine_EventShowMessageAndWait(ACTOR_GERALD, 0, 10);
        Engine_ActorSetSpeed(ACTOR_GERALD, 0x28000, 0x14000);
        Engine_ActorSetAnimation(ACTOR_GERALD, 5);
        Engine_ActorMoveToAndWait(ACTOR_GERALD, 0x31c, 138);
        Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        Engine_ActorMoveToAndWait(ACTOR_GERALD, 0x324, 140);
        Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
        Engine_ActorMoveToAndWait(ACTOR_GERALD, 0x324, 166);
        Engine_ActorMoveToAndWait(ACTOR_GERALD, 0x2fc, 166);
        Engine_ActorMoveToAndWait(ACTOR_GERALD, 0x2fc, 198);
        Engine_ActorMoveToAndWait(ACTOR_GERALD, 0x312, 198);
        Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
        Engine_ActorMoveToAndWait(ACTOR_GERALD, 0x312, 246);
        Engine_ActorSetAnimation(ACTOR_GERALD, 1);
        Engine_ActorSetPosition(ACTOR_GERALD, 0, 0);
        Engine_EventWait(40);
        FieldScene_RunSharedSetPiece(10);
    }
    Engine_EventEnd();
}

/* What each of Altin Peak's areas answers; the first area answers
   differently once flag 0x8fd is set. */
const struct SceneEvent *Scene_GetEvents(void)
{
    const struct SceneEvent *table;

    if (gGameState.scene == (s32)&SceneId_ArutinYama1) {
        if (Engine_GameFlagIsSet(0x8fd) != 0) {
            table = (const struct SceneEvent *)ArutinYama_OpenedAreaScript;
        } else {
            table = gArutinYamaEvents1;
        }
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama2) {
        table = gArutinYamaEvents2;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama3) {
        table = gArutinYamaEvents3;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama4) {
        table = gArutinYamaEvents4;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama5) {
        table = gArutinYamaEvents5;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama6) {
        table = gArutinYamaEvents6;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama7) {
        table = gArutinYamaEvents7;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama8) {
        table = gArutinYamaEvents8;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama9) {
        table = gArutinYamaEvents9;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama10) {
        table = gArutinYamaEvents10;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama11) {
        table = gArutinYamaEvents11;
    } else {
        table = gArutinYamaEventsOther;
    }
    return table;
}

/*
 * Arutin mountain: a timed actor that bobs toward the ground. While a delay
 * is pending it counts down; with no vertical velocity it sinks one step per
 * tick and settles on the ground (firing the landing cue on first contact);
 * when the timer lapses it reactivates and starts the fall.
 */
void SceneMotion_UpdateTimedActor(struct SceneMotion *work)
{
    if (work->delay != 0) {
        if (--work->delay == 1)
            Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    }
    if (work->velocity == 0) {
        Object_SetMode(work,1);
        work->y += -0x18000;
        if (work->y < work->ground) {
            if (work->active != 0) {
                Engine_AudioPlayCue(229);
                work->active = 0;
                work->delay = 4;
                Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
            }
            work->y = work->ground;
        }
        work->state = 1;
    } else {
        work->state = 0;
    }
    if (work->timer == 0) {
        Engine_AudioPlayCue(152);
        work->active = 1;
        Object_SetMode(work,2);
        work->velocity = 0x30000;
    }
    if (++work->timer == 60)
        work->timer = 0;
}

/* The zero is a one-halfword struct, so its movhi pool load reaches 64 bytes
 * and the pool lands where the ROM has it; the motion callbacks are Value_
 * link symbols. */
void FieldScene_BuildMultiPhasePresentation(void)
{
    struct Half p10;
    s32 p8;
    u8 *rec3;
    s32 rec7;
    s32 record;
    s32 v6;

    rec3 = (s32)Object_GetById(10);
    Engine_EventBegin();
    Engine_CameraSetSpeed(0x26666, 0x4ccc);
    Call4(Engine_CameraMoveTo, 0x12a0000, -1, 0x1510000, 1);
    Engine_CameraWaitForMove();
    Engine_AudioPlayCue(147);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(40);
    Engine_CameraSetSpeed(0xcccc, 0x1999);
    Engine_CameraMoveTo(0x1270000, 0x200000, 0xd40000, 1);
    ((struct ActorMotion *)rec3)->phase = 0;
    ((struct ActorMotion *)rec3)->step[0] = 0;
    p8 = (s32)rec3 + 100;
    p10.v = 0;
    ((struct ActorMotion *)rec3)->step[1] = 0;
    *(s32 *)((s32)rec3 + 72) = 0x6666;
    ((struct ActorMotion *)rec3)->callback = (s32)&(*(u8 *)SceneMotion_UpdateTimedActor);
    Call3(Engine_ActorSetSpeed, 10, 0x13333, 0x9999);
    Call3(Engine_ActorMoveToAndWait, 10, 0x134, 0x123);
    Engine_ActorMoveToAndWait(10, 0x137, 215);
    ((struct ActorMotion *)rec3)->callback = 0;
    rec3[91] = p10.v;
    Engine_EventWait(16);
    Engine_ActorSetAnimation(10, 1);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 10, 0xb000, 20);
    Call3(Engine_ActorFaceDirection, 10, 0x8000, 40);
    FieldScene_RunActorTenFourStepSequence();
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 10, 0x5000, 20);
    Call3(Engine_ActorFaceDirection, 10, 0x3000, 40);
    Call4(Engine_CameraMoveTo, 0x14e0000, -1, 0xf40000, 1);
    ((struct ActorMotion *)rec3)->phase = 0;
    ((struct ActorMotion *)rec3)->step[0] = 0;
    ((struct ActorMotion *)rec3)->step[1] = 0;
    ((struct ActorMotion *)rec3)->callback = (s32)&(*(u8 *)SceneMotion_UpdateTimedActor);
    Call3(Engine_ActorMoveToAndWait, 10, 0x140, 232);
    Call3(Engine_ActorMoveToAndWait, 10, 0x154, 0x106);
    Engine_ActorMoveToAndWait(10, 0x176, 0x106);
    ((struct ActorMotion *)rec3)->callback = 0;
    Engine_EventWait(16);
    Engine_ActorSetAnimation(10, 1);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 10, 0xf000, 20);
    Engine_ActorFaceDirection(10, 0xd000, 40);
    Engine_AudioPlayCue(153);
    record = (s32)Object_GetById(10);
    *(s32 *)(record + 40) = 0x40000;
    Engine_ActorSetAnimation(10, 2);
    Engine_ActorMoveToAndWait(10, 0x17c, 248);
    Engine_EventWait(10);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 10, 0xb000, 20);
    Call3(Engine_ActorFaceDirection, 10, 0x8000, 40);
    Call4(Engine_CameraMoveTo, 0x1300000, -1, 0xd70000, 1);
    ((struct ActorMotion *)rec3)->phase = 0;
    ((struct ActorMotion *)rec3)->step[0] = 0;
    ((struct ActorMotion *)rec3)->step[1] = 0;
    ((struct ActorMotion *)rec3)->callback = (s32)&(*(u8 *)SceneMotion_UpdateTimedActor);
    Call3(Engine_ActorMoveToAndWait, 10, 0x149, 219);
    ((struct ActorMotion *)rec3)->callback = 0;
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(16);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(10, 0x8000, 40);
    *(u8 *)((s32)Object_GetById(9) + 85) = p10.v;
    Engine_MapCopyCellAttributes(3, 0, 1, 1, 17, 13);
    Engine_MapCopyCellAttributes(3, 0, 1, 1, 18, 13);
    Engine_MapCopyCellAttributes(3, 0, 1, 1, 19, 13);
    Engine_ActorSetSpeed(10, 0x16666, 0xb333);
    record = (s32)Object_GetById(10);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_AudioPlayCue(153);
    v6 = 160;
    record = (s32)Object_GetById(10);
    *(s32 *)(record + 40) = (v6 << 11);
    Engine_ActorSetAnimation(10, 3);
    Call3(Engine_ActorMoveToAndWait, 10, 0x127, 215);
    Engine_ActorSetAnimation(10, 1);
    record = (s32)Object_GetById(10);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(40);
    Engine_AudioPlayCue(153);
    record = (s32)Object_GetById(10);
    *(s32 *)(record + 40) = (v6 << 11);
    Engine_ActorSetAnimation(10, 3);
    Call3(Engine_ActorMoveToAndWait, 10, 0x104, 215);
    Engine_ActorSetAnimation(10, 1);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 10, 0x5000, 20);
    Engine_ActorFaceDirection(10, 0x3000, 20);
    Engine_AudioPlayCue(147);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(40);
    Engine_MapCopyCellAttributes(4, 0, 1, 1, 17, 13);
    Engine_MapCopyCellAttributes(2, 0, 1, 1, 18, 13);
    Engine_MapCopyCellAttributes(4, 0, 1, 1, 19, 13);
    rec7 = (s32)Object_GetById(0);
    Engine_CameraSetSpeed(0x4cccc, 0x9999);
    Engine_CameraMoveTo(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), *(s32 *)(rec7 + 16), 1);
    Engine_CameraWaitForMove();
    Call3(Engine_ObjectSetTargetAndCallback, 10, 0x10000, (s32)&ArutinYama_ActorScript);
    Engine_GameFlagSet(0x904);
    Engine_EventEnd();
}

/* One object record, seen as the actor or as the leap it is making. */
union TimedActor {
    struct FieldActor actor;
    struct SceneMotion motion;
};

extern s32 gKeyState;

/* Actor 10 comes down the mountain in three leaps, landing with a shake
 * each time, then waits up to a second for a key before the camera
 * returns to the leader. */
void ArutinYama_RunLeapSequence(void)
{
    union TimedActor *actor;
    struct FieldActor *record;
    u32 n;

    actor = (union TimedActor *)Object_GetById(10);
    Engine_EventBegin();
    ObjectMotion_EnableActionAndResetMotion(10);
    Call2(Engine_CameraSetSpeed, 0x26666, 0x4ccc);
    Call4(Engine_CameraMoveTo, 0x1170000, 0x400000, 0xd80000, 1);
    Engine_CameraWaitForMove();
    Engine_AudioPlayCue(147);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 10, 0x3000, 20);
    Call3(Engine_ActorFaceDirection, 10, 0x5000, 20);
    Call3(Engine_ActorFaceDirection, 10, 0x8000, 40);
    Call2(Engine_CameraSetSpeed, 0xcccc, 0x1999);
    Call4(Engine_CameraMoveTo, 0x800000, 0x400000, 0xca0000, 1);
    actor->motion.active = 0;
    actor->motion.timer = 0;
    actor->motion.delay = 0;
    *(s32 *)&actor->actor.unknown_44[4] = 0x6666;
    actor->actor.update = (void (*)(union FieldObject *))SceneMotion_UpdateTimedActor;
    Call3(Engine_ActorSetSpeed, 10, 0x13333, 0x9999);
    Call3(Engine_ActorMoveToAndWait, 10, 212, 200);
    Call3(Engine_ActorMoveToAndWait, 10, 103, 200);
    actor->actor.update = NULL;
    actor->motion.state = 0;
    Engine_EventWait(10);
    Engine_ActorSetAnimation(10, 1);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 10, 0x5000, 40);
    Object_GetById(10)->unknown_5a &= 254;
    Call3(Engine_ActorSetSpeed, 10, 0x13333, 0x9999);
    record = Object_GetById(10);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_AudioPlayCue(153);
    record = Object_GetById(10);
    record->velocity_y = 0x40000;
    Call2((void (*)())Engine_ActorSetAnimation, 10, 3);
    Engine_ActorMoveToAndWait(10, 86, 214);
    Engine_ActorSetAnimation(10, 1);
    record = Object_GetById(10);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_EventWait(10);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0, 0x10000);
    Engine_EventWait(8);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(40);
    Object_GetById(10)->unknown_5a |= 1;
    Call3(Engine_ActorFaceDirection, 10, 0x3000, 20);
    Engine_ActorFaceDirection(10, 0, 40);
    actor->motion.active = 0;
    actor->motion.timer = 0;
    actor->motion.delay = 0;
    actor->actor.update = (void (*)(union FieldObject *))SceneMotion_UpdateTimedActor;
    Call3(Engine_ActorSetSpeed, 10, 0x13333, 0x9999);
    Call3(Engine_ActorMoveToAndWait, 10, 120, 215);
    actor->actor.update = NULL;
    actor->motion.state = 0;
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(16);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(10, 0, 10);
    Engine_AudioPlayCue(147);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(80);
    Engine_ActorSetAnimation(10, 3);
    Call4(SceneState_StoreParamsAndInstallTask, 0x820000, 0, 0xa80000, 0);
    Engine_EventWait(60);
    for (n = 0; n < 60 && gKeyState == 0; n++) {
        Engine_EventWait(1);
    }
    actor = (union TimedActor *)Object_GetById(0);
    Call2(Engine_CameraSetSpeed, 0x4cccc, 0x9999);
    Engine_CameraMoveTo(actor->actor.x.fixed, actor->actor.y.fixed, actor->actor.z.fixed, 1);
    Engine_CameraWaitForMove();
    Call1((void (*)())Engine_GameFlagSet, 0x905);
    Engine_EventEnd();
}
