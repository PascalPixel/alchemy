#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "shian.h"
#include "types.h"
#include "field_event.h"
#include "call.h"
#include "field_service.h"
#include "text/msg_ids.h"

#define FLAG_SHIAN_WATER_SPILLED 0x200

struct SceneState { u8 unk_00[14]; s16 field_0e; };

struct Actor { u8 unk_00[35]; u8 presentation_flags; };

struct MapActor {
    u8 reserved_00[6];
    u16 heading;
    s32 x;
    u8 reserved_0c[4];
    s32 z;
    u8 reserved_14[71];
    u8 tracking_state;
    u8 reserved_5c[8];
    s16 tracking_mode;
};

s32 ArcTan2(s32 z_delta, s32 x_delta);


void Engine_EventBegin();
s32 Engine_GameFlagIsSet();
void Engine_ActorFaceActor();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_ActorFaceDirection();
void Engine_EventEnd();
void Vector_AddPolarOffset();
void Engine_ActorSetAnimation();
void Engine_ActorSetDestinationOffset();
void Engine_ActorWaitForMove();
void Engine_AudioPlayCue();
s32 Engine_TaskAddCallback();
void Engine_ActorSetSpeed();
void Engine_ActorSetDestination();
void Object_RefreshSelectorById();
void Engine_EventShowMessageAndWait();

struct SpawnParams {
    s32 count;
    u8 unknown_04[20];
    u16 sprite;
    u16 pad_1a;
    s32 callback;
    u16 facing;
    u8 unknown_22[6];
};

void SceneEffect_SpawnPeriodicEffect();
void SceneEffect_AdvanceRotatingSprite();


void Vector_AddPolarOffset(s32 distance, s32 angle, union FieldCoordinate *pos);
s32 Object_CheckMovementCollision(struct FieldActor *actor, union FieldCoordinate *pos);


void BattleFx_SetQueuedSoundAndPlay(s32 music);

s32 SceneEffect_CalculatePositionDistance( s32 *first_position, s32 *second_position);
s32 ShianMura_WatchGateTrigger(struct FieldActor *self);

void SceneEffect_RequestFixedEffect(void)
{
    BattleFx_RunPageEffectForSlot(22, 1, 2);
}

void Actor_UpdatePresentationFlag(void)
{
    struct SceneState *scene = (void *)Object_GetById(0);
    if (scene->field_0e > 31) {
        ((struct Actor *)Object_GetById(20))->presentation_flags |= 2;
    } else {
        ((struct Actor *)Object_GetById(20))->presentation_flags &= (u8)~2;
    }
}

void SceneEffect_AdvanceRotatingSprite(void)
{
    u8 *entity = Object_GetById(19);
    u8 *sprite = (u8 *)((struct FieldActor *)entity)->sprite;
    ((struct FieldSprite *)sprite)->rotation += 0x1400;
}

void SceneEffect_SpawnPeriodicEffect(void)
{
    u8 *entity = Object_GetById(14);

    if ((gFrameCount & 3) == 0) {
        struct PeriodicEffectConfig config;
        config.kind = 1;
        config.variant = 9;
        config.id = 169;
        config.data = ShianMura_EffectConfig;
        Effect_Spawn(
            *(s32 *)(entity + 8),
            *(s32 *)(entity + 12),
            *(s32 *)(entity + 16) - 0x10000,
            0,
            -0x10000,
            -0x10000,
            0x330000,
            &config);
    }
}

s32 SceneEffect_CalculatePositionDistance(
    s32 *first_position,
    s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 SceneActor_UpdateTracking(
    struct MapActor *actor,
    struct MapActor *target,
    s32 distance_limit,
    s32 force_tracking)
{
    s32 result = 0;

    if (actor->tracking_state == 1 && actor->tracking_mode == 0) {
        Object_SetMode(actor, 1);
        return 1;
    }

    if (SceneEffect_CalculatePositionDistance(&target->x, &actor->x) < distance_limit ||
        force_tracking != 0) {
        u16 angle = ArcTan2(target->z - actor->z, target->x - actor->x);
        s32 prev = (angle - 0x1000) & 0xf000;
        s32 next = (angle + 0x1000) & 0xf000;
        s32 facing = angle & 0xf000;
        s32 heading = actor->heading & 0xf000;

        if (facing == heading || next == heading || prev == heading ||
            force_tracking != 0) {
            actor->tracking_state = 1;
            Object_SetMode(actor, 1);
            result = 1;
            actor->tracking_mode = result;
        } else {
            actor->tracking_state = 0;
            Object_SetMode(actor, 2);
            actor->tracking_mode = 0;
        }
    } else {
        actor->tracking_state = 0;
        Object_SetMode(actor, 2);
        actor->tracking_mode = 0;
    }

    return result;
}

s32 ShianMura_WatchGateTrigger(struct FieldActor *self)
{
    struct FieldActor *leader;
    s32 near = 0;

    if (self->target_x == ACTOR_NO_TARGET && self->target_z == ACTOR_NO_TARGET)
        return 0;
    leader = Object_GetById(0);
    if ((u32)((leader->x.fixed >> 20) - 17) <= 1 && leader->z.fixed >> 20 == 14 && self->x.fixed >> 20 <= 19
        && self->velocity_x <= 0) {
        if (leader->x.fixed <= self->x.fixed) {
            self->rise_counter++;
            near = 1;
        }
    } else {
        self->rise_counter = 0;
    }
    if (near && self->rise_counter > 119) {
        gEventWork->raised_trigger = 200;
        self->rise_counter = 0;
    }
    SceneActor_UpdateTracking(self, leader, 18, near);
    return 0;
}

u8 *SceneEffect_GetPrimaryData(void)
{
    return gShianMuraEntrances;
}

s32 SceneEffect_GetInitialValue(void)
{
    return 0;
}

u8 *SceneEffect_GetSecondaryData(void)
{
    return gShianMuraExits;
}

s32 SceneEffect_PrepareState(void)
{
    if (Engine_GameFlagIsSet(0x895) != 0)
        gShianMuraPlacements[0xbe] = 0;
    return (s32)gShianMuraPlacements;
}

void SceneEffect_ShowActorSetupMessage(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgShianYoungWarriorsVeryGallantCame);
    Engine_EventAskYesNo(9, 0);
    Engine_EventEnd();
}

void FieldScene_RunPrimarySequence(void)
{
    struct FieldActor *puddle;
    struct FieldActor *record;
    struct FieldActor *leader;
    s32 moved;
    s32 facing;
    s32 x;
    s32 z;
    s32 column;
    s32 scale_y;
    s32 scale_x;

    puddle = Object_GetById(20);
    Engine_EventBegin();
    moved = 0;
    record = Actor_Get(18);
    record->update = NULL;
    if (GameFlag_IsSet(FLAG_SHIAN_WATER_SPILLED) != 0 || (Object_GetById(18)->x.fixed >> 20) <= 19) {
        record = Actor_Get(18);
        facing = record->facing;
        Actor_FaceActor(18, ACTOR_PARTY_LEADER, 0);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgShianNowMustGetWaterAgain);
        if (GameFlag_IsSet(FLAG_SHIAN_WATER_SPILLED) == 0) {
            gEventWork->message++;
            Event_ShowMessage(18, 0);
            Object_GetById(18)->timer = 0;
            record = Object_GetById(18);
            record->facing = facing;
        } else {
            Event_ShowMessage(18, 0);
            Actor_FaceDirection(18, 0x8000, 20);
        }
        record = Actor_Get(18);
        record->update = (void *)ShianMura_WatchGateTrigger;
        Engine_EventEnd();
        return;
    }
    if ((Object_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 19) > 27
        && (Object_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 19) <= 29
        && (Object_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20) != 26) {
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
        Engine_ActorFaceActor(0, 18, 0);
        Engine_EventWait(5);
        leader = Object_GetById(ACTOR_PARTY_LEADER);
        record = Actor_Get(18);
        if (leader->x.fixed < record->x.fixed) {
            Object_GetById(0)->steering_flags &= ~ACTOR_FACES_MOTION;
            record = Object_GetById(18);
            Actor_WalkTo(ACTOR_PARTY_LEADER, ((record->x.fixed >> 20) << 4) - 8, 232);
            moved = 1;
        } else {
            Object_GetById(0)->steering_flags &= ~ACTOR_FACES_MOTION;
            record = Object_GetById(18);
            Actor_WalkTo(ACTOR_PARTY_LEADER, ((record->x.fixed >> 20) << 4) + 24, 232);
        }
        Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    }
    record = Actor_Get(18);
    record->target_x = ACTOR_NO_TARGET;
    record = Object_GetById(18);
    record->target_y = ACTOR_NO_TARGET;
    record = Actor_Get(18);
    record->target_z = ACTOR_NO_TARGET;
    Engine_ActorEnableActionCallback(18, 1);
    Engine_ActorSetAnimation(18, 1);
    Engine_ActorRunRepeatedMotion(18, 2);
    Engine_EventWait(10);
    Event_PlayCue(228);
    puddle->scale_x = 0x4ccc;
    puddle->scale_y = 0x4ccc;
    record = Object_GetById(18);
    x = record->x.fixed;
    record = Object_GetById(18);
    z = record->z.fixed >> 20;
    Actor_SetPosition(20, ((x >> 20) << 20) + (8 << 16), (z << 20) + (8 << 16));
    record = Object_GetById(18);
    column = record->x.fixed;
    record = Object_GetById(18);
    Map_CopyCellAttributes(16, 16, 1, 1, column >> 20, record->z.fixed >> 20);
    Engine_ActorSetSpritePriority(20, 2);
    puddle->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    do {
        Engine_TaskWait(3);
        scale_y = puddle->scale_y;
        scale_x = puddle->scale_x;
        puddle->scale_y = scale_y + 0x1999;
        scale_x += 0x1999;
        puddle->scale_x = scale_x;
    } while (scale_x <= 0xffff);
    Actor_ShowEmote(18, 0x105, 70);
    Actor_FaceActor(18, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(18, 0x103, 0);
    Engine_ActorStartRepeatedMotion(18, 2);
    Engine_EventWait(70);
    Engine_EventSetMessage((s32)MsgShianDoingMadeMeSpillMy);
    Event_ShowMessageAndWait(18, 0, 20);
    BattleFx_PlayQueuedSound();
    if ((Object_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20) == 26
        && (Object_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 20) > 13) {
        moved = 1;
    }
    if (moved != 0) {
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 10);
        Object_GetById(0)->steering_flags &= ~ACTOR_FACES_MOTION;
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
        Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    }
    Actor_SetSpeed(18, 0xcccc, 0x6666);
    record = Object_GetById(18);
    if ((record->z.fixed >> 20) != 14) {
        record = Actor_Get(18);
        Actor_WalkToAndWait(18, record->x.part.pixel, 232);
    }
    Actor_WalkToAndWait(18, 0x118, 232);
    GameFlag_Set(FLAG_SHIAN_WATER_SPILLED);
    Actor_Get(ACTOR_PARTY_LEADER)->steering_flags |= ACTOR_FACES_MOTION;
    Engine_EventEnd();
}

void SceneEffect_ActivateNearbyActor(void)
{
    u8 *leader = Object_GetById(ACTOR_PARTY_LEADER);
    if ((*(s32 *)(leader + 16) >> 20) <= 13)
        Engine_ActorSetSpritePriority(20, 1);
}

void ShianMura_ActivateActorTwentyPlatform(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 x;

    Engine_EventBegin();
    ((struct FieldActor *)Object_GetById(20))->priority_flags &= 253;
    v5 = 0;
    ((struct FieldActor *)Object_GetById(20))->motion_flags = v5;
    record = Object_GetById(20);
    x = *(s32 *)(record + 8);
    record = Object_GetById(20);
    Map_CopyCellAttributes(3, 17, 1, 1, (x >> 20), (*(s32 *)(record + 16) >> 20));
    Engine_TaskAddCallback((s32)Actor_UpdatePresentationFlag, 0xc80);
    GameFlag_Set(0x201);
    Engine_ActorSetSpritePriority(20, 2);
    Engine_EventEnd();
}

void ShianMura_RunNpcMeetScene(void)
{
    s32 flag;
    struct FieldActor *leader;
    s32 cb;
    s32 back;
    s32 pos[3];
    struct SpawnParams params;

    Engine_EventBegin();
    flag = Engine_GameFlagIsSet(0x202);
    if (flag != 0) {
        Engine_ActorFaceActor(14, 0, 0);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgShianKungFuStrong);
        Engine_EventShowMessage(14, 0);
        Engine_ActorFaceDirection(14, 0, 10);
        Engine_EventEnd();
    } else {
        Engine_EventSetMessage((s32)MsgShianHooWhaaaHachaaa);
        Engine_EventShowMessage(14, 0);
        Engine_ActorRunRepeatedMotion(0, 2);
        Object_GetById(0)->steering_flags &= ~ACTOR_FACES_MOTION;
        pos[0] = flag;
        pos[1] = flag;
        pos[2] = flag;
        leader = Object_GetById(0);
        FieldVector_AddPolarOffset(-0x80000, leader->facing, (struct FieldPosition *)pos);
        Engine_ActorSetAnimation(0, 2);
        Engine_ActorSetDestinationOffset(0, pos[0] / 0x10000, pos[2] / 0x10000);
        Engine_ActorWaitForMove(0);
        Object_GetById(0)->steering_flags |= ACTOR_FACES_MOTION;
        Engine_EventWait(30);
        Engine_ActorRunRepeatedMotion(14, 2);
        params.count = 1;
        Effect_Spawn(0xc00000, 0, 0x1380000, 0x1999, 0x3333, 0, 0x20001, 0);
        Effect_Spawn(0xc00000, 0, 0x1380000, 0x3333, 0x1999, 0, 0x20001, 0);
        Engine_AudioPlayCue(132);
        cb = (s32)SceneEffect_SpawnPeriodicEffect;
        Task_AddCallback((void (*)(void))cb, 0xc80);
        Object_GetById(14)->velocity_y = 0x60000;
        Object_GetById(14)->gravity = 0x10000;
        Object_GetById(14)->bounce = 0;
        Actor_SetSpeed(14, 0x30000, 0x18000);
        Actor_SetDestination(14, 168, 0x138);
        Engine_ActorWaitForMove(14);
        Engine_AudioPlayCue(134);
        Engine_ActorEnableActionCallback(19, (s32)ShianMura_Actor19Motion);
        Engine_TaskAddCallback((s32)SceneEffect_AdvanceRotatingSprite, 0xc80);
        params.sprite = 0x11b;
        params.callback = (s32)ShianMura_EffectScript;
        params.facing = 0x4000;
        Effect_Spawn(0xa80000, 0, 0x14c0000, 0, 0, 0, 0x720000, &params);
        Actor_SetDestination(14, 146, 0x138);
        Engine_ActorWaitForMove(14);
        Engine_TaskRemoveCallback((void (*)(void))cb);
        Effect_Spawn(0x900000, 0, 0x1380000, 0, 0, 0, 0x20001, 0);
        Effect_Spawn(0x900000, 0, 0x1380000, -0x3333, 0x1999, 0, 0x20001, 0);
        back = -0x8000;
        Effect_Spawn(0x900000, 0, 0x1380000, back, 0, 0, 0x20001, 0);
        Object_RefreshSelectorById(19);
        Engine_AudioPlayCue(124);
        Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0, 0, 0, 0x20001, 0);
        Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0x3333, 0, 0, 0x20001, 0);
        Effect_Spawn(0xa80000, 0x80000, 0x1380000, -0x3333, 0, 0, 0x20001, 0);
        Engine_TaskRemoveCallback(SceneEffect_AdvanceRotatingSprite);
        Object_GetById(19)->sprite->rotation = back;
        Object_GetById(14)->bounce = 0x4000;
        Object_GetById(14)->gravity = 0x10000;
        Engine_EventWait(30);
        Engine_ActorFaceDirection(14, 0, 20);
        Engine_ActorRunRepeatedMotion(14, 2);
        Engine_EventWait(20);
        Engine_EventShowMessageAndWait(14, 0, 20);
        Engine_GameFlagSet(0x202);
        BattleFx_PlayQueuedSound();
        Engine_EventEnd();
    }
}

void SceneEffect_RunActorSceneMessage(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgShianWarriorsFromSchoolStrongWarriors);
    Engine_EventAskYesNo(17, 0);
    Engine_EventEnd();
}

void ShianMura_HopLeaderAhead(void)
{
    s32 flags;
    struct FieldActor *leader;
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;

    leader = (struct FieldActor *)Object_GetById(0);
    flags = leader->motion_flags;
    if (Engine_GameFlagIsSet(0x200) != 0) {
        p = pos;
        p[0].fixed = (leader->x.fixed & -0x100000) + 0x80000;
        p[1].fixed = leader->y.fixed;
        p[2].fixed = (leader->z.fixed & -0x100000) + 0x80000;
        FieldVector_AddPolarOffset(0x200000, (leader->facing + 0x2000) & 0xc000, (struct FieldPosition *)p);
        if (((s32 (*)())Object_CheckMovementCollision)((s32)leader, (s32)p) == 0) {
            Engine_EventBegin();
            Object_SetMode(leader, 6);
            Engine_TaskWait(6);
            Engine_AudioPlayCue(152);
            Object_SetMode(leader, 7);
            leader->speed = 0x30000;
            leader->acceleration = 0x20000;
            leader->velocity_y = 0x40000;
            leader->motion_flags &= 126;
            Engine_ActorSetSpriteFlags(leader, 0);
            Engine_ObjectMotionSetPositionAndCommit(0, p[0].part.pixel, p[2].part.pixel);
            Object_SetMode(leader, 6);
            Engine_ActorSetSpriteFlags(leader, 1);
            leader->motion_flags = flags;
            Engine_EventEnd();
        }
    }
}

void ShianMura_WalkThroughExit(s32 a0)
{
    u32 i;
    s32 record;

    ((struct FieldActor *)Object_GetById(0))->motion_flags = 0;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    if (a0 == 6) {
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    } else {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
    }
    gEventWork->transition_frames = 16;
    Engine_EventRequestExit(a0);
}

void SceneEffect_DispatchStep(void)
{
    s16 *scene = (s16 *)gEventWork;
    u8 *shared0;
    s32 shared1;

    Engine_EventBegin();

    switch (scene[182]) {
    case 1:
        Engine_AudioPlayCue(158);
        Engine_MapAnimateCells(ShianMura_GateSteps1, 81, 18);
        break;
    case 2:
        Engine_AudioPlayCue(158);
        shared0 = ShianMura_GateSteps2;
        shared1 = 83;
        goto shared;
    case 3:
        Engine_AudioPlayCue(158);
        shared0 = ShianMura_GateSteps2;
        shared1 = 86;
        goto shared;
    case 4:
        Engine_AudioPlayCue(158);
        Engine_MapAnimateCells(ShianMura_GateSteps3, 84, 24);
        break;
    case 5:
        Engine_AudioPlayCue(158);
        Engine_MapAnimateCells(ShianMura_GateSteps3, 72, 7);
        break;
    case 6:
        Engine_AudioPlayCue(188);
        shared0 = ShianMura_GateSteps4;
        shared1 = 69;
    shared:
        Engine_MapAnimateCells(shared0, shared1, 11);
        break;
    case 7:
        Engine_AudioPlayCue(158);
        Engine_MapAnimateCells(ShianMura_GateSteps5, 83, 7);
        break;
    default:
        break;
    }

    ShianMura_WalkThroughExit(scene[182]);
    Engine_EventEnd();
}

void Scene_RunActorNineteenScript(void)
{
    s32 cb;

    Engine_EventBegin();
    ((s32 (*)())Engine_ActorEnableActionCallback)(19, (s32)ShianMura_Actor19Motion);
    cb = (s32)SceneEffect_AdvanceRotatingSprite;
    ((s32 (*)())Engine_TaskAddCallback)(cb, 0xc80);
    Object_RefreshSelectorById(19);
    Engine_AudioPlayCue(124);
    Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0, 0, 0, 0x20001, 0);
    Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0x3333, 0, 0, 0x20001, 0);
    Effect_Spawn(0xa80000, 0x80000, 0x1380000, -0x3333, 0, 0, 0x20001, 0);
    Engine_TaskRemoveCallback(cb);
    Object_GetById(19)->sprite->rotation = 0x8000;
    Engine_ActorSetPosition(21, 0xa80000, 0x1380000);
    Engine_EventWait(20);
    Engine_ActorFaceActor(14, 19, 0);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(10);
    Engine_EventSetMessage((s32)MsgShianWhaWhaHappened);
    if (GameFlag_IsSet(0x203)) {
        gEventWork->message++;
    }
    Engine_EventShowMessage(14, 0);
    Engine_GameFlagSet(0x203);
    BattleFx_PlayQueuedSound();
    Engine_EventEnd();
}

void ShianMura_EscortActorEighteen(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    ((void (*)())Engine_ActorEnableActionCallback)(18, 1);
    record = Actor_Get(18);
    *(s32 *)(record + 108) = 0;
    record = Actor_Get(18);
    *(s32 *)(record + 56) = -0x80000000;
    record = Object_GetById(18);
    *(s32 *)(record + 64) = -0x80000000;
    record = Object_GetById(18);
    *(s32 *)(record + 36) = 0;
    record = Object_GetById(18);
    *(s32 *)(record + 44) = 0;
    record = Object_GetById(18);
    *(s32 *)(record + 48) = 0;
    record = Actor_Get(18);
    *(s32 *)(record + 52) = 0;
    Actor_ShowEmote(18, 0x103, 0);
    Engine_ActorStartRepeatedMotion(18, 2);
    Engine_EventWait(60);
    Actor_SetSpeed(18, 0x18000, 0xc000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
    Actor_WalkTo(18, 0x118, 232);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x128, 232);
    Engine_ActorWaitForMove(18);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Engine_ActorEnableActionCallback(18, ShianMura_ActionTable);
    record = Actor_Get(18);
    *(s32 *)(record + 108) = (s32)ShianMura_WatchGateTrigger;
    Engine_EventEnd();
}

u8 *SceneEffect_GetTertiaryData(void)
{
    return gShianMuraEvents;
}

s32 ShianMura_SetupScene(void)
{
    struct FieldActor *actor;
    u32 n;
    s32 x;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    BattleFx_SetQueuedSoundAndPlay(169);
    if (gGameState.entrance > 9) {
        Engine_GameFlagClear(0x12f);
    }
    if (Engine_GameFlagIsSet(0x895)) {
        Actor_FaceDirection(13, 0x8000, 0);
        Actor_SetPosition(14, 0x920000, 0x1380000);
        Engine_ActorFaceDirection(14, 0, 0);
        if (Engine_GameFlagIsSet(0x89a)) {
            Engine_ActorSetPosition(17, 0, 0);
        }
    }
    if (Engine_GameFlagIsSet(0x8b0)) {
        Engine_ActorSetPosition(17, 0, 0);
    }
    for (n = 0; n <= 2; n++) {
        struct FieldActor *actor;
        actor = Object_GetById(n + 23);
        /* FAKEMATCH: one value spans priority and motion setup so the motion zero stays inside the loop and feeds the later actors. */
        x = 1;
        actor->sprite->priority = x;
        x = 0;
        actor->motion_flags = x;
        actor->collision_flags = 8;
        Engine_ActorSetSpriteFlags(actor, 0);
        ObjectGroup_SetChildValue(actor, 15);
        actor->priority_flags = (actor->priority_flags & 254) | 2;
    }
    if (Engine_GameFlagIsSet(0x202)) {
        Actor_SetPosition(14, 0x920000, 0x1380000);
        Engine_ActorFaceDirection(14, 0, 0);
    }
    if (Engine_GameFlagIsSet(0x201)) {
        Engine_ActorSetAnimation(20, 5);
        {
            s32 px = Object_GetById(20)->x.fixed;

            Engine_MapCopyCellAttributes(3, 17, 1, 1, px >> 20, Object_GetById(20)->z.fixed >> 20);
        }
        ((void (*)())Engine_TaskAddCallback)((s32)Actor_UpdatePresentationFlag, 0xc80);
    }
    Engine_ActorSetChildValue(18, 2);
    Object_GetById(18)->update = ShianMura_WatchGateTrigger;
    actor = Object_GetById(19);
    actor->motion_flags = x;
    actor->y.fixed = 0x100000;
    actor->target_y = 0x100000;
    actor->scale_x = 0x8ccc;
    actor->scale_y = 0x6666;
    actor->sprite->rotation = 0x8000;
    Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
    {
        /* FAKEMATCH: narrow local retains the short-range zero pool load. */
        u8 shown = 0;

        Object_GetById(21)->motion_flags = shown;
    }
    Object_GetById(21)->y.fixed = x;
    Object_GetById(21)->target_y = -0x80000000;
    return 0;
}
