#include "KYUDEN.H"
extern u8 MsgBiribinoHumblyThank[];
extern u8 MsgBiribinoNaeYehDinnaeNeedTae[];
extern u8 MsgBiribinoNameSorryRejected[];
extern u8 MsgBiribinoWasButWorriedYehMight[];
extern u8 MsgBiribinoWeveBroughtWarriorsMilord[];

void FieldScene_RunPalaceGreeting(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
    Call3(Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x100, 0x294);
    Engine_EventWait(20);
    Camera_MoveTo(-1, -1, -1, 0);
    Call1(Engine_GameFlagSet, 0x200);
    Audio_PlayCue(188);
    Map_ClearLayerEntryFlag(1);
    Map_ClearLayerEntryFlag(2);
    Call3(Engine_ActorSetPosition, 19, 0x1000000, 0x2780000);
    Engine_TaskWait(1);
    Call3(Engine_ActorSetSpeed, 19, 0x9999, 0x4ccc);
    Call3(Engine_ActorWalkToAndWait, 19, 0x100, 0x284);
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Event_Wait(20);
    Actor_RunRepeatedMotion(19, 2);
    Call1(Engine_EventSetMessage, (s32)MsgBiribinoNameSorryRejected);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Call3(Engine_ActorShowEmote, 0, 0x100, 40);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x108, 0x294);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Call3(Engine_ActorWalkToAndWait, 19, 248, 0x294);
    Call3(Engine_ActorFaceDirection, 19, 0x1000, 40);
    Engine_ActorSetAnimationAndWait(19, 4);
    Engine_EventShowMessage(19, 0);
    Engine_ActorSetAnimationAndWait(19, 3);
    Value2(Engine_EventAskYesNo, 19, 0);
    Actor_RunRepeatedMotion(19, 2);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Call3(Engine_ActorShowEmote, 0, 0x101, 60);
    Actor_SetAttachedEffect(19, 0x102);
    Event_Wait(60);
    Engine_ActorRunRepeatedMotion(19, 1);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Engine_ActorSetAnimationAndWait(19, 3);
    Event_ShowMessage(19, 0);
    Actor_SetSpeed(19, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 19, 248, 0x304);
    Engine_ActorSetPosition(19, 0, 0);
    Call1(Engine_GameFlagClear, 0x12f);
    Call1(Engine_GameFlagSet, 0x84f);
    Engine_EventEnd();
}

void ConfigurePrimarySceneChannels(void)
{
    FaceActor(1, 0xe000, 0);
    FaceActor(2, 0xa000, 0);
    FaceActor(3, 0x8000, 0);
}

void ConfigureSecondarySceneChannels(void)
{
    FaceActor(1, 0xc000, 0);
    FaceActor(2, 0xc000, 0);
    FaceActor(3, 0xa000, 0);
}

/* FAKEMATCH: The cleared motion byte keeps its own zero local so the
 * following call reloads its separate zero argument. */
void RunEventScript02(void)
{
    u8 *buf;
    struct FieldActor *actor;
    s32 flag;
    u8 clear = 0;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    buf = (u8 *)Engine_EventGetViewCenter();
    buf[85] = clear;
    Camera_MoveTo(0x037e0000, -1, 0x02980000, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Map_Redraw();
    Task_Wait(1);

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    gEventWork->transition_frames = 16;

    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_SetPosition(19, 0x03780000, 0x031e0000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x03880000, 0x031e0000);
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0x037e0000, -1, 0x02ba0000, 1);
    Actor_SetSpeed(19, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Actor_WalkTo(19, 888, 720);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 904, 736);
    Event_Wait(60);
    Actor_WaitForMove(19);
    Actor_SetAnimation(19, 1);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(20);
    Actor_RunRepeatedMotion(19, 2);
    Event_SetMessage((s32)MsgBiribinoWeveBroughtWarriorsMilord);

    flag = 1;
    if (GameFlag_IsSet(0x84f) == 0) {
        AdvanceMessage(1);
        flag = 0;
    }
    Event_ShowMessage(19, 0);
    if (flag != 0) {
        AdvanceMessage(1);
    }

    Camera_MoveTo(0x037e0000, -1, 0x02980000, 1);
    Actor_EnableActionCallback(19, Kyuden_PacingActions);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);

    actor = (struct FieldActor *)Engine_ActorGet(0);
    if (actor != 0) {
        Actor_SetPosition(ACTOR_GERALD, actor->x.fixed, actor->z.fixed);
    }
    actor = (struct FieldActor *)Engine_ActorGet(0);
    if (actor != 0) {
        Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
    }
    actor = (struct FieldActor *)Engine_ActorGet(0);
    if (actor != 0) {
        Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
    }

    Actor_SetSpeed(ACTOR_GERALD, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_IVAN, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    Actor_SetAnimation(ACTOR_IVAN, 2);
    Actor_SetAnimation(ACTOR_MIA, 2);
    Actor_SetDestinationOffset(ACTOR_GERALD, -16, 16);
    Actor_SetDestinationOffset(ACTOR_IVAN, 16, 16);
    Actor_SetDestinationOffset(ACTOR_MIA, 32, 16);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_SetAnimation(ACTOR_MIA, 1);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 0);
    Object_RefreshSelectorById(19);
    Event_Wait(20);

    flag = 1;
    if (GameFlag_IsSet(0x84f) == 0) {
        AdvanceMessage(1);
        flag = 0;
    }
    Actor_RunRepeatedMotion(18, 3);
    Event_ShowMessageAndWait(0x2012, 0, 20);
    if (flag != 0) {
        AdvanceMessage(1);
    }

    flag = 1;
    if (GameFlag_IsSet(0x84f) == 0) {
        AdvanceMessage(1);
        flag = 0;
    }
    Actor_RunRepeatedMotion(18, 1);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    if (flag != 0) {
        AdvanceMessage(1);
    }

    ConfigurePrimarySceneChannels();
    Event_Wait(20);
    if (GameFlag_IsSet(0x84f) != 0) {
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
        Actor_ShowEmote(ACTOR_GERALD, 261, 40);
    } else {
        Event_Wait(40);
    }

    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 10);
    Event_ShowMessageAndWait(0x4001, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_ShowMessage(0x4002, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 10);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_ShowMessageAndWait(0x4003, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x2012, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 259, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);

    if (GameFlag_IsSet(0x84f) != 0) {
        Actor_RunRepeatedMotion(18, 1);
        Actor_SetAnimationAndWait(18, 4);
        Event_OpenMessage(0x2012, 0);
        ConfigurePrimarySceneChannels();
        flag = 1;
        if (Event_ChooseYesNo(0, 0) != 0) {
            AdvanceMessage(1);
            flag = 0;
        }
        Actor_FaceDirection(18, 0x5000, 0);
        ConfigureSecondarySceneChannels();
        Event_Wait(10);
        Event_ShowMessageAndWait(0x2012, 0, 10);
        if (flag != 0) {
            AdvanceMessage(1);
        }
        Actor_ShowEmote(18, 258, 60);
    } else {
        AdvanceMessage(4);
    }

    Event_OpenMessage(0x2012, 0);
    ConfigurePrimarySceneChannels();
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage((s32)MsgBiribinoHumblyThank);
    } else {
        Event_SetMessage((s32)MsgBiribinoNaeYehDinnaeNeedTae);
    }

    ConfigureSecondarySceneChannels();
    Event_ShowMessageAndWait(0x2012, 0, 20);
    Actor_RunRepeatedMotion(19, 1);
    Event_SetMessage((s32)MsgBiribinoWasButWorriedYehMight);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 40);
    Actor_RunRepeatedMotion(18, 2);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    ConfigureSecondarySceneChannels();
    Event_Wait(10);
    Actor_ShowEmote(18, 261, 60);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 3);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_ShowEmote(18, 264, 60);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_FaceDirection(18, 0x3000, 10);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_SetAnimationAndWait(18, 3);
    Event_OpenMessage(0x2012, 0);

    ConfigurePrimarySceneChannels();
    flag = 1;
    if (Event_ChooseYesNo(0, 0) == 1) {
        AdvanceMessage(1);
        flag = 0;
    }
    ConfigureSecondarySceneChannels();
    Event_ShowMessageAndWait(0x2012, 0, 10);
    if (flag != 0) {
        AdvanceMessage(1);
    }

    Actor_FaceDirection(18, 0x7000, 10);
    Actor_RunRepeatedMotion(19, 1);
    Actor_FaceDirection(19, 0x1000, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(19, 3);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(19, 0x3000, 10);
    Actor_FaceDirection(18, 0x3000, 20);
    Actor_RunRepeatedMotion(18, 1);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_SetAnimationAndWait(18, 3);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);

    Actor_SetAnimation(ACTOR_GERALD, 2);
    actor = (struct FieldActor *)Engine_ActorGet(0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_GERALD, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_SetAnimation(ACTOR_IVAN, 2);
    actor = (struct FieldActor *)Engine_ActorGet(0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_IVAN, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_SetAnimation(ACTOR_MIA, 2);
    actor = (struct FieldActor *)Engine_ActorGet(0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_MIA, actor->x.part.pixel, actor->z.part.pixel);
    }

    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Event_Wait(20);
    Actor_FaceDirection(18, 0x5000, 0);
    Call3(Object_SetTargetAndCallback, 0, 0x10013, (s32)Kyuden_FacingActions);
    Actor_WalkToAndWait(19, 852, 646);
    Actor_WalkToAndWait(19, 852, 666);
    Actor_WalkToAndWait(19, 864, 672);
    Actor_FaceDirection(19, 0x1000, 10);
    Actor_RunRepeatedMotion(19, 1);
    Event_Wait(10);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_WalkToAndWait(19, 886, 708);
    Actor_WalkTo(19, 894, 764);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 764);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(802);
    if (GameFlag_IsSet(0x84f) == 0) {
        GameFlag_Set(0x84f);
        GameFlag_Set(0x84a);
    }
    Event_RequestExit(6);
    Event_End();
}
