#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "YAMA.H"
#include "TYPES.H"
#include "ARUTIN.H"
#include "CALL.H"
#include "FIELD_EVENT.H"
#include "FIELD_SERVICE.H"
#include "text/MSG_IDS.H"

#define ArutinYama_ActorScript (*(u8 *)&ArutinYama_ActorScript)



struct ActorMotion {
    u8 pad0[100];
    u16 step[2];
    s32 phase;
    s32 callback;
};

void FieldScene_RunActorTenFourStepSequence();
void Engine_MapCopyCellAttributes();
void Engine_ObjectSetTargetAndCallback();

struct Half { u16 v; };


void FieldScene_RunSharedSetPiece(s32 a0)
{
    struct FieldActor *actor0;
    struct FieldActor *actor8;
    struct FieldActor *actor9;
    struct FieldActor *actor10;
    struct FieldActor *actor;

    actor0 = Actor_Get(ACTOR_PARTY_LEADER);
    actor8 = Actor_Get(8);
    actor9 = Actor_Get(9);
    actor10 = Actor_Get(10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_EventWait(40);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x3100000, -1, 0x740000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x318, 140);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 100);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
    Audio_PlayCue(183);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Engine_EventWait(20);
    actor9->scale_x = 0x13333;
    actor9->scale_y = 0x13333;
    actor9->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    actor9->update = (void (*)(union FieldObject *))SceneActor_CopyActor8PositionWithFixedY;
    Engine_ActorSetAnimation(8, 4);
    actor8->bounce = 0x8000;
    actor8->x.fixed = 0x3120000;
    actor8->y.fixed = 0x200000;
    actor8->z.fixed = 0x5a0000;
    actor8->scale_x = 0x20000;
    actor8->scale_y = 0x20000;
    Engine_EventWait(10);
    Audio_PlayCue(183);
    Work_SetValuesIfNonNegative(0x40000, 0x20000, 0x10000);
    Engine_EventWait(20);
    actor10->x.fixed += 0xe0000;
    actor10->y.fixed += -0x80000;
    actor10->sprite->rotation = 0xc000;
    Audio_PlayCue(107);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 80);
    Audio_PlayCue(55);
    Work_SetValuesIfNonNegative(0x10000, 0x30000, 0x10000);
    Engine_ActorSetSpritePriority(8, 0);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 0);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
    actor0->timer = 0;
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, ArutinYama_LeaderRideScript);
    if (GameFlag_IsSet(0x205) != 0) {
        Actor_SetPosition(ACTOR_GERALD, 0x36e0000, 0x2100000);
        actor = Actor_Get(ACTOR_GERALD);
        actor->facing = FACING_SOUTH + FACING_STEP;
    }
    Camera_SetSpeed(0x14000, 0x2800);
    Camera_MoveTo(0x3120000, -1, 0x22c0000, 1);
    Engine_EventWait(a0);
    Engine_ActorSetSpritePriority(8, 1);
    Actor_SetSpeed(8, 0x195c2, 0xcae1);
    actor8->timer = 0;
    Engine_ActorEnableActionCallback(8, ArutinYama_LogRideScript);
    do {
        Engine_TaskWait(1);
    } while ((s16)actor0->timer == 0);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0);
    do {
        Engine_TaskWait(1);
    } while ((s16)actor8->timer == 0);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Actor_Get(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    actor9->x.fixed = 0x3120000;
    actor9->update = NULL;
    actor9->z.fixed = 0x26a0000;
    actor9->y.fixed = -0x400000;
    Actor_SetSpeed(8, 0x19999, 0xcccc);
    actor8->bounce = 0x1999;
    actor8->gravity = 0x3333;
    actor8->velocity_y = 0x40000;
    Actor_MoveToAndWait(8, 0x312, 0x25c);
    Actor_SetSpeed(8, 0x33333, 0x19999);
    Actor_SetDestination(8, 0x312, 0x284);
    Engine_EventWait(15);
    Work_SetValuesIfNonNegative(0x50000, 0x70000, 0x10000);
    Map_CopyCellsTo(25, 36, 43, 36, 11, 9);
    Map_CopyCellAttributes(25, 35, 10, 5, 43, 35);
    Actor_SetPosition(8, 0, 0);
    Actor_SetPosition(9, 0, 0);
    Engine_TaskAddCallback(FieldScene_RunScene3a4SequenceG, 0xc80);
    Engine_EventWait(80);
    Engine_TaskRemoveCallback(FieldScene_RunScene3a4SequenceG);
    Engine_EventWait(60);
    Audio_PlayCue(17);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(120);
    if (GameFlag_IsSet(0x205) != 0) {
        Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
        Actor_WalkTo(ACTOR_GERALD, 0x338, 0x22e);
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x356, 0x248);
    if (GameFlag_IsSet(0x205) != 0) {
        Engine_ActorSetAnimation(ACTOR_GERALD, 1);
        Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 0);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 40);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Camera_MoveTo(0x3140000, -0x400000, 0x2620000, 1);
    Engine_CameraWaitForMove();
    Audio_PlayCue(148);
    Engine_EventWait(240);
    if (GameFlag_IsSet(0x205) != 0) {
        Camera_SetSpeed(0x40000, 0x8000);
        Camera_MoveTo(0x3560000, 0, 0x2480000, 1);
        Engine_CameraWaitForMove();
        Actor_WalkToAndWait(ACTOR_GERALD, 0x348, 0x228);
        Actor_WalkToAndWait(ACTOR_GERALD, 0x356, 0x232);
        Engine_ActorSetAnimation(ACTOR_GERALD, 2);
        actor = Actor_Get(ACTOR_PARTY_LEADER);
        if (actor != NULL) {
            Actor_SetDestination(ACTOR_GERALD, actor->x.part.pixel, actor->z.part.pixel);
        }
        Engine_ActorWaitForMove(ACTOR_GERALD);
        Actor_SetPosition(ACTOR_GERALD, 0, 0);
    }
    Audio_PlayCueFromEventWork();
    GameFlag_Set(0x908);
}

void FieldScene_RunFallingRocksWarning(void)
{
    struct FieldActor *record;
    s32 msg;

    Engine_EventBegin();
    msg = (s32)MsgArutinWatchFallingRocks;
    Engine_MessageShowCentered(msg, 1);
    if (GameFlag_IsSet(0x908) == 0 && GameFlag_IsSet(0xf14) == 0) {
        GameFlag_Set(0x205);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x316, 140);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x30c, 140);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if (record != 0) {
            Actor_SetPosition(ACTOR_GERALD, record->x.fixed, record->z.fixed);
        }
        Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
        Actor_WalkToAndWait(ACTOR_GERALD, 0x320, 140);
        Actor_FaceDirection(ACTOR_GERALD, 0xc000, 20);
        Engine_EventSetMessage(msg + 1);
        Engine_ActorSetAnimation(ACTOR_GERALD, 4);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
        Engine_ActorJump(ACTOR_GERALD, 6, 0);
        Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
        Object_GetById(ACTOR_GERALD)->steering_flags &= ~ACTOR_FACES_MOTION;
        Actor_WalkToAndWait(ACTOR_GERALD, 0x318, 110);
        Engine_EventWait(1);
        Object_GetById(ACTOR_GERALD)->steering_flags |= ACTOR_FACES_MOTION;
        Audio_PlayCue(161);
        Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
        Object_GetById(ACTOR_GERALD)->steering_flags &= ~ACTOR_FACES_MOTION;
        Actor_WalkToAndWait(ACTOR_GERALD, 0x318, 120);
        Engine_EventWait(1);
        {
            struct FieldActor *record = Actor_Get(ACTOR_GERALD);

            record->steering_flags = ACTOR_FACES_MOTION | record->steering_flags;
        }
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Engine_EventWait(80);
        Audio_PlayCue(141);
        Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        Engine_EventWait(40);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0x8000, 40);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0, 40);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xc000, 40);
        Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
        Actor_FaceDirection(ACTOR_GERALD, 0x4000, 20);
        Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
        Actor_SetSpeed(ACTOR_GERALD, 0x28000, 0x14000);
        Engine_ActorSetAnimation(ACTOR_GERALD, 5);
        Actor_MoveToAndWait(ACTOR_GERALD, 0x31c, 138);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        Actor_MoveToAndWait(ACTOR_GERALD, 0x324, 140);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
        Actor_MoveToAndWait(ACTOR_GERALD, 0x324, 166);
        Actor_MoveToAndWait(ACTOR_GERALD, 0x2fc, 166);
        Actor_MoveToAndWait(ACTOR_GERALD, 0x2fc, 198);
        Actor_MoveToAndWait(ACTOR_GERALD, 0x312, 198);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
        Actor_MoveToAndWait(ACTOR_GERALD, 0x312, 246);
        Engine_ActorSetAnimation(ACTOR_GERALD, 1);
        Actor_SetPosition(ACTOR_GERALD, 0, 0);
        Engine_EventWait(40);
        FieldScene_RunSharedSetPiece(10);
    }
    Engine_EventEnd();
}

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

void SceneMotion_UpdateTimedActor(struct SceneMotion *work)
{
    if (work->delay != 0) {
        if (--work->delay == 1)
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    }
    if (work->velocity == 0) {
        Object_SetMode(work,1);
        work->y += -0x18000;
        if (work->y < work->ground) {
            if (work->active != 0) {
                Engine_AudioPlayCue(229);
                work->active = 0;
                work->delay = 4;
                Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
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

void FieldScene_BuildMultiPhasePresentation(void)
{
    struct Half p10;
    s32 p8;
    u8 *rec3;
    struct FieldActor *rec7;
    struct FieldActor *record;
    s32 v6;

    rec3 = (s32)Object_GetById(10);
    Engine_EventBegin();
    Engine_CameraSetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x12a0000, -1, 0x1510000, 1);
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
    Actor_SetSpeed(10, 0x13333, 0x9999);
    Actor_MoveToAndWait(10, 0x134, 0x123);
    Engine_ActorMoveToAndWait(10, 0x137, 215);
    ((struct ActorMotion *)rec3)->callback = 0;
    rec3[91] = p10.v;
    Engine_EventWait(16);
    Engine_ActorSetAnimation(10, 1);
    Engine_AudioPlayCue(229);
    Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(40);
    Actor_FaceDirection(10, 0xb000, 20);
    Actor_FaceDirection(10, 0x8000, 40);
    FieldScene_RunActorTenFourStepSequence();
    Engine_EventWait(40);
    Actor_FaceDirection(10, 0x5000, 20);
    Actor_FaceDirection(10, 0x3000, 40);
    Camera_MoveTo(0x14e0000, -1, 0xf40000, 1);
    ((struct ActorMotion *)rec3)->phase = 0;
    ((struct ActorMotion *)rec3)->step[0] = 0;
    ((struct ActorMotion *)rec3)->step[1] = 0;
    ((struct ActorMotion *)rec3)->callback = (s32)&(*(u8 *)SceneMotion_UpdateTimedActor);
    Actor_MoveToAndWait(10, 0x140, 232);
    Actor_MoveToAndWait(10, 0x154, 0x106);
    Engine_ActorMoveToAndWait(10, 0x176, 0x106);
    ((struct ActorMotion *)rec3)->callback = 0;
    Engine_EventWait(16);
    Engine_ActorSetAnimation(10, 1);
    Engine_AudioPlayCue(229);
    Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(20);
    Actor_FaceDirection(10, 0xf000, 20);
    Engine_ActorFaceDirection(10, 0xd000, 40);
    Engine_AudioPlayCue(153);
    record = Object_GetById(10);
    record->velocity_y = 0x40000;
    Engine_ActorSetAnimation(10, 2);
    Engine_ActorMoveToAndWait(10, 0x17c, 248);
    Engine_EventWait(10);
    Engine_AudioPlayCue(229);
    Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(40);
    Actor_FaceDirection(10, 0xb000, 20);
    Actor_FaceDirection(10, 0x8000, 40);
    Camera_MoveTo(0x1300000, -1, 0xd70000, 1);
    ((struct ActorMotion *)rec3)->phase = 0;
    ((struct ActorMotion *)rec3)->step[0] = 0;
    ((struct ActorMotion *)rec3)->step[1] = 0;
    ((struct ActorMotion *)rec3)->callback = (s32)&(*(u8 *)SceneMotion_UpdateTimedActor);
    Actor_MoveToAndWait(10, 0x149, 219);
    ((struct ActorMotion *)rec3)->callback = 0;
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(16);
    Engine_AudioPlayCue(229);
    Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(10, 0x8000, 40);
    Object_GetById(9)->motion_flags = p10.v;
    Engine_MapCopyCellAttributes(3, 0, 1, 1, 17, 13);
    Engine_MapCopyCellAttributes(3, 0, 1, 1, 18, 13);
    Engine_MapCopyCellAttributes(3, 0, 1, 1, 19, 13);
    Engine_ActorSetSpeed(10, 0x16666, 0xb333);
    record = Object_GetById(10);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_AudioPlayCue(153);
    v6 = 160;
    record = Object_GetById(10);
    record->velocity_y = (v6 << 11);
    Engine_ActorSetAnimation(10, 3);
    Actor_MoveToAndWait(10, 0x127, 215);
    Engine_ActorSetAnimation(10, 1);
    record = Object_GetById(10);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_AudioPlayCue(229);
    Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(40);
    Engine_AudioPlayCue(153);
    record = Object_GetById(10);
    record->velocity_y = (v6 << 11);
    Engine_ActorSetAnimation(10, 3);
    Actor_MoveToAndWait(10, 0x104, 215);
    Engine_ActorSetAnimation(10, 1);
    Engine_AudioPlayCue(229);
    Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(40);
    Actor_FaceDirection(10, 0x5000, 20);
    Engine_ActorFaceDirection(10, 0x3000, 20);
    Engine_AudioPlayCue(147);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(40);
    Engine_MapCopyCellAttributes(4, 0, 1, 1, 17, 13);
    Engine_MapCopyCellAttributes(2, 0, 1, 1, 18, 13);
    Engine_MapCopyCellAttributes(4, 0, 1, 1, 19, 13);
    rec7 = Object_GetById(0);
    Engine_CameraSetSpeed(0x4cccc, 0x9999);
    Engine_CameraMoveTo(rec7->x.fixed, rec7->y.fixed, rec7->z.fixed, 1);
    Engine_CameraWaitForMove();
    Call3(Engine_ObjectSetTargetAndCallback, 10, 0x10000, (s32)&ArutinYama_ActorScript);
    Engine_GameFlagSet(0x904);
    Engine_EventEnd();
}

union TimedActor {
    struct FieldActor actor;
    struct SceneMotion motion;
};

#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern s32 gKeyState;
#endif

void ArutinYama_RunLeapSequence(void)
{
    union TimedActor *actor;
    struct FieldActor *record;
    u32 n;

    actor = (union TimedActor *)Object_GetById(10);
    Engine_EventBegin();
    ObjectMotion_EnableActionAndResetMotion(10);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x1170000, 0x400000, 0xd80000, 1);
    Engine_CameraWaitForMove();
    Engine_AudioPlayCue(147);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(40);
    Actor_FaceDirection(10, 0x3000, 20);
    Actor_FaceDirection(10, 0x5000, 20);
    Actor_FaceDirection(10, 0x8000, 40);
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x800000, 0x400000, 0xca0000, 1);
    actor->motion.active = 0;
    actor->motion.timer = 0;
    actor->motion.delay = 0;
    actor->actor.gravity = 0x6666;
    actor->actor.update = (void (*)(union FieldObject *))SceneMotion_UpdateTimedActor;
    Actor_SetSpeed(10, 0x13333, 0x9999);
    Actor_MoveToAndWait(10, 212, 200);
    Actor_MoveToAndWait(10, 103, 200);
    actor->actor.update = NULL;
    actor->motion.state = 0;
    Engine_EventWait(10);
    Engine_ActorSetAnimation(10, 1);
    Engine_AudioPlayCue(229);
    Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(20);
    Actor_FaceDirection(10, 0x5000, 40);
    Object_GetById(10)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_SetSpeed(10, 0x13333, 0x9999);
    record = Object_GetById(10);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_AudioPlayCue(153);
    record = Object_GetById(10);
    record->velocity_y = 0x40000;
    Actor_SetAnimation(10, 3);
    Engine_ActorMoveToAndWait(10, 86, 214);
    Engine_ActorSetAnimation(10, 1);
    record = Object_GetById(10);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_EventWait(10);
    Engine_AudioPlayCue(229);
    Work_SetValuesIfNonNegative(0x20000, 0, 0x10000);
    Engine_EventWait(8);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(40);
    Object_GetById(10)->steering_flags |= ACTOR_FACES_MOTION;
    Actor_FaceDirection(10, 0x3000, 20);
    Engine_ActorFaceDirection(10, 0, 40);
    actor->motion.active = 0;
    actor->motion.timer = 0;
    actor->motion.delay = 0;
    actor->actor.update = (void (*)(union FieldObject *))SceneMotion_UpdateTimedActor;
    Actor_SetSpeed(10, 0x13333, 0x9999);
    Actor_MoveToAndWait(10, 120, 215);
    actor->actor.update = NULL;
    actor->motion.state = 0;
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(16);
    Engine_AudioPlayCue(229);
    Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(10, 0, 10);
    Engine_AudioPlayCue(147);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(80);
    Engine_ActorSetAnimation(10, 3);
    Call4(ArutinYama_StartLeafGust, 0x820000, 0, 0xa80000, 0);
    Engine_EventWait(60);
    for (n = 0; n < 60 && gKeyState == 0; n++) {
        Engine_EventWait(1);
    }
    actor = (union TimedActor *)Object_GetById(0);
    Camera_SetSpeed(0x4cccc, 0x9999);
    Engine_CameraMoveTo(actor->actor.x.fixed, actor->actor.y.fixed, actor->actor.z.fixed, 1);
    Engine_CameraWaitForMove();
    GameFlag_Set(0x905);
    Engine_EventEnd();
}
