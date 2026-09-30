#include "ENTRANCE.H"
#include "CALL.H"
#include "SCENE_IDS.H"
#include "TYPES.H"
#include "MAKYURI.H"
#include "FIELD_EFFECT.H"

extern u8 MsgMakyuriWhoHonorsHeart[];

void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void BattleFx_SetWeightedResult(s32 value, s32 weight);

void Makyuri_SpawnLightObjects(s32 count, s32 base);
void Makyuri_ClearPalette(void);
void Makyuri_CyclePalette(void);
void BattleFx_StartFadeOverlay(s32 value);
void BattleFx_SetQueuedSoundAndPlay(s32 sound);
void UiText_ShowCenteredMessage(s32 message, s32 a1, s32 a2);
void MakyuriIriguchi_ArriveWithSparks(void);
void MakyuriIriguchi_CrossDoorway(void);
void MakyuriIriguchi_RunDoorScene(void);

/* An actor's lift: how fast it rises and what pulls it back. */
struct ActorLift {
    s32 gravity;
    s32 rise;
};

#define ACTOR_LIFT(actor) ((struct ActorLift *)(actor)->unknown_44)

void Engine_EventBegin();
void Engine_ActorSetPosition();
void ObjectMotion_SetSpeedParameters();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_ActorRunRepeatedMotion();
void Battle_WaitMode0();
void Engine_ActorWalkTo();
void Engine_ActorEnableActionCallback();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Engine_ActorFaceDirection();
void Engine_ActorShowEmote();
void Engine_CameraMoveToActor();
void Engine_ActorWalkToAndWait();
void Engine_CameraWaitForMove();
void Map_CopyCellAttributeRect();
void Engine_EventEnd();
s32 SceneActor_FaceLeaderWhileGrounded(u8 *object);

/* Actor 8's action tables in the overlay's data. */
extern const u8 MakyuriIriguchi_Actor8Path1[];
extern const u8 MakyuriIriguchi_Actor8Path2[];
extern const u8 MakyuriIriguchi_Actor8Path3[];

void FieldScene_Forward31d4(void)
{
    Makyuri_TickSpawnTimer();
}

void FieldScene_CallHelper2e58(void)
{
    SceneState_ClearCurrentRecordAndReleaseTarget();
}

void FieldScene_RunSingleStep(void)
{
    Makyuri_RunActorMove();
}

void FieldScene_CallHelper2d50(void)
{
    MakyuriIriguchi_RaisePillar();
}

/* Contiguous unnamed leaf-owner run for resource_39b. */
void *SceneData_GetSceneTableA(void) { return MakyuriIriguchi_SceneTableA; }

int SceneData_ReturnZero(void) { return 0; }

void *SceneData_GetSceneTableB(void) { return MakyuriIriguchi_SceneTableB; }

void *SceneData_GetSceneTableC(void) { return MakyuriIriguchi_SceneTableC; }

/* Apply the overlay's common actor-0 presentation preset. */
void FieldScene_RunStepWithValue1632(void)
{
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Message_ShowCentered((s32)MsgMakyuriWhoHonorsHeart, 1);
    Event_End();
}

void SceneActor_RunActorZeroHandledMotion(s32 a)
{
    u8 *v = Actor_Get(ACTOR_PARTY_LEADER);
    Event_Begin();
    Audio_PlayCue(0xe4);
    F(v, s32, 0x6c) = (s32)MakyuriIriguchi_TrailSparks;
    F(v, s32, 0x30) = 0x3333;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    F(v, s32, 0x6c) = 0;
    Event_Wait(30);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(a);
    Event_End();
}

void MakyuriIriguchi_DropLeaderToColumn(s32 a0)
{
    u32 i;
    s32 record;

    Event_Begin();
    Audio_PlayCue(228);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpriteFlags(record, 0);
    Event_Wait(8);
    Actor_SetPosition(ACTOR_PARTY_LEADER, ((a0 << 19) + 0x80000), 0);
    Event_Wait(30);
}

/* Contiguous unnamed leaf-owner run for resource_39b. */

/* Clear the scene flag and point actor 8 at its first local path. */
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

    a = *(s32 *)(Value1(Object_GetById, ACTOR_PARTY_LEADER) + 8) / 0x100000;
    b = *(s32 *)(Value1(Object_GetById, ACTOR_PARTY_LEADER) + 16) / 0x100000;
    if (a == 12 && b == 32) {
        Event_Begin();
        ColorBuffer_ApplyTarget(0x10000, 0);
        ColorBuffer_Interpolate(60);
        Event_Wait(120);
        ColorBuffer_ApplyTarget(0x10005, 1);
        ColorBuffer_Interpolate(60);
        Event_Wait(40);
        counter = 0;
        slot = slot16;
        zero = 0;
        do {
            *(s32 *)(slot) = 1;
            {
                s32 shown = 0x11e;

                *(u16 *)(slot + 24) = shown;
            }
            *(s32 *)(slot + 28) = (s32)MakyuriIriguchi_SparkBurstScript;
            Audio_PlayCue(246);
            x = 208 - ((u32)(Random_Next() << 4) >> 16);
            y = 560 - ((u32)(Random_Next() << 4) >> 16);
            t = ((u32)(Engine_RandomNext() << 2) >> 16);
            record = Math_Divide((((t << 4) - t) << 16) + 0x3c0000, 100);
            Effect_Spawn(x << 16, 0, y << 16, 0, record, zero, 0x320001, slot);
            Event_Wait(4);
            counter = counter + 1;
        } while ((u32)counter <= 14);
        Audio_PlayCue(220);
        Event_Wait(60);
        GameFlag_Set(0x875);
        Engine_TaskAddCallback((s32)Makyuri_CyclePalette, 0xc80);
        Map_CopyCellsTo(37, 98, 10, 97, 5, 3);
        Map_CopyCellAttributes(70, 32, 13, 7, 6, 32);
        ColorBuffer_ApplyTarget(0x10000, 0);
        ColorBuffer_Interpolate(60);
        Event_Wait(120);
        Event_End();
    }
}

void FieldScene_RunIndexedStep63(void)
{
    Event_RequestExit(63);
}

void FieldScene_RunActor8StepWithTableA820(void)
{
    GameFlag_Clear(0x205);
    Engine_ActorEnableActionCallback(8, MakyuriIriguchi_Actor8Path1);
}

void MakyuriIriguchi_SendActor8ByLeaderColumn(void)
{
    s32 record;
    s32 field8;
    s32 quotient;

    record = Object_GetById(ACTOR_PARTY_LEADER);
    field8 = *(s32 *)(record + 8);
    quotient = field8 / 0x100000;
    GameFlag_Set(0x205);
    if (quotient == 7) {
        Engine_ActorEnableActionCallback(8, MakyuriIriguchi_Actor8Path2);
    } else {
        Engine_ActorEnableActionCallback(8, MakyuriIriguchi_Actor8Path3);
    }
}

void MakyuriIriguchi_OpenEntrance(void)
{
    s32 game;

    Event_Begin();
    Engine_ActorRunRepeatedMotion(8, 2);
    Event_Wait(20);
    gEventWork->start_transition = 0x200;
    Party_SetFields1ceAnd1d0((s32)&SceneId_MakyuriIriguchi, 31);
    game = (s32)&gGameState;
    /* FAKEMATCH: the do/while loads the game state's address ahead of the
     * offset for the byte store. */
    do {
        *(u8 *)(game + 0x22b) = 3;
        BattleFx_SetWeightedResult(36, 1);
    } while (0);
    Event_End();
}

void MakyuriIriguchi_ArriveWithSparks(void)
{
    struct FieldActor *actor;
    s32 flag;
    s32 record;

    actor = (struct FieldActor *)Object_GetById(ACTOR_PARTY_LEADER);
    flag = GameFlag_IsSet(0x109);
    if (flag == 0) {
        Event_Begin();
        Camera_MoveTo(-1, -1, -1, 0);
        actor->motion_flags = 0;
        Engine_ActorSetPosition(0, actor->x.part.pixel << 16, (actor->z.part.pixel << 16) + -0x100000);
        Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
        record = Actor_Get(ACTOR_PARTY_LEADER);
        Actor_SetSpriteFlags(record, 0);
        Event_OpenScreen();
        Event_WaitForScreen();
        Audio_PlayCue(228);
        actor->update = (void (*)(union FieldObject *))MakyuriIriguchi_TrailSparks;
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
        Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
        Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
        record = Actor_Get(ACTOR_PARTY_LEADER);
        Actor_SetSpriteFlags(record, 1);
        actor->sprite->priority = 1;
        Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 10);
        actor->motion_flags = 3;
        actor->update = NULL;
        BattleFx_PlayQueuedSound();
        Event_End();
    }
}

/* Mercury Lighthouse entrance: set the blend, lights and palette cycle and
 * record the retreat point, then prepare the scene for the entrance the
 * party came through; through the sixth entrance the first time, a burst
 * of light lifts the leader in. */
s32 MakyuriIriguchi_ApplyEntryState(void)
{
    struct EffectOptions options;
    s32 velocity[3];
    struct FieldActor *actor;
    u32 i;
    s32 angle;
    s32 flag;

    /* FAKEMATCH: the do/while and the held values keep the blend constants
     * in registers ahead of their address loads, as in MAKYURI_HEYA. */
    do {
        s32 blend = 0x3f40;
        *(volatile u16 *)0x04000050 = blend;
    } while (0);
    {
        s32 alpha = 0x1010;
        *(volatile u16 *)0x04000052 = alpha;
    }
    Makyuri_SpawnLightObjects(21, (s32)gSceneState);
    GameFlag_Set(0x111);
    gGameState.retreat_entrance = 11;
    gGameState.retreat_scene = (s32)&SceneId_MakyuriHeya4;
    BattleFx_StartFadeOverlay(0);
    if (GameFlag_IsSet(0x875))
        Engine_TaskAddCallback(Makyuri_CyclePalette, 0xc80);
    else
        Makyuri_ClearPalette();
    gEventWork->start_transition = 0x204;
    switch (gGameState.entrance) {
    case 1:
        if (!GameFlag_IsSet(0x872))
            Event_RequestExit(20);
    case 2:
        Actor_Get(12)->scale_x = -0x10000;
        Actor_Get(13)->scale_x = -0x10000;
        Actor_Get(14)->scale_x = -0x10000;
        Task_Wait(1);
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
            Map_Redraw();
            Task_Wait(1);
            Map_CopyCellAttributes(70, 32, 13, 7, 6, 32);
        }
        if (gGameState.entrance != 6)
            break;
        flag = GameFlag_IsSet(0x251);
        if (flag != 0)
            break;
        GameFlag_Set(0x251);
        Event_Begin();
        Camera_MoveTo(-1, -1, -1, 0);
        Map_Redraw();
        Task_Wait(1);
        Actor_Get(0)->y.fixed = 0x820000;
        ACTOR_LIFT(Actor_Get(0))->rise = 0x8000;
        ACTOR_LIFT(Actor_Get(0))->gravity = flag;
        Actor_Get(0)->motion_flags = flag;
        Event_OpenScreen();
        Event_WaitForScreen();
        Event_Wait(30);
        Actor_Get(0)->motion_flags = 3;
        Audio_PlayCue(204);
        Event_Wait(24);
        actor = Actor_Get(0);
        options.palette = 7;
        for (i = 0; i <= 16; i++) {
            angle = i << 12;
            velocity[0] = Math_Cos(angle);
            velocity[1] = 0;
            velocity[2] = Math_Sin(angle);
            velocity[0] -= velocity[0] / 4;
            velocity[2] -= velocity[2] / 2;
            Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, velocity[0], velocity[1],
                         velocity[2], EFFECT_USE_PALETTE | 1, &options);
        }
        Audio_PlayCue(188);
        Actor_SetAttachedEffect(0, 0x101);
        Object_SetModeById(0, 22);
        Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Engine_MapRenderWaitForValues();
        Actor_SetAttachedEffect(0, 0x100);
        ACTOR_LIFT(Actor_Get(0))->rise = 0x10000;
        ACTOR_LIFT(Actor_Get(0))->gravity = 0x4000;
        if (!GameFlag_IsSet(0x875)) {
            ColorBuffer_ApplySource(0x10000, 0);
            ColorBuffer_ApplyTarget(0x10003, 1);
            ColorBuffer_Interpolate(30);
            Event_WaitForScreen();
            Object_SetModeById(0, 1);
            Event_Wait(30);
            UiText_ShowCenteredMessage((s32)MsgMakyuriWhoHonorsHeart, 0, 0);
            ColorBuffer_ApplyTarget(0x10000, 0);
            ColorBuffer_Interpolate(30);
        }
        Event_End();
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
        Actor_SetSpriteFlags(Actor_Get(0), 0);
        Event_Begin();
        Map_Redraw();
        Task_Wait(1);
        gEventWork->start_transition = 0x100;
        Event_OpenScreen();
        Event_WaitForScreen();
        Event_Wait(120);
        Event_RequestExit(50);
        Event_End();
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

/* Makyuri entrance event: Ivan crosses the doorway twice while the guard's action callbacks alternate, then shows an emote and the cells are copied. */
/* Makyuri entrance event: an actor crosses the doorway twice while actor 8's action callbacks alternate, then shows an emote and the cells are copied. */
void MakyuriIriguchi_CrossDoorway(void)
{
    s32 i;
    s32 zero;
    s32 mask;
    u8 *p;
    s32 record;

    Engine_EventBegin();
    record = (s32)Object_GetById(12);
    *(s32 *)(record + 24) = -0x10000;
    record = (s32)Object_GetById(13);
    *(s32 *)(record + 24) = -0x10000;
    record = (s32)Object_GetById(14);
    *(s32 *)(record + 24) = -0x10000;
    Call3(Engine_ActorSetPosition, 3, 0x880000, 0xb80000);
    Call3(Engine_ActorSetPosition, 0, 0x880000, 0x1280000);
    Call3(Engine_ActorSetPosition, 8, 0x880000, 0x980000);
    Call3(ObjectMotion_SetSpeedParameters, 3, 0x18000, 0xc000);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0x18000, 0xc000);
    Call3(ObjectMotion_SetSpeedParameters, 0, 0xcccc, 0x6666);
    Engine_CameraSetSpeed(0xcccc, 0x1999);
    Engine_CameraMoveTo(0x880000, -1, 0xb80000, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_ActorRunRepeatedMotion(3, 1);
    *(u8 *)((s32)Object_GetById(8) + 90) &= 254;
    Battle_WaitMode0(20);
    mask = 254;
    zero = 0;
    for (i = 0; i < 2; i++) {
        Engine_ActorWalkTo(3, 152, 168);
        Battle_WaitMode0(10);
        Engine_ActorEnableActionCallback(8, (s32)MakyuriIriguchi_Actor8Path3);
        ObjectMotion_CommitCurrentPositionAndActivate(3);
        Call3(Engine_ActorFaceDirection, 3, 0xc000, 30);
        Engine_ActorRunRepeatedMotion(3, 1);
        p = (u8 *)Object_GetById(3);
        /* FAKEMATCH: or-ing a zero kept from before the loop leaves the
         * reference's unread zero in sl. */
        p[90] = (p[90] & mask) | zero;
        Engine_ActorWalkTo(3, 136, 184);
        Battle_WaitMode0(10);
        Engine_ActorEnableActionCallback(8, (s32)MakyuriIriguchi_Actor8Path1);
        ObjectMotion_CommitCurrentPositionAndActivate(3);
        *(u8 *)((s32)Object_GetById(3) + 90) |= 1;
        Battle_WaitMode0(30);
        Engine_ActorWalkTo(3, 120, 168);
        Battle_WaitMode0(5);
        Engine_ActorEnableActionCallback(8, (s32)MakyuriIriguchi_Actor8Path2);
        ObjectMotion_CommitCurrentPositionAndActivate(3);
        Engine_ActorFaceDirection(3, 0xc000, 30);
        Engine_ActorRunRepeatedMotion(3, 1);
        Battle_WaitMode0(15);
        *(u8 *)((s32)Object_GetById(3) + 90) &= mask;
        Engine_ActorWalkTo(3, 136, 184);
        Battle_WaitMode0(15);
        Engine_ActorEnableActionCallback(8, (s32)MakyuriIriguchi_Actor8Path1);
        ObjectMotion_CommitCurrentPositionAndActivate(3);
        Engine_ActorRunRepeatedMotion(3, 1);
        *(u8 *)((s32)Object_GetById(3) + 90) |= 1;
    }
    Battle_WaitMode0(20);
    Engine_ActorShowEmote(3, 0x102, 60);
    record = (s32)Object_GetById(3);
    *(s32 *)(record + 108) = (s32)SceneActor_FaceLeaderWhileGrounded;
    Engine_CameraMoveToActor(0, 1);
    Battle_WaitMode0(30);
    Call3(Engine_ActorWalkToAndWait, 0, 136, 0x108);
    Engine_CameraWaitForMove();
    Call6(Map_CopyCellAttributeRect, 0, 0, 3, 3, 7, 9);
    Engine_EventEnd();
}

void MakyuriIriguchi_RunDoorScene(void)
{
    s32 record;

    if (GameFlag_IsSet(0x250) == 0) {
        GameFlag_Set(0x250);
        Event_Begin();
        record = Actor_Get(12);
        *(s32 *)(record + 24) = -0x10000;
        record = Object_GetById(13);
        *(s32 *)(record + 24) = -0x10000;
        record = Actor_Get(14);
        *(s32 *)(record + 24) = -0x10000;
        Actor_SetPosition(ACTOR_MIA, 0x880000, 0x900000);
        Actor_FaceDirection(ACTOR_MIA, 0x4000, 10);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
        Event_OpenScreen();
        Event_WaitForScreen();
        Event_Wait(60);
        Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
        Actor_SetAnimationAndWait(ACTOR_MIA, 3);
        Event_Wait(30);
        Actor_WalkTo(ACTOR_MIA, 136, 72);
        Event_Wait(40);
        Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        Actor_WaitForMove(ACTOR_MIA);
        Actor_SetPosition(ACTOR_MIA, 0, 0);
        GameFlag_Set(0x872);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
        Event_End();
    }
}
