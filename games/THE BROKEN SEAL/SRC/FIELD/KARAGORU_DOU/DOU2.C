#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR_PAIR_SCENE.H"
#include "STAGED_ACTOR.H"
#include "FIELD_EFFECT.H"

extern u8 MsgKaragoruIveBeenWaitingForRobin[];
extern u8 MsgKaragoruDoYouWishCrossInto[];

struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

struct HeightTrackedObject {
    u8 pad00[12];
    s32 height;                 /* +12 */
};

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_SetPosition(actor, x, y);
}

extern u8 MsgKaragoruWarriorsHaveBeenFightingWhile[];
extern u8 MsgKaragoruWeMissedColossoBecauseWe[];

union SpinningActor {
    union FieldObject object;
    struct StagedActor staged;
};

void Map_WaitWorkValuesBelow256(void);

enum StagedPairMessage {
    MSG_WARRIORS_HAVE_BEEN_FIGHTING_WHILE = 0x23d2,
    MSG_WE_MISSED_COLOSSO_BECAUSE_WE = 0x23d5,
    MSG_IVE_BEEN_WAITING_FOR_ROBIN = 0x23d9,
    MSG_WHY_GOING_BACK_ROBIN_DO = 0x23da
};

extern u8 LinkedMessage_DoYouWishCrossInto[];

void ActorPresentation_RunActorElevenRecoveryScene(void)
{
    Event_Begin();
    Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 11, 0);
    Event_SetMessage((s32)MsgKaragoruIveBeenWaitingForRobin);
    Event_ShowMessage(11, 0);
    Actor_SetAnimation(11, 2);
    {
        s16 *position = Actor_Get(ACTOR_PARTY_LEADER);

        if (position != 0)
            Actor_SetDestination(11, position[5], position[9]);
    }
    Actor_WaitForMove(11);
    Actor_SetPosition(11, 0, 0);
    Event_Wait(20);
    GameFlag_Set(2464);
    Event_End();
}

void KaragoruDou_AskToCross(void)
{
    s32 base;

    base = (s32)MsgKaragoruDoYouWishCrossInto;
    Event_SetMessage(base);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        if (GameFlag_IsSet(0x950) != 0) {
            if (GameFlag_IsSet(0x96f) == 0) {
                Event_SetMessage((base + 8));
            }
        }
        Event_ShowMessage(8, 0);
    } else {
        bump_step(1);
        Event_ShowMessage(8, 0);
    }
}

void ActorPresentation_SelectActorNineScript(void)
{
    if (GameFlag_IsSet(2384) != 0 && GameFlag_IsSet(2415) == 0)
        Event_SetMessage((s32)MsgKaragoruWeMissedColossoBecauseWe);
    else
        Event_SetMessage((s32)MsgKaragoruWarriorsHaveBeenFightingWhile);
    Event_ShowMessage(9, 0);
}

void FieldScene_RunScene3be_02001080(void)
{
    extern u8 *Data_03001ebc;

    u8 *work;

    work = Data_03001ebc;
    Event_Begin();
    if (GameFlag_IsSet(0x204) != 0) {
        GameFlag_Clear(0x9a3);
        GameFlag_Clear(0x9a5);
        GameFlag_Clear(0x9a4);
        GameFlag_Clear(0x9a6);
        GameFlag_Set(0x9a5);
        GameFlag_Set(0x9a4);
    }
    Event_RequestExit(*(s16 *)(work + 0x16c));
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

void StagedActorPairScene_RunStep(void)
{
    Leader_CheckAhead();
}

void ActorPresentation_RunActorEightThresholdScene(void)
{
    Actor_Get(8);
    Event_Begin();
    {
        s32 *actor = Actor_Get(8);

        if ((actor[2] >> 20) <= 30) {
            StagedActorPairScene_RunSpinningLeap(8);
            {
                s32 x = 27;
                s32 y = 19;

                Map_CopyCellAttributes(29, 19, 1, 1, x, y);
            }
            GameFlag_Set(2466);
        }
    }
    Event_End();
}

void StagedActorPairScene_RunUpdate(void)
{
    StagedActor_AdvancePair();
    ActorPresentation_RunActorNineThresholdScene();
}

void ActorPresentation_RunActorNineThresholdScene(void)
{
    Event_Begin();
    if ((((s32 *)Object_GetById(9))[2] >> 20) > 42) {
        s32 x = 107;
        s32 y = 17;

        Map_CopyCellAttributes(108, 17, 1, 1, x, y);
        Event_Wait(8);
        Actor_SetPosition(9, 0, 0);
        PlaceActor(10, 45613056, 18874368);
        Actor_SetAnimation(10, 3);
        Audio_PlayCue(154);
        GameFlag_Set(2469);
    }
    Event_End();
}

void StagedActorPairScene_NoopActorCallback(void){}

void StagedActorPairScene_RotateActorPart(u8 *actor)
{
    u8 *sprite_part = *(u8 **)(actor + 80);

    *(u16 *)(sprite_part + 30) -= 0x400;
}

void StagedActorPairScene_WaitForHeight(struct HeightTrackedObject *object,
                                       s32 limit)
{
    extern u8 Data_03001ebc[];

    s32 frames = 40;

    while (frames != 0) {
        Task_Wait(1);
        frames--;
        if (object->height <= limit) {
            break;
        }
    }
}

void StagedActorPairScene_RunSpinningLeap(s32 id)
{
    union SpinningActor *work;
    u32 cnt;
    s32 velocity[3];
    s32 angle;

    work = (union SpinningActor *)Object_GetById(id);
    work->object.actor.motion_flags = 0;
    for (cnt = 0; cnt < 9; cnt++) {
        Engine_TaskWait(1);
        work->object.actor.sprite->rotation -= 0x100;
        work->object.actor.x.fixed -= Engine_MathCos(work->object.actor.sprite->rotation) / 2;
        work->staged.unknown_38 = 0x80000000;
    }
    work->object.actor.update = (void (*)(union FieldObject *))StagedActorPairScene_RotateActorPart;
    Engine_AudioPlayCue(136);
    Actor_SetSpeed(id, 0x20000, 0x10000);
    Actor_SetDestination(id, 472, 288);
    work->object.effect.velocity_y = 0xcccc;
    work->object.actor.motion_flags = 3;
    work->object.actor.unknown_22 = 0;
    Engine_ActorWaitForMove(id);
    StagedActorPairScene_WaitForHeight(work, 0x200000);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    for (cnt = 0; cnt < 17; cnt++) {
        angle = cnt << 12;
        velocity[0] = Engine_MathCos(angle);
        velocity[1] = 0;
        velocity[2] = Engine_MathSin(angle);
        velocity[0] -= velocity[0] / 4;
        velocity[2] -= velocity[2] / 2;
        Effect_Spawn(work->object.actor.x.fixed, work->object.actor.y.fixed,
                     work->object.actor.z.fixed, velocity[0], velocity[1], velocity[2], 0, 0);
    }
    Actor_SetDestination(id, 440, 308);
    Engine_ActorWaitForMove(id);
    StagedActorPairScene_WaitForHeight(work, 0x200000);
    work->object.actor.update = 0;
    work->object.actor.sprite->rotation = 0x1000;
    Engine_AudioPlayCue(154);
    Engine_ActorSetAnimation(id, 3);
    Map_WaitWorkValuesBelow256();
    Engine_EventWait(10);
    Engine_ActorSetAnimation(id, 2);
}

void StagedActorPairScene_NoopSceneCallback(void){}

void StagedActorPairScene_RunActorTwelveCommand(void)
{
    Actor_SetPosition(12, 0, 0);
}

/* The cave's scene start. Entering the first scene sets flag 0x144 and
   hides actor 11 while flag 0x9a0 is set. In the third, entrance 1 opens
   its passage, flags 0x9a2 and 0x9a5 move the actors their events have
   moved, and actor 12's sprite flags are cleared. */
s32 Scene_Initialize(void)
{
    struct FieldActor *actor;

    if (gGameState.scene == (s32)&SceneId_KaragoruDou1) {
        GameFlag_Set(0x144);
        if (GameFlag_IsSet(0x9a0) != 0) {
            Actor_SetPosition(11, 0, 0);
        }
    }
    if (gGameState.scene == (s32)&SceneId_KaragoruDou3) {
        if (gGameState.entrance == 1) {
            Map_CopyCellAttributes(108, 17, 1, 1, 107, 17);
        }
        if (GameFlag_IsSet(0x9a2) != 0) {
            Actor_SetPosition(8, 0x1b80000, 0x1340000);
            Actor_SetAnimation(8, 2);
            Map_CopyCellAttributes(29, 19, 1, 1, 27, 19);
        }
        if (GameFlag_IsSet(0x9a5) != 0) {
            Actor_SetPosition(9, 0, 0);
            Actor_SetPosition(10, 0x2b80000, 0x1200000);
            Actor_SetAnimation(10, 2);
        }
        actor = Actor_Get(12);
        Actor_SetSpriteFlags(actor, 0);
    }
    return 0;
}
