#include "TIMED_EVENTS.H"
#include "CALL.H"
extern u8 MsgHaidiaAWiseManFleesWhen[];
extern u8 MsgHaidiaGoodArmorDrawsOutStrength[];
extern u8 MsgHaidiaGoodWeaponsDrawOutStrength[];
extern u8 MsgHaidiaHey2[];
extern u8 MsgHaidiaImNotSadJustGo[];
extern u8 MsgHaidiaTheRumorWasTrue[];
extern u8 MsgHaidiaValeFeelsEmpty[];
extern u8 MsgHaidiaWhenDidYouComeBack[];
extern u8 MsgHaidiaYouCameBack[];
extern u8 MsgHaidiaYoureLeavingAgainSoon[];

void SceneState_SetRuntimeWord448To521AndRun(s32 value)
{
    if (GameFlag_IsSet(0x834) != 0)
        BattleFx_SetBlock30ValuesMaxZero();
    Audio_PlayCue(123);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    gEventWork->transition_frames = 16;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(value);
}

/* Eight numbered-scene wrappers follow, each a twelve-byte owner. */
void FieldScene_RunIndexedStep1(void)
{
    SceneState_SetRuntimeWord448To521AndRun(1);
}

void FieldScene_RunIndexedStep2(void)
{
    SceneState_SetRuntimeWord448To521AndRun(2);
}

void FieldScene_RunIndexedStep3(void)
{
    SceneState_SetRuntimeWord448To521AndRun(3);
}

void FieldScene_RunIndexedStep4(void)
{
    SceneState_SetRuntimeWord448To521AndRun(4);
}

void FieldScene_RunIndexedStep5(void)
{
    SceneState_SetRuntimeWord448To521AndRun(5);
}

void FieldScene_RunIndexedStep6(void)
{
    SceneState_SetRuntimeWord448To521AndRun(6);
}

void FieldScene_RunIndexedStep7(void)
{
    SceneState_SetRuntimeWord448To521AndRun(7);
}

void FieldScene_RunIndexedStep8(void)
{
    SceneState_SetRuntimeWord448To521AndRun(8);
}

struct FlashCueWork {
    u8 unknown_0000[0x1f84];
    s16 alternate_cue;
};

s32 Scene_RunSupplementalSequenceOne(void)
{
    extern u8 *gWork;

    u8 *record;
    u8 **scene = (u8 **)&gWork;

    *(s32 *)(scene[0] + 0x1c0) = 0x209;
    if (GameFlag_IsSet(0x834) != 0) {
        Actor_SetPosition(8, 0, 0);
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(10, 0, 0);
        Actor_SetPosition(11, 0, 0);
        Actor_SetPosition(12, 0, 0);
        Actor_SetPosition(13, 0, 0);
        Actor_SetPosition(14, 0, 0);
        Actor_SetPosition(15, 0, 0);
        ((void (*)())Engine_ActorSetPosition)(16, 0, 0);
        Actor_SetPosition(17, 0, 0);
        Actor_SetPosition(18, 0, 0);
        Actor_SetPosition(19, 0, 0);
        Actor_SetPosition(20, 0, 0);
        Actor_SetPosition(21, 0, 0);
        Actor_SetPosition(22, 0, 0);
        BattleFx_StartTwelveFrameBlend();
        ((struct FlashCueWork *)scene[3])->alternate_cue = 1;
        BattleFx_SetBlock30Values12Zero();
        Task_Wait(30);
        Event_OpenScreen();
        Event_WaitForScreen();
        BattleFx_SetBlock30Values128One();
    }
    if (GameFlag_IsSet(0x87a) != 0) {
        if (gGameState.entrance == 6) {
            if (GameFlag_IsSet(0x81d) == 0) {
                FieldScene_RunLongPresentationSequence();
            }
        }
        Actor_Get(10)->collision_flags |= 0x80;
    }
    if (gGameState.entrance == 2) {
        if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
            Actor_SetPosition(13, 0x1c60000, 0x960000);
            record = Actor_Get(13);
            Actor_SetSpriteFlags((s32)record, 0);
            Actor_SetAnimation(13, 5);
            Map_ClearLayerEntryFlag(4);
        }
    }
    return 0;
}

void FieldScene_RunByActorDirectionAndFlags(void)
{

    u8 *p;
    u32 dir;

    p = Actor_Get(ACTOR_PARTY_LEADER);
    dir = *(u16 *)(p + 6);
    dir += 0xffff5fff;

    if (dir <= 0x3ffe) {
        Shop_Open(1, 21);
        return;
    }

    Event_Begin();
    if (GameFlag_IsSet(0x87a) != 0) {
        Event_SetMessage((s32)MsgHaidiaYouCameBack);
        Event_AskYesNo(21, 0);
    } else {
        if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
            Event_SetMessage((s32)MsgHaidiaValeFeelsEmpty);
        } else {
            Event_SetMessage((s32)MsgHaidiaGoodWeaponsDrawOutStrength);
        }
        Event_ShowMessage(21, 0);
    }
    Event_End();
}

void FieldScene_RunScene376_0200055c(void)
{
    struct FieldActor *actor;

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if ((u32)(actor->facing - 0xa001) <= 0x3ffe) {
        Shop_Open(2, 22);
    } else {
        ((void (*)())Engine_EventBegin)();
        if (GameFlag_IsSet(0x87a) != 0) {
            Event_SetMessage((s32)MsgHaidiaTheRumorWasTrue);
        } else {
            if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
                Event_SetMessage((s32)MsgHaidiaImNotSadJustGo);
            } else {
                Event_SetMessage((s32)MsgHaidiaGoodArmorDrawsOutStrength);
            }
        }
        Event_ShowMessage(22, 0);
        Event_End();
    }
}

void FieldScene_RunScene376_020005d4(void)
{
    struct FieldActor *actor;

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if ((u32)(actor->facing - 0xa001) <= 0x3ffe) {
        Shop_Open(3, 20);
    } else {
        if (GameFlag_IsSet(0x87a) != 0) {
            Event_Begin();
            Event_SetMessage((s32)MsgHaidiaWhenDidYouComeBack);
            Event_ShowMessage(20, 0);
            Event_End();
        } else {
            if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
                Scene_GiveFarewellHerb();
            } else {
                Event_Begin();
                Event_SetMessage((s32)MsgHaidiaAWiseManFleesWhen);
                Event_ShowMessage(20, 0);
                Event_End();
            }
        }
    }
}

void FieldScene_RunLongPresentationSequence(void)
{
    void Camera_MoveTo();

    u32 i;
    s32 record;
    struct FieldActor *actor;
    s32 v6;
    s32 base7_20090c1;
    s32 base5_20092fc;
    s32 base5_2009400;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetSpritePriority(ACTOR_MIA, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Actor_SetSpeed(ACTOR_GERALD, 0x6666, 0x3333);
    Actor_SetSpeed(ACTOR_IVAN, 0x6666, 0x3333);
    Actor_SetSpeed(ACTOR_MIA, 0x6666, 0x3333);
    Actor_SetAnimation(8, 5);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x328, 0x1fc);
    record = Actor_Get(23);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(24);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(25);
    Actor_SetSpriteFlags(record, 0);
    v6 = 0;
    *((u8 *)Engine_ActorGet(23) + 85) = v6;
    *((u8 *)Engine_ActorGet(24) + 85) = v6;
    *((u8 *)Engine_ActorGet(25) + 85) = v6;
    base7_20090c1 = (s32)Scene_UpdateTimedActor;
    ((void (*)())Engine_TaskAddCallback)(base7_20090c1, 0xc80);
    Task_Wait(1);
    gEventWork->transition_frames = 32;
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
        Actor_SetPosition(ACTOR_GERALD, actor->x.fixed, actor->z.fixed);
    }
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
        Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
    }
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
        Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
    }
    Actor_WalkTo(ACTOR_GERALD, 0x318, 0x200);
    Actor_WalkTo(ACTOR_IVAN, 0x338, 0x1f8);
    Actor_WalkToAndWait(ACTOR_MIA, 0x332, 0x20c);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Event_Wait(10);
    base5_20092fc = (s32)gValeFaceTargetScript;
    Call3(Object_SetTargetAndCallback, 0, 0x1000a, base5_20092fc);
    Call3(Object_SetTargetAndCallback, 1, 0x1000a, base5_20092fc);
    Call3(Object_SetTargetAndCallback, 2, 0x1000a, base5_20092fc);
    Object_SetTargetAndCallback(3, 0x1000a, base5_20092fc);
    Event_Wait(0x12c);
    *(u8 *)(Battle_GetWorkObject1e0() + 85) = v6;
    Camera_SetSpeed(0x1999, 0x333);
    Camera_MoveTo(0x3120000, 0, 0x1ae0000, 1);
    Event_Wait(240);
    Actor_Stop(10);
    Actor_ShowEmote(10, 0x102, 80);
    Actor_WalkToAndWait(10, 0x333, 0x195);
    Event_Wait(40);
    Actor_SetAnimationAndWait(10, 4);
    Event_Wait(40);
    Actor_FaceDirection(10, 0xd000, 20);
    Event_SetMessage((s32)MsgHaidiaHey2);
    Event_ShowMessageAndWait(0x900a, 0, 20);
    Actor_Stop(ACTOR_PARTY_LEADER);
    Actor_Stop(ACTOR_GERALD);
    Actor_Stop(ACTOR_IVAN);
    Actor_Stop(ACTOR_MIA);
    Actor_ShowEmote(11, 0x100, 40);
    Event_ShowMessageAndWait(0x200b, 0, 20);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x900a, 0, 10);
    Actor_FaceDirection(11, 0x5000, 10);
    Event_ShowMessageAndWait(0x200b, 0, 40);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x900a, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 80);
    Actor_ShowEmote(11, 0x106, 40);
    Event_ShowMessageAndWait(0x200b, 0, 40);
    Actor_StartRepeatedMotion(10, 2);
    Actor_ShowEmote(10, 0x102, 20);
    Actor_SetAnimation(10, 4);
    Event_ShowMessageAndWait(0x900a, 0, 10);
    Actor_StartRepeatedMotion(11, 1);
    Actor_SetAnimationAndWait(11, 3);
    Event_Wait(20);
    Actor_StartRepeatedMotion(10, 1);
    Actor_SetAnimationAndWait(10, 4);
    Actor_StartRepeatedMotion(11, 1);
    Actor_SetAnimationAndWait(11, 3);
    Actor_StartRepeatedMotion(10, 1);
    Actor_SetAnimationAndWait(10, 4);
    Actor_ShowEmote(9, 0x105, 0);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    Actor_FaceDirection(9, 0x1000, 40);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(60);
    Actor_RunRepeatedMotion(9, 3);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x4009, 0, 40);
    Actor_SetAnimation(11, 0);
    Actor_RunRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(0x200b, 0, 10);
    Actor_SetAnimationAndWait(9, 4);
    Actor_RunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Actor_ShowEmote(10, 0x100, 20);
    Actor_FaceDirection(10, 0x5000, 40);
    Actor_SetAnimationAndWait(10, 3);
    Event_ShowMessageAndWait(0x400a, 0, 10);
    Actor_SetAnimationAndWait(9, 4);
    Actor_FaceDirection(9, 0xd000, 10);
    Actor_Jump(9, 2, 0);
    Actor_SetAnimation(9, 4);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Actor_ShowEmote(11, 0x101, 0);
    Actor_ShowEmote(10, 0x101, 40);
    Actor_FaceDirection(10, 0xd000, 80);
    Actor_FaceDirection(10, 0x5000, 60);
    Actor_StartRepeatedMotion(10, 2);
    Actor_StartRepeatedMotion(11, 2);
    Call11(Engine_EventShowTwoMessagesAndWait, 10, 11, 6, 6, 6, 11, 12, 1, 7, 1, v6);
    Event_Wait(20);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x3090000, 0, 0x1d40000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_ShowMessageAndWait(0x1001, 0, 20);
    Actor_RunRepeatedMotion(8, 2);
    Engine_TaskRemoveCallback((void (*)(void))base7_20090c1);
    Event_Wait(40);
    Actor_SetAnimationAndWait(8, 6);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Camera_MoveTo(0x2ee0000, 0, 0x1c30000, 1);
    Event_Wait(20);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_FaceDirection(10, 0x5000, 10);
    Actor_FaceDirection(8, 0x1000, 40);
    Actor_ShowEmote(8, 0x100, 40);
    Actor_FaceDirection(8, 0x3000, 20);
    Actor_FaceDirection(8, 0x1000, 20);
    Actor_FaceDirection(8, 0x3000, 40);
    Actor_SetAnimationAndWait(8, 6);
    Event_Wait(60);
    Actor_Jump(8, 6, 0);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x315, 0x1d9);
    Actor_FaceDirection(ACTOR_GERALD, 0x7000, 20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_ShowMessageAndWait(0x4001, 0, 10);
    Actor_FaceDirection(8, 0x1000, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_OpenMessage(0x4008, 0);
    Actor_FaceDirection(10, 0x5000, 0);
    Actor_FaceDirection(9, 0x1000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x7000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xb000, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Camera_MoveTo(0x3090000, 0, 0x1ac0000, 1);
    ((void (*)())Engine_EventWait)(20);
    Actor_RunRepeatedMotion(10, 2);
    Event_ShowMessage(10, 0);
    Actor_SetAnimationAndWait(11, 4);
    Event_Wait(20);
    Event_SetMessage((s32)MsgHaidiaYoureLeavingAgainSoon);
    Event_ShowMessage(0x200b, 0);
    Camera_MoveTo(0x3090000, 0, 0x1d40000, 1);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(9, 4);
    Actor_FaceDirection(9, 0xd000, 10);
    Event_ShowMessage(0x4009, 0);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessage(0x4008, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x7000, 10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Actor_FaceDirection(9, 0x1000, 10);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 80);
    Actor_FaceDirection(ACTOR_GERALD, 0x7000, 20);
    Event_ShowMessageAndWait(0x4001, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x1000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x333, 0x1e9);
    Actor_FaceDirection(ACTOR_IVAN, 0xb000, 40);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimationAndWait(9, 3);
    *((u8 *)Engine_ActorGet(3) + 35) &= 254;
    Actor_SetSpritePriority(ACTOR_MIA, 1);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_MIA, 0x31a, 0x208);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_WalkToAndWait(ACTOR_MIA, 0x310, 0x1f0);
    Actor_FaceDirection(ACTOR_MIA, 0x9000, 10);
    *((u8 *)Engine_ActorGet(3) + 35) |= 1;
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(20);
    Camera_MoveTo(0x3090000, 0, 0x1ac0000, 1);
    Event_Wait(20);
    Actor_SetSpeed(11, 0x6666, 0x3333);
    Actor_WalkToAndWait(11, 0x343, 0x184);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_ShowEmote(11, 0x108, 40);
    Event_ShowMessageAndWait(0x200b, 0, 20);
    Camera_MoveTo(0x3090000, 0, 0x1d40000, 1);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_IVAN, 0x7000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xf000, 40);
    Actor_FaceDirection(ACTOR_IVAN, 0x9000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xd000, 20);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(10, 1);
    Event_Wait(20);
    Actor_SetAnimation(10, 3);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xb000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xd000, 40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    base5_2009400 = (s32)gValePartyScript;
    Actor_EnableActionCallback(ACTOR_GERALD, base5_2009400);
    Engine_ActorEnableActionCallback(2, base5_2009400);
    Object_SetActionCallbackAndRefreshById(3, base5_2009400);
    Engine_ActorEnableActionCallback(10, (s32)gValeActor10Script);
    Actor_WalkToAndWait(11, 0x345, 0x178);
    Actor_FaceDirection(11, 0xd000, 20);
    GameFlag_Set(0x81d);
    Event_End();
}

void Scene_UpdateTimedActor(void)
{
    s32 no;
    u32 phase;
    union SceneActor *actor;
    s32 *other;

    phase = gFrameCount % 180;
    no = 23;
    switch (phase) {
    case 10:
        break;
    case 20:
        no = 24;
        break;
    case 30:
        no = 25;
        break;
    default:
        return;
    }
    actor = Actor_Get(no);
    if (actor == NULL) {
        return;
    }
    other = Actor_Get(8);
    if (other != NULL) {
        Actor_SetPosition(no, other[2], other[4]);
    }
    actor->words[6] = 0x6666;
    actor->words[7] = 0x6666;
    {
        s32 y = actor->words[3] + 0x180000;
        union SceneField *dst = (union SceneField *)(actor->halfwords + 50);
        s32 value;
        actor->words[3] = y;
        actor->words[15] = y;
        value = 25;
        dst->value = value;
        dst++;
        value = 128;
        dst->value = value;
    }
    Actor_EnableActionCallback(no, gValeTimedActorScript);
}
