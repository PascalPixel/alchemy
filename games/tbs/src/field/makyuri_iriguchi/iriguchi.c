#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#include "fieldobj.h"
#include "mem.h"
#include "types.h"
#include "makyuri.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "call.h"
#include "scene_ids.h"
#include "field_effect.h"
#include "io_reg.h"
#include "field_event.h"
#include "text/msg_ids.h"
#include "iwram_call.h"

void SceneActor_RunActorZeroHandledMotion(int page);

struct Src_39c {
    u8 pad00[8];
    s32 f8;
    u8 pad0c[4];
    s32 f16;
};

struct Work_39c {
    u8 pad00[8];
    s32 f8;
    s32 f12;
    s32 f16;
    u8 pad14[4];
    s32 f24;
    s32 f28;
    u8 pad20[68];
    s16 f100;
    u8 pad66[2];
    struct Src_39c *f104;
};

struct WorkPointers {
    u8 *event;
    u8 *unknown_04[7];
    u8 *scene;
};

extern const u8 MakyuriIriguchi_Actor8Path1[];
extern const u8 MakyuriIriguchi_Actor8Path2[];
extern const u8 MakyuriIriguchi_Actor8Path3[];
extern const s32 MakyuriIriguchi_RiseScript[];
extern const s32 MakyuriIriguchi_SparkBurstScript[];
extern const s32 Makyuri_ScaleCounterScript[];
extern u8 MakyuriIriguchi_SceneTableA[];
extern u8 MakyuriIriguchi_SceneTableB[];
extern u8 MakyuriIriguchi_SceneTableC[];
extern u8 MakyuriIriguchi_SceneTableD[];
void Makyuri_CyclePalette(void);
void BattleFx_PlayQueuedSound();
s32 ArcTan2(s32 deltaZ, s32 deltaX);
void Makyuri_TickSpawnTimer(void);
void Makyuri_RunActorMove(void);
void SceneState_ClearCurrentRecordAndReleaseTarget(void);
void MakyuriIriguchi_RaisePillar(void);
void SceneActor_RunActorZeroHandledMotion(int page);

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_CreateConfigured(s32 first, s32 second, s32 third, s32 fourth);
void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 MakyuriIriguchi_TrailSparks(struct FieldActor *a0);
s32 SceneAudio_PlayCue118AndReturnZero(void);
s32 SceneActor_FaceLeaderWhileGrounded(struct FieldActor *object);
void MakyuriIriguchi_TickSpawnTimer(void);
void MakyuriIriguchi_ClearCurrentRecordAndReleaseTarget(void);
void FieldScene_RunSingleStep(void);
void MakyuriIriguchi_TriggerPillarRise(void);
void FieldScene_RunStepWithValue1632(void);
void MakyuriIriguchi_DropLeaderToColumn(s32 a0);
void FieldScene_RunIndexedStep17(void);
void FieldScene_RunIndexedStep18(void);
void FieldScene_RunIndexedStep19(void);
void FieldScene_RunSupplementalSequenceTwo(void);
void FieldScene_RunIndexedStep63(void);
void MakyuriIriguchi_InstallActorEightPath(void);
void MakyuriIriguchi_SendActor8ByLeaderColumn(void);
void MakyuriIriguchi_ArriveWithSparks(void);
void MakyuriIriguchi_RunDoorScene(void);
void OverlayObject_SpawnKind24AtObject(struct FieldActor *src);
s32 SceneEffect_AdvanceAnchoredRiseFrame(struct Work_39c *work);
s32 OverlayObject_RampWords24And28Over16Frames(struct FieldActor *o);
s32 OverlayObject_AdvanceScaleCounter(struct FieldActor *o);
void OverlayObject_ReleasePublishedAttachment(void);
void MakyuriIriguchi_CrossDoorway(void);

void SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
    work->sprite->priority = mode;
}

void *OverlayObject_CreateConfigured(s32 first, s32 second, s32 third, s32 fourth)
{
    struct FieldActor *obj;
    struct FieldSprite *rec;

    obj = Engine_ObjectCreate(fourth, first, second, third);
    if (obj != NULL) {
        rec = obj->sprite;
        rec->priority = 0;
        obj->motion_flags = 0;
        obj->collision_flags = 8;
        Engine_ActorSetSpriteFlags(obj, 0);
        ObjectGroup_SetChildValue(obj, 0xE);
        Engine_ObjectSetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct FieldActor *result = Engine_ObjectCreate(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        struct FieldSprite *object = result->sprite;

        object->priority = 1;
        result->motion_flags = 0;
        result->collision_flags = 8;
        Engine_ActorSetSpriteFlags(result, 0);
        ObjectGroup_SetChildValue(result, 15);
        result->priority_flags = (result->priority_flags & 0xfe) | 2;
        return result;
    }
    return NULL;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/move.c"
#include "../common/effect/spawnov.c"
#include "../common/object/staged_actor.c"

s32 MakyuriIriguchi_TrailSparks(struct FieldActor *a0)
{
    s32 value;
    s32 magic;
    u8 storage[40];
    u8 *rec = storage;

    FIELD(rec, s32, 4) = 7;
    if ((gFrameCount & 1) == 0) {
        FIELD(rec, s32, 4) = 5;
    }
    FIELD(rec, s32, 8) = 0xcccc;
    FIELD(rec, s32, 12) = 0xcccc;
    FIELD(rec, s32, 0) = 0;
    value = Engine_RandomNext();
    magic = -(s32)(((u32)(value << 3) >> 16) * 0x3333);
    Effect_Spawn((a0->x.fixed + ((8 - (gFrameCount & 15)) << 16)), (a0->y.fixed + 0x1a0000), a0->z.fixed, 0, magic, 0, 0xb0000, rec);
    return 0;
}

s32 SceneAudio_PlayCue118AndReturnZero(void)
{
    Engine_AudioPlayCue(118);
    return 0;
}

s32 SceneActor_FaceLeaderWhileGrounded(struct FieldActor *object)
{
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

    if ((leader->z.fixed >> 19) <= 22) {
        object->facing = ArcTan2(
            leader->z.fixed - object->z.fixed,
            leader->x.fixed - object->x.fixed);
    } else if (object->facing != 0xc000) {
        Engine_ActorFaceDirection(MEARI, 0xc000, 0);
    }
    return 0;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/makyuri/palette_cycle.c"

void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void BattleFx_SetWeightedResult(s32 value, s32 weight);

void BattleFx_StartFadeOverlay(s32 value);
void BattleFx_SetQueuedSoundAndPlay(s32 sound);
void UiText_ShowCenteredMessage(s32 message, s32 a1, s32 a2);

void MakyuriIriguchi_TickSpawnTimer(void)
{
    Makyuri_TickSpawnTimer();
}

void MakyuriIriguchi_ClearCurrentRecordAndReleaseTarget(void)
{
    SceneState_ClearCurrentRecordAndReleaseTarget();
}

void FieldScene_RunSingleStep(void)
{
    Makyuri_RunActorMove();
}

void MakyuriIriguchi_TriggerPillarRise(void)
{
    MakyuriIriguchi_RaisePillar();
}

void *SceneData_GetSceneTableA(void) { return MakyuriIriguchi_SceneTableA; }

int SceneData_ReturnZero(void) { return 0; }

void *SceneData_GetSceneTableB(void) { return MakyuriIriguchi_SceneTableB; }

void *SceneData_GetSceneTableC(void) { return MakyuriIriguchi_SceneTableC; }

void FieldScene_RunStepWithValue1632(void)
{
    Engine_EventBegin();
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_MessageShowCentered((s32)MsgMakyuriWhoHonorsHeart, 1);
    Engine_EventEnd();
}

void SceneActor_RunActorZeroHandledMotion(s32 a)
{
    u8 *v = Object_GetById(ACTOR_PARTY_LEADER);
    Engine_EventBegin();
    Engine_AudioPlayCue(0xe4);
    F(v, s32, 0x6c) = (s32)MakyuriIriguchi_TrailSparks;
    F(v, s32, 0x30) = 0x3333;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
    F(v, s32, 0x6c) = 0;
    Engine_EventWait(30);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(a);
    Engine_EventEnd();
}

void MakyuriIriguchi_DropLeaderToColumn(s32 a0)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_AudioPlayCue(228);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_EventWait(8);
    Actor_SetPosition(ACTOR_PARTY_LEADER, ((a0 << 19) + 0x80000), 0);
    Engine_EventWait(30);
}

void FieldScene_RunIndexedStep17(void)
{
    SceneActor_RunActorZeroHandledMotion(17);
}

void FieldScene_RunIndexedStep18(void)
{
    SceneActor_RunActorZeroHandledMotion(18);
}

void FieldScene_RunIndexedStep19(void)
{
    SceneActor_RunActorZeroHandledMotion(19);
}

void *SceneData_GetSceneTableD(void) { return MakyuriIriguchi_SceneTableD; }

void FieldScene_RunSupplementalSequenceTwo(void)
{
    s32 a;
    s32 b;
    s32 zero;
    s32 counter;
    s32 x;
    s32 y;
    s32 t;
    s32 record;
    u8 *slot;
    u8 slot16[40];

    a = Object_GetById(ACTOR_PARTY_LEADER)->x.fixed / 0x100000;
    b = Object_GetById(ACTOR_PARTY_LEADER)->z.fixed / 0x100000;
    if (a == 12 && b == 32) {
        Engine_EventBegin();
        ColorBuffer_ApplyTarget(0x10000, 0);
        Engine_ColorBufferInterpolate(60);
        Engine_EventWait(120);
        ColorBuffer_ApplyTarget(0x10005, 1);
        Engine_ColorBufferInterpolate(60);
        Engine_EventWait(40);
        counter = 0;
        slot = slot16;
        zero = 0;
        do {
            *(s32 *)(slot) = 1;
            {
                u16 shown = 0x11e;

                *(u16 *)(slot + 24) = shown;
            }
            *(s32 *)(slot + 28) = (s32)MakyuriIriguchi_SparkBurstScript;
            Engine_AudioPlayCue(246);
            x = 208 - ((u32)(Random_Next() << 4) >> 16);
            y = 560 - ((u32)(Random_Next() << 4) >> 16);
            t = ((u32)(Engine_RandomNext() << 2) >> 16);
            record = Math_Divide((((t << 4) - t) << 16) + 0x3c0000, 100);
            Effect_Spawn(x << 16, 0, y << 16, 0, record, zero, 0x320001, slot);
            Engine_EventWait(4);
            counter = counter + 1;
        } while ((u32)counter <= 14);
        Engine_AudioPlayCue(220);
        Engine_EventWait(60);
        GameFlag_Set(0x875);
        Engine_TaskAddCallback((s32)Makyuri_CyclePalette, 0xc80);
        Map_CopyCellsTo(37, 98, 10, 97, 5, 3);
        Map_CopyCellAttributes(70, 32, 13, 7, 6, 32);
        ColorBuffer_ApplyTarget(0x10000, 0);
        Engine_ColorBufferInterpolate(60);
        Engine_EventWait(120);
        Engine_EventEnd();
    }
}

void FieldScene_RunIndexedStep63(void)
{
    Engine_EventRequestExit(63);
}

void MakyuriIriguchi_InstallActorEightPath(void)
{
    Engine_GameFlagClear(0x205);
    Engine_ActorEnableActionCallback(8, MakyuriIriguchi_Actor8Path1);
}

void MakyuriIriguchi_SendActor8ByLeaderColumn(void)
{
    struct FieldActor *record;
    s32 field8;
    s32 quotient;

    record = Object_GetById(ACTOR_PARTY_LEADER);
    field8 = record->x.fixed;
    quotient = field8 / 0x100000;
    Engine_GameFlagSet(0x205);
    if (quotient == 7) {
        Engine_ActorEnableActionCallback(8, MakyuriIriguchi_Actor8Path2);
    } else {
        Engine_ActorEnableActionCallback(8, MakyuriIriguchi_Actor8Path3);
    }
}

void MakyuriIriguchi_OpenEntrance(void)
{
    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Party_SetFields1ceAnd1d0((s32)&SceneId_MakyuriIriguchi, 31);
    Battle_SetStart(3);
    BattleFx_SetWeightedResult(36, 1);
    Engine_EventEnd();
}

void MakyuriIriguchi_ArriveWithSparks(void)
{
    struct FieldActor *actor;
    s32 flag;
    s32 record;

    actor = (struct FieldActor *)Object_GetById(ACTOR_PARTY_LEADER);
    flag = GameFlag_IsSet(0x109);
    if (flag == 0) {
        Engine_EventBegin();
        Camera_MoveTo(-1, -1, -1, 0);
        actor->motion_flags = 0;
        Engine_ActorSetPosition(0, actor->x.part.pixel << 16, (actor->z.part.pixel << 16) + -0x100000);
        Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
        record = Actor_Get(ACTOR_PARTY_LEADER);
        Engine_ActorSetSpriteFlags(record, 0);
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Engine_AudioPlayCue(228);
        actor->update = (void (*)(union FieldObject *))MakyuriIriguchi_TrailSparks;
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
        Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
        Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
        record = Actor_Get(ACTOR_PARTY_LEADER);
        Engine_ActorSetSpriteFlags(record, 1);
        actor->sprite->priority = 1;
        Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 10);
        actor->motion_flags = 3;
        actor->update = NULL;
        BattleFx_PlayQueuedSound();
        Engine_EventEnd();
    }
}

s32 MakyuriIriguchi_ApplyEntryState(void)
{
    struct EffectOptions options;
    s32 velocity[3];
    struct FieldActor *actor;
    u32 i;
    s32 angle;
    s32 flag;

    /* FAKEMATCH: the do/while and the held values keep the blend constants in registers ahead of their address loads, as in MAKYURI_HEYA. */
    do {
        s32 blend = 0x3f40;
        REG_BLDCNT = blend;
    } while (0);
    {
        s32 alpha = 0x1010;
        REG_BLDALPHA = alpha;
    }
    Makyuri_SpawnLightObjects(21, (struct MakyuriLights *)gSceneState);
    GameFlag_Set(0x111);
    gGameState.retreat_entrance = 11;
    gGameState.retreat_scene = (s32)&SceneId_MakyuriHeya4;
    BattleFx_StartFadeOverlay(0);
    if (GameFlag_IsSet(0x875))
        Engine_TaskAddCallback(Makyuri_CyclePalette, 0xc80);
    else
        Makyuri_ClearPalette();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    switch (gGameState.entrance) {
    case 1:
        if (!GameFlag_IsSet(0x872))
            Engine_EventRequestExit(20);
    case 2:
        Actor_Get(12)->scale_x = -0x10000;
        Actor_Get(13)->scale_x = -0x10000;
        Actor_Get(14)->scale_x = -0x10000;
        Engine_TaskWait(1);
        break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        if (GameFlag_IsSet(0x875)) {
            Map_CopyCellAttributes(84, 5, 10, 7, 20, 5);
            Map_CopyCellAttributes(101, 5, 12, 7, 37, 5);
        }
        break;
    case 3:
    case 4:
    case 5:
    case 6:
        Engine_TaskAddCallback(Makyuri_CyclePalette, 0xc80);
        if (GameFlag_IsSet(0x875)) {
            Map_CopyCellsTo(37, 98, 10, 97, 5, 3);
            Engine_MapRedraw();
            Engine_TaskWait(1);
            Map_CopyCellAttributes(70, 32, 13, 7, 6, 32);
        }
        if (gGameState.entrance != 6)
            break;
        flag = GameFlag_IsSet(0x251);
        if (flag != 0)
            break;
        GameFlag_Set(0x251);
        Engine_EventBegin();
        Camera_MoveTo(-1, -1, -1, 0);
        Engine_MapRedraw();
        Engine_TaskWait(1);
        Actor_Get(0)->y.fixed = 0x820000;
        Actor_Get(0)->gravity = 0x8000;
        Actor_Get(0)->bounce = flag;
        Actor_Get(0)->motion_flags = flag;
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Engine_EventWait(30);
        Actor_Get(0)->motion_flags = 3;
        Engine_AudioPlayCue(204);
        Engine_EventWait(24);
        actor = Actor_Get(0);
        options.palette = 7;
        for (i = 0; i <= 16; i++) {
            angle = i << 12;
            velocity[0] = Engine_MathCos(angle);
            velocity[1] = 0;
            velocity[2] = Engine_MathSin(angle);
            velocity[0] -= velocity[0] / 4;
            velocity[2] -= velocity[2] / 2;
            Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, velocity[0], velocity[1],
                         velocity[2], EFFECT_USE_PALETTE | 1, &options);
        }
        Engine_AudioPlayCue(188);
        Actor_SetAttachedEffect(0, 0x101);
        Engine_ActorSetAnimation(0, 22);
        Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Engine_MapRenderWaitForValues();
        Actor_SetAttachedEffect(0, 0x100);
        Actor_Get(0)->gravity = 0x10000;
        Actor_Get(0)->bounce = 0x4000;
        if (!GameFlag_IsSet(0x875)) {
            ColorBuffer_ApplySource(0x10000, 0);
            ColorBuffer_ApplyTarget(0x10003, 1);
            Engine_ColorBufferInterpolate(30);
            Engine_EventWaitForScreen();
            Engine_ActorSetAnimation(0, 1);
            Engine_EventWait(30);
            UiText_ShowCenteredMessage((s32)MsgMakyuriWhoHonorsHeart, 0, 0);
            ColorBuffer_ApplyTarget(0x10000, 0);
            Engine_ColorBufferInterpolate(30);
        }
        Engine_EventEnd();
        break;
    case 18:
    case 19:
    case 20:
        MakyuriIriguchi_ArriveWithSparks();
    case 17:
        BattleFx_SetQueuedSoundAndPlay(170);
        break;
    case 25:
        Actor_SetChildValue(0, 15);
        Engine_ActorSetSpriteFlags(Actor_Get(0), 0);
        Engine_EventBegin();
        Engine_MapRedraw();
        Engine_TaskWait(1);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Engine_EventWait(120);
        Engine_EventRequestExit(50);
        Engine_EventEnd();
        break;
    case 30:
        if (!GameFlag_IsSet(0x109))
            MakyuriIriguchi_CrossDoorway();
        else
            Map_CopyCellAttributes(0, 0, 3, 3, 7, 9);
        break;
    case 31:
        if (!GameFlag_IsSet(0x109))
            MakyuriIriguchi_RunDoorScene();
        break;
    }
    return 0;
}

void MakyuriIriguchi_CrossDoorway(void)
{
    s32 i;
    struct FieldActor *ivan;
    struct FieldActor *actor;

    Engine_EventBegin();
    actor = Object_GetById(12);
    actor->scale_x = -0x10000;
    actor = Object_GetById(13);
    actor->scale_x = -0x10000;
    actor = Object_GetById(14);
    actor->scale_x = -0x10000;
    Actor_SetPosition(3, 0x880000, 0xb80000);
    Actor_SetPosition(0, 0x880000, 0x1280000);
    Actor_SetPosition(8, 0x880000, 0x980000);
    Actor_SetSpeed(3, 0x18000, 0xc000);
    Actor_SetSpeed(8, 0x18000, 0xc000);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Engine_CameraSetSpeed(0xcccc, 0x1999);
    Engine_CameraMoveTo(0x880000, -1, 0xb80000, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_ActorRunRepeatedMotion(3, 1);
    Object_GetById(8)->steering_flags &= ~ACTOR_FACES_MOTION;
    Engine_EventWait(20);
    for (i = 0; i < 2; i++) {
        Engine_ActorWalkTo(3, 152, 168);
        Engine_EventWait(10);
        Engine_ActorEnableActionCallback(8, (s32)MakyuriIriguchi_Actor8Path3);
        Engine_ActorWaitForMove(3);
        Actor_FaceDirection(3, 0xc000, 30);
        Engine_ActorRunRepeatedMotion(3, 1);
        ivan = Object_GetById(3);
        ivan->steering_flags &= ~ACTOR_FACES_MOTION;
        Engine_ActorWalkTo(3, 136, 184);
        Engine_EventWait(10);
        Engine_ActorEnableActionCallback(8, (s32)MakyuriIriguchi_Actor8Path1);
        Engine_ActorWaitForMove(3);
        Object_GetById(3)->steering_flags |= ACTOR_FACES_MOTION;
        Engine_EventWait(30);
        Engine_ActorWalkTo(3, 120, 168);
        Engine_EventWait(5);
        Engine_ActorEnableActionCallback(8, (s32)MakyuriIriguchi_Actor8Path2);
        Engine_ActorWaitForMove(3);
        Engine_ActorFaceDirection(3, 0xc000, 30);
        Engine_ActorRunRepeatedMotion(3, 1);
        Engine_EventWait(15);
        Object_GetById(3)->steering_flags &= ~ACTOR_FACES_MOTION;
        Engine_ActorWalkTo(3, 136, 184);
        Engine_EventWait(15);
        Engine_ActorEnableActionCallback(8, (s32)MakyuriIriguchi_Actor8Path1);
        Engine_ActorWaitForMove(3);
        Engine_ActorRunRepeatedMotion(3, 1);
        Object_GetById(3)->steering_flags |= ACTOR_FACES_MOTION;
    }
    Engine_EventWait(20);
    Engine_ActorShowEmote(3, 0x102, 60);
    actor = Object_GetById(3);
    actor->update = (void (*)(union FieldObject *))SceneActor_FaceLeaderWhileGrounded;
    Engine_CameraMoveToActor(0, 1);
    Engine_EventWait(30);
    Actor_WalkToAndWait(0, 136, 0x108);
    Engine_CameraWaitForMove();
    Map_CopyCellAttributes(0, 0, 3, 3, 7, 9);
    Engine_EventEnd();
}

void MakyuriIriguchi_RunDoorScene(void)
{
    s32 record;

    if (GameFlag_IsSet(0x250) == 0) {
        GameFlag_Set(0x250);
        Engine_EventBegin();
        record = Actor_Get(12);
        *(s32 *)(record + 24) = -0x10000;
        record = Object_GetById(13);
        *(s32 *)(record + 24) = -0x10000;
        record = Actor_Get(14);
        *(s32 *)(record + 24) = -0x10000;
        Actor_SetPosition(MEARI, 0x880000, 0x900000);
        Actor_FaceDirection(MEARI, 0x4000, 10);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Engine_EventWait(60);
        Actor_FaceActor(MEARI, ACTOR_PARTY_LEADER, 0);
        Engine_ActorSetAnimationAndWait(MEARI, 3);
        Engine_EventWait(30);
        Actor_WalkTo(MEARI, 136, 72);
        Engine_EventWait(40);
        Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        Engine_ActorWaitForMove(MEARI);
        Actor_SetPosition(MEARI, 0, 0);
        GameFlag_Set(0x872);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
        Engine_EventEnd();
    }
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/makyuri/lights.c"

void OverlayObject_SpawnKind24AtObject(struct FieldActor *src)
{
    struct FieldActor *obj = Engine_ObjectCreate(24, src->x.fixed, src->y.fixed, src->z.fixed);

    if (obj != 0) {
        struct FieldSprite *sprite = obj->sprite;

        Engine_ObjectSetScript(obj, MakyuriIriguchi_RiseScript);
        obj->motion_flags = 0;
        obj->terrain_id = 1;
        obj->priority_flags = 2;
        if (sprite != 0) {
            AnimationObjects_SelectAnimation(sprite, 2);
            sprite->flags = 0;
            ((struct FieldSpritePriority *)sprite)->priority = 3;
        }
    }
}

s32 SceneEffect_AdvanceAnchoredRiseFrame(struct Work_39c *work)
{
    struct Src_39c *source = work->f104;
    s16 step = ++work->f100;
    s32 seed;

    if (step > 31) {
        return 0;
    }

    seed = Engine_MathSin(step << 10);
    work->f24 = seed;
    work->f28 = seed;
    work->f8 = source->f8;
    work->f12 += 0x10000;
    work->f16 = source->f16;
    return 1;
}

s32 OverlayObject_RampWords24And28Over16Frames(struct FieldActor *o)
{
    u16 *c = &o->timer;
    s16 n;

    *c = *c + 1;
    n = (s16)*c;
    if (n > 16) {
        return 0;
    }
    o->scale_x = (n * 3) << 10;
    o->scale_y = (n * 3) << 10;
    return 1;
}

s32 OverlayObject_AdvanceScaleCounter(struct FieldActor *o)
{
    u16 *c = &o->timer;
    s16 n;

    *c = *c + 1;
    n = (s16)*c;
    if (n > 16) {
        return 0;
    }
    o->scale_x = (n << 11) + 0x10000;
    o->scale_y = (n << 11) + 0x10000;
    return 1;
}

// ここに共通ソースが入る（ROM の並びのとおり）

#include "pillar.c"

void SceneState_ClearCurrentRecordAndReleaseTarget(void)
{
    s32 *record = **(s32 ***)&((struct WorkPointers *)&gEventWork)->scene;
    s32 *target;

    if (record[0] == 0) {
        return;
    }

    record[0] = 0;
    Engine_GameFlagClear(0x161);

    target = (s32 *)record[5];
    if (target != 0) {
        *(short *)((u8 *)target + 0x64) = 0;
        Engine_ObjectSetScript(target, (s32)Makyuri_ScaleCounterScript);
        Object_SetMode(target, 7);
        record[5] = 0;
    }
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/makyuri/makyuri.c"

void OverlayObject_ReleasePublishedAttachment(void)
{
    u8 **pub = Runtime_AllocateBlock(35, 4);
    u8 *state;
    u8 *obj;

    if (pub == 0)
        return;
    state = *pub;
    obj = *(u8 **)(state + 20);
    if (obj == 0)
        return;
    Engine_ObjectDispatchRelease(obj);
    *(u8 **)(state + 20) = 0;
}
