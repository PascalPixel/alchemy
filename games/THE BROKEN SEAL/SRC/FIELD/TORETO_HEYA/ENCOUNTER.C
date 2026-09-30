#include "HEYA.H"
#include "CALL.H"
extern u8 MsgToretoDoingNowsNot[];
extern u8 MsgToretoMmmmm[];
extern u8 MsgToretoTurnedPeopleKolima[];

void FieldScene_RunFourActorEncounter(void)
{
    u32 i;
    s32 rec;
    s32 record;
    s32 v6;
    s32 v5;
    s32 base5_200962d;
    s32 base5_2009ec8;

    rec = GameFlag_IsSet(3);
    *((u8 *)Engine_ActorGet(3) + 35) &= 254;
    Actor_SetSpritePriority(ACTOR_MIA, 2);
    *((u8 *)Engine_ActorGet(0) + 35) &= 254;
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    PartyInventory_FindOwner(184);
    Audio_PlayCue(17);
    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_MIA, 0xcccc, 0x6666);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xa60000, 0x500000);
    v6 = 192;
    record = Actor_Get(ACTOR_PARTY_LEADER);
    *(u16 *)(record + 6) = (v6 << 8);
    Actor_SetPosition(ACTOR_GERALD, 0x940000, 0x5a0000);
    record = Actor_Get(ACTOR_GERALD);
    *(u16 *)(record + 6) = (v6 << 8);
    Actor_SetPosition(ACTOR_IVAN, 0xb60000, 0x5a0000);
    record = Engine_ActorGet(ACTOR_IVAN);
    *(u16 *)(record + 6) = (v6 << 8);
    if (rec != 0) {
        Actor_SetPosition(ACTOR_MIA, 0xa60000, 0x680000);
        record = Engine_ActorGet(ACTOR_MIA);
        *(u16 *)(record + 6) = (v6 << 8);
    }
    ToretoHeya_PlayGesture(0);
    Task_Wait(10);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 48;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Camera_SetSpeed(0x13333, 0x2666);
    Camera_MoveTo(0xa80000, -1, 0x980000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    v5 = 10;
    Audio_PlayCue(123);
    Map_CopyCellAttributes(26, 3, 1, 2, v5, 8);
    Map_CopyCells(26, 38, 1, 1, v5, 43);
    Task_Wait(4);
    Map_CopyCells(26, 37, 1, 2, v5, 42);
    Task_Wait(4);
    Map_CopyCells(26, 36, 1, 3, v5, 41);
    Task_Wait(4);
    Map_CopyCells(26, 35, 1, 4, v5, 40);
    Task_Wait(80);
    Event_SetMessage((s32)MsgToretoMmmmm);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Camera_MoveTo(0xa80000, -1, 0x5a0000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    ToretoHeya_PlayGesture(1);
    Event_Wait(60);
    Audio_PlayCue(21);
    ToretoHeya_PlayGesture(4);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 80);
    Event_ShowMessage(0x8009, 0);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    ToretoHeya_PlayGesture(0);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 10);
    Event_ShowMessageAndWait(0x8001, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    Event_ShowMessageAndWait(0x8002, 0, 20);
    ToretoHeya_PlayGesture(0);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x8009, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, (v6 << 8), 0);
    ((void (*)())Engine_ActorFaceDirection)(1, (v6 << 8), 0);
    Actor_FaceDirection(ACTOR_IVAN, (v6 << 8), 40);
    ToretoHeya_PlayGesture(4);
    Event_OpenMessage(0x8009, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        Actor_ShowEmote(ACTOR_GERALD, 0x103, 20);
        Actor_SetAnimation(ACTOR_GERALD, 4);
        Event_SetMessage((s32)MsgToretoDoingNowsNot);
        Event_ShowMessage(0x8001, 0);
        Actor_ShowEmote(ACTOR_IVAN, 0x103, 10);
        Actor_SetAnimation(ACTOR_IVAN, 3);
        Event_ShowMessage(0x8002, 0);
    }
    Event_Wait(20);
    ToretoHeya_PlayGesture(4);
    Event_SetMessage((s32)MsgToretoTurnedPeopleKolima);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Event_ShowMessageAndWait(0x8009, 0, 10);
    ToretoHeya_PlayGesture(0);
    Event_Wait(20);
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(20);
    Task_Wait(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Actor_FaceDirection(ACTOR_GERALD, (v6 << 8), 0);
    Actor_FaceDirection(ACTOR_IVAN, (v6 << 8), 20);
    Event_Wait(20);
    *(s32 *)ToretoHeya_SparkCounter = 0;
    {
        s32 *bank = (s32 *)ToretoHeya_SparkOrigin;
        bank[0] = 0xa80000;
        bank[1] = 0x200000;
        base5_200962d = (s32)ToretoHeya_SpawnSwirlSparks;
        bank[2] = 0x340000;
    }
    Call2(Engine_TaskAddCallback, base5_200962d, 0xc80);
    Event_Wait(220);
    Engine_TaskRemoveCallback(base5_200962d);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(20);
    Task_Wait(40);
    ToretoHeya_PlayGesture(4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x8009, 0, 10);
    ToretoHeya_PlayGesture(0);
    Event_ShowMessage(0x8009, 0);
    Object_SetActionCallbackAndRefreshById(8, (s32)ToretoHeya_ActionTable1);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
    Event_ShowMessage(0x8001, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 10);
    Event_ShowMessage(0x8002, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_ShowMessageAndWait(0x8001, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_ShowMessageAndWait(0x8002, 0, 10);
    if (rec != 0) {
        Actor_RunRepeatedMotion(ACTOR_MIA, 1);
        Engine_EventShowMessageAndWait(0x8003, 0, 10);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    base5_2009ec8 = (s32)ToretoHeya_ActionTable2;
    Actor_EnableActionCallback(ACTOR_GERALD, base5_2009ec8);
    if (rec != 0) {
        Actor_EnableActionCallback(ACTOR_MIA, base5_2009ec8);
    }
    Object_SetActionCallbackAndRefreshById(2, base5_2009ec8);
    Event_Wait(20);
    *((u8 *)Engine_ActorGet(0) + 35) |= 1;
    GameFlag_Set(0x844);
    Engine_TaskAddCallback((s32)ToretoPalette_ApplyTint, 0xc80);
    Event_End();
}

void SceneState_ApplyRectsByFlag844(s32 flag)
{
    if (flag != 0 && GameFlag_IsSet(0x109) == 0)
        FieldScene_RunFourActorEncounter();

    Task_Wait(1);
    if (GameFlag_IsSet(0x844) != 0) {
        s32 w1 = 10;
        Map_CopyCells(121, 34, 3, 1, 93, w1);
        {
            s32 w2 = 30;
            Map_CopyCells(46, 38, 1, 1, w2, 43);
            Map_CopyCellAttributes(0, 0, 1, 2, w2, 9);
        }
        Map_CopyCellAttributes(26, 3, 1, 2, w1, 8);
        Map_CopyCells(26, 35, 1, 4, w1, 40);
    } else {
        s32 w1 = 10;
        s32 w2 = 8;
        Map_CopyCellAttributes(11, 8, 1, 2, w1, w2);
    }
}
