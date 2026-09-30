#include "STATUS.H"
extern u8 MsgKorashiamuPrepareYourselvesContestantsFinalsWill[];
extern u8 MsgKorashiamuRobinOnlyOneEnteringFinals[];
extern u8 MsgKorashiamuSeeYouveMadeThroughYour[];
extern u8 MsgKorashiamuWonBothMatches[];

/*
 * The case order is load-bearing and it is not the selector order: the arms
 * are laid out in the order the reference places their bodies, and the jump
 * table stores their absolute addresses. Each callee name is keyed to the
 * address of the instruction that calls it, so an arm moved out of place
 * emits the wrong call word.
 */
void FieldScene_DispatchBySelector(void)
{
    s32 no;

    Task_Wait(1);
    no = gGameState.entrance;
    switch (no) {
    case 5:
        Actor_SetAnimation(8, 2);
        Actor_SetAnimation(9, 2);
        break;
    case 69:
        Actor_SetAnimation(8, 2);
        Actor_SetAnimation(9, 2);
        if (GameFlag_IsSet(0x109) != 0)
            break;
        SceneState_SetRuntimeWord448To513();
        break;
    case 7:
        FieldScene_BuildActorPresentationGroup();
        break;
    case 70:
        FieldScene_RunMiddleSequence();
        break;
    case 64:
        FieldScene_BuildActorPresentationSequence();
        InventorySnapshot_Take();
        break;
    case 65:
        FieldScene_RunScene3b9_020023e0();
        break;
    case 66:
        FieldScene_RunScene3b9_020025f0();
        break;
    case 12:
        GameFlag_Set(324);
        ActorPresentation_SetActorsTwelveToEighteen();
        if (GameFlag_IsSet(0x109) != 0)
            break;
        FieldScene_RunScene3b9_020024d8();
        break;
    case 21:
        Party_AddActiveOwner(1);
        Party_AddActiveOwner(2);
        Party_AddActiveOwner(3);
        GameFlag_Set(0x90e);
        FieldScene_RunScene3b9_02002668();
        break;
    case 67:
        FieldScene_RunScene3b9_02002820();
        break;
    case 68:
        FieldScene_RunScene3b9_02002904();
        break;
    case 31:
        Party_AddActiveOwner(1);
        Party_AddActiveOwner(2);
        Party_AddActiveOwner(3);
        GameFlag_Set(0x90f);
        FieldScene_RunScene3b9_02002964();
        break;
    default:
        break;
    }
}

void FieldScene_CallPairWith10(s32 no)
{
    Event_ShowMessage(no, 0);
    Event_Wait(10);
}

void SceneState_ForwardMaskedHalfwordWith10(s32 arg0, s32 arg1)
{
    Actor_FaceDirection(arg0, (u16)arg1, 10);
}

/* Two alternative layouts. */
void SceneState_ApplyRectsByFlatla384And962(void)
{
    s32 pair;
    s32 a, b;

    if (GameFlag_IsSet(2384) != 0) {
        pair = 2;
        Map_CopyCellsTo(64, 0, 48, 5, pair, pair);
        a = 16;
        b = 8;
        Map_CopyCellAttributes(14, 8, 2, 1, a, b);
    } else {
        Actor_SetChildValue(16, 2);
        if (GameFlag_IsSet(0x962) != 0) {
            a = 14;
            b = 11;
            Map_CopyCellAttributes(30, 22, 1, 2, a, b);
        }
    }
}

void FieldScene_BuildActorPresentationSequence(void)
{
    s32 flag;

    Event_Begin();
    Actor_SetPosition(ACTOR_GERALD, 0x3180000, 0x880000);
    Actor_SetPosition(ACTOR_IVAN, 0x3380000, 0x880000);
    Actor_SetPosition(ACTOR_MIA, 0x3280000, 0x980000);
    Task_Wait(1);
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_ColorBufferApplySource(0, 0);
    ColorBuffer_ApplyTarget(0, 0);
    ColorBuffer_Interpolate(1);
    Task_Wait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
    gEventWork->transition_frames = 1;
    Event_OpenScreen();
    Event_WaitForScreen();
    Engine_ColorBufferApplySource(0, 0);
    ColorBuffer_ApplyTarget(0x10002, 0);
    ColorBuffer_Interpolate(40);
    Event_Wait(80);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(40);
    SceneState_ForwardMaskedHalfwordWith10(8, 0x3000);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(40);
    Event_Wait(80);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(20);
    Event_SetMessage((s32)MsgKorashiamuRobinOnlyOneEnteringFinals);
    FieldScene_CallPairWith10(2);
    Actor_SetAnimationAndWait(8, 3);
    FieldScene_CallPairWith10(8);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 40);
    FieldScene_CallPairWith10(3);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_FaceDirection(9, 0, 40);
    Actor_FaceDirection(8, 0x5000, 0);
    SceneState_ForwardMaskedHalfwordWith10(9, 0x3000);
    Actor_ShowEmote(9, 0x105, 20);
    FieldScene_CallPairWith10(9);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 20);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    FieldScene_CallPairWith10(1);
    Actor_ShowEmote(10, 0x102, 40);
    FieldScene_CallPairWith10(10);
    Actor_SetAnimationAndWait(11, 3);
    FieldScene_CallPairWith10(11);
    SceneState_ForwardMaskedHalfwordWith10(2, 0xa000);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    FieldScene_CallPairWith10(2);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    FieldScene_CallPairWith10(3);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 20);
    SceneState_ForwardMaskedHalfwordWith10(1, 0);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    FieldScene_CallPairWith10(1);
    SceneState_ForwardMaskedHalfwordWith10(0, 0x6000);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    SceneState_ForwardMaskedHalfwordWith10(2, 0x8000);
    FieldScene_CallPairWith10(2);
    Actor_SetAnimation(ACTOR_MIA, 4);
    Event_Wait(20);
    FieldScene_CallPairWith10(3);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    SceneState_ForwardMaskedHalfwordWith10(1, 0x2000);
    FieldScene_CallPairWith10(1);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 60);
    Actor_FaceDirection(ACTOR_IVAN, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 80);
    Actor_ShowEmote(ACTOR_IVAN, 0x106, 0);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    SceneState_ForwardMaskedHalfwordWith10(2, 0xc000);
    FieldScene_CallPairWith10(2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 20);
    SceneState_ForwardMaskedHalfwordWith10(8, 0x3000);
    Actor_SetAnimationAndWait(8, 3);
    FieldScene_CallPairWith10(8);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 40);
    Actor_RunRepeatedMotion(9, 1);
    FieldScene_CallPairWith10(9);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 40);
    Actor_RunRepeatedMotion(10, 1);
    FieldScene_CallPairWith10(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    SceneState_ForwardMaskedHalfwordWith10(3, 0xc000);
    Actor_SetAnimationAndWait(11, 3);
    FieldScene_CallPairWith10(11);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x105, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x105, 80);
    Actor_ShowEmote(ACTOR_IVAN, 0x106, 0);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    SceneState_ForwardMaskedHalfwordWith10(2, 0xe000);
    FieldScene_CallPairWith10(2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 20);
    Actor_ShowEmote(11, 0x101, 60);
    Event_OpenMessage(ACTOR_IVAN, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    flag = 0;
    if (Event_ChooseYesNo(0, 0) == 1) {
        FieldScene_CallPairWith10(2);
        flag = 1;
    } else {
        gEventWork->message++;
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        SceneState_ForwardMaskedHalfwordWith10(2, 0xc000);
        FieldScene_CallPairWith10(2);
    }
    if (flag != 0) {
        gEventWork->message++;
    }
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_FaceDirection(9, 0, 0);
    Actor_FaceDirection(10, 0x3000, 0);
    Actor_FaceDirection(11, 0xb000, 20);
    Actor_ShowEmote(8, 0x105, 0);
    Actor_ShowEmote(9, 0x105, 0);
    Actor_ShowEmote(10, 0x105, 0);
    Actor_ShowEmote(11, 0x105, 60);
    Actor_RunRepeatedMotion(8, 1);
    SceneState_ForwardMaskedHalfwordWith10(8, 0x3000);
    FieldScene_CallPairWith10(8);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 20);
    Actor_RunRepeatedMotion(9, 1);
    SceneState_ForwardMaskedHalfwordWith10(9, 0x3000);
    FieldScene_CallPairWith10(9);
    Actor_RunRepeatedMotion(10, 1);
    SceneState_ForwardMaskedHalfwordWith10(10, 0x5000);
    FieldScene_CallPairWith10(10);
    SceneState_ForwardMaskedHalfwordWith10(11, 0x8000);
    Actor_SetAnimationAndWait(11, 3);
    FieldScene_CallPairWith10(11);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Actor_EnableActionCallback(ACTOR_GERALD, (s32)KorashiamuIriguchi_ActionTable3);
    Actor_EnableActionCallback(ACTOR_IVAN, (s32)KorashiamuIriguchi_ActionTable3);
    Object_SetActionCallbackAndRefreshById(3, (s32)KorashiamuIriguchi_ActionTable3);
    Event_Wait(20);
    SceneState_ForwardMaskedHalfwordWith10(0, 0);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(11, 3);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetAnimation(11, 2);
    Actor_MoveToAndWait(11, 830, 152);
    Actor_MoveToAndWait(11, 808, 164);
    Actor_SetDestination(11, 808, 312);
    Event_Wait(20);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x3280000, -1, 0x1380000, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 808, 164);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 312);
    Event_Wait(60);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 40;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(64);
}

void FieldScene_RunScene3b9_020023e0(void)
{
    u32 i;
    struct FieldActor *record;

    if (GameFlag_IsSet(5) != 0) {
        GameFlag_Set(0x16d);
        Party_RemoveActiveOwner(5);
        Party_AddActiveOwner(3);
    }
    Event_Begin();
    Actor_SetPosition(11, 0x2c80000, 0x24c0000);
    Task_Wait(1);
    Camera_FollowActor(11, 1);
    Actor_SetSpeed(11, 0x19999, 0xcccc);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    record = Object_GetById(11);
    record->facing = 0;
    Event_OpenScreen();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetAnimation(11, 2);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x30c, 0x24c);
    Actor_MoveToAndWait(11, 0x32c, 0x24c);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x370, 0x24c);
    Actor_MoveToAndWait(11, 0x390, 0x24c);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x3d4, 0x24c);
    Actor_SetDestination(11, 0x3f4, 0x24c);
    Event_CloseScreen();
    Event_WaitForScreen();
    if (GameFlag_IsSet(0x90f) != 0) {
        Event_RequestExit(31);
    } else {
        Event_RequestExit(65);
    }
}

void FieldScene_RunScene3b9_020024d8(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Object_GetById(13);
    Event_Begin();
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(8, 2);
    Actor_Stop(13);
    Task_Wait(1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_SetAnimation(13, 1);
    Actor_FaceDirection(12, 0xd000, 0);
    Actor_FaceDirection(13, 0, 0);
    Actor_FaceDirection(14, 0x8000, 0);
    Actor_FaceDirection(15, 0xd000, 0);
    Actor_FaceDirection(16, 0x8000, 0);
    Actor_FaceDirection(17, 0xb000, 0);
    Actor_FaceDirection(18, 0xb000, 0);
    Event_SetMessage((s32)MsgKorashiamuPrepareYourselvesContestantsFinalsWill);
    FieldScene_CallPairWith10(8);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    {
        u16 *target = (u16 *)(rec7 + 100);
        s32 shown = 0x2d0;

        *target = shown;
    }
    {
        u16 *target = (u16 *)(rec7 + 102);
        s32 shown = 112;

        *target = shown;
    }
    Actor_EnableActionCallback(13, 2);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(15, 0x5000, 0);
    Actor_FaceDirection(16, 0, 0);
    Actor_FaceDirection(17, 0x5000, 0);
    Actor_FaceDirection(18, 0x5000, 0);
    Event_End();
}

void FieldScene_RunScene3b9_020025f0(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    Event_OpenScreen();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x30c, 0x1ac);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x370, 0x1ac);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x3d4, 0x1ac);
    Event_CloseScreen();
    Event_WaitForScreen();
    if (GameFlag_IsSet(0x90f) != 0) {
        Event_RequestExit(32);
    } else {
        Event_RequestExit(12);
    }
}

void FieldScene_RunScene3b9_02002668(void)
{
    u32 i;
    s32 record;
    s32 base5_200adac;

    Event_Begin();
    Actor_SetPosition(ACTOR_GERALD, 0x3180000, 0x880000);
    Actor_SetPosition(ACTOR_IVAN, 0x3380000, 0x880000);
    Actor_SetPosition(ACTOR_MIA, 0x3280000, 0x980000);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(8, 1);
    Actor_SetAnimation(8, 3);
    Event_SetMessage((s32)MsgKorashiamuSeeYouveMadeThroughYour);
    FieldScene_CallPairWith10(8);
    Actor_RunRepeatedMotion(9, 1);
    FieldScene_CallPairWith10(9);
    Actor_RunRepeatedMotion(10, 1);
    FieldScene_CallPairWith10(10);
    Actor_RunRepeatedMotion(11, 1);
    Actor_SetAnimation(11, 3);
    FieldScene_CallPairWith10(11);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    base5_200adac = (s32)KorashiamuIriguchi_ActionTable3;
    Actor_EnableActionCallback(ACTOR_GERALD, base5_200adac);
    Engine_ActorEnableActionCallback(2, base5_200adac);
    Object_SetActionCallbackAndRefreshById(3, base5_200adac);
    Event_Wait(20);
    SceneState_ForwardMaskedHalfwordWith10(0, 0);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(11, 3);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetAnimation(11, 2);
    Actor_MoveToAndWait(11, 0x33e, 152);
    Actor_MoveToAndWait(11, 0x328, 164);
    Actor_SetDestination(11, 0x328, 0x138);
    Event_Wait(20);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x3280000, -1, 0x1380000, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x328, 164);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x328, 0x138);
    Event_Wait(60);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(67);
}

void FieldScene_RunScene3b9_02002820(void)
{
    u32 i;
    struct FieldActor *record;

    if (GameFlag_IsSet(5) != 0) {
        GameFlag_Set(0x16d);
        Party_RemoveActiveOwner(5);
        Party_AddActiveOwner(3);
    }
    Event_Begin();
    Actor_SetPosition(11, 0x3640000, 0x24c0000);
    Task_Wait(1);
    Camera_FollowActor(11, 1);
    Actor_SetSpeed(11, 0x19999, 0xcccc);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    record = Object_GetById(11);
    record->facing = 0x8000;
    Event_OpenScreen();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetAnimation(11, 2);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x320, 0x24c);
    Actor_MoveToAndWait(11, 0x300, 0x24c);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x2bc, 0x24c);
    Actor_MoveToAndWait(11, 0x29c, 0x24c);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x258, 0x24c);
    Actor_SetDestination(11, 0x238, 0x24c);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(21);
}

void FieldScene_RunScene3b9_02002904(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    Event_OpenScreen();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x320, 0x1ac);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x2bc, 0x1ac);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x258, 0x1ac);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(22);
}

void FieldScene_RunScene3b9_02002964(void)
{
    u32 i;
    s32 record;
    s32 base5_200adac;

    Event_Begin();
    Actor_SetPosition(ACTOR_GERALD, 0x3180000, 0x880000);
    Actor_SetPosition(ACTOR_IVAN, 0x3380000, 0x880000);
    Actor_SetPosition(ACTOR_MIA, 0x3280000, 0x980000);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(8, 1);
    Actor_SetAnimation(8, 3);
    Event_SetMessage((s32)MsgKorashiamuWonBothMatches);
    FieldScene_CallPairWith10(8);
    Actor_RunRepeatedMotion(9, 1);
    FieldScene_CallPairWith10(9);
    Actor_RunRepeatedMotion(10, 1);
    FieldScene_CallPairWith10(10);
    Actor_RunRepeatedMotion(11, 1);
    Actor_SetAnimation(11, 3);
    FieldScene_CallPairWith10(11);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    base5_200adac = (s32)KorashiamuIriguchi_ActionTable3;
    Actor_EnableActionCallback(ACTOR_GERALD, base5_200adac);
    Engine_ActorEnableActionCallback(2, base5_200adac);
    Object_SetActionCallbackAndRefreshById(3, base5_200adac);
    Event_Wait(20);
    SceneState_ForwardMaskedHalfwordWith10(0, 0);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(11, 3);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetAnimation(11, 2);
    Actor_MoveToAndWait(11, 0x33e, 152);
    Actor_MoveToAndWait(11, 0x328, 164);
    Actor_SetDestination(11, 0x328, 0x138);
    Event_Wait(20);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x3280000, -1, 0x1380000, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x328, 164);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x328, 0x138);
    Event_Wait(60);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(64);
}

void ActorPresentation_SetActorsTwelveToEighteen(void)
{
    Actor_SetChildValue(12, 3);
    Actor_SetChildValue(13, 0);
    Actor_SetChildValue(14, 4);
    Actor_SetChildValue(15, 1);
    Actor_SetChildValue(16, 5);
    Actor_SetChildValue(17, 2);
    Actor_SetChildValue(18, 6);
    Object_SetActionById(13, 10);
    Object_SetActionById(14, 20);
    Actor_SetAnimation(15, 0);
    Object_SetActionById(16, 40);
    Object_SetActionById(17, 50);
    Object_SetActionById(18, 60);
}
