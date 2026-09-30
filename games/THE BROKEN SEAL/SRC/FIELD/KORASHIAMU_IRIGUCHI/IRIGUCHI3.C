#include "STATUS.H"

extern u8 MsgKorashiamuHaveMakeThroughCountlessMatches[];
extern u8 MsgKorashiamuHavePerfectTechniquesLikeThose[];
extern u8 MsgKorashiamuSilence[];
void SceneState_ForwardMaskedHalfwordWith10();

void FieldScene_DispatchBySelector(void);
void SceneState_ApplyRectsByFlatla384And962(void);

extern u8 MsgKorashiamuPrepareYourselvesContestantsFinalsWill[];
extern u8 MsgKorashiamuRobinOnlyOneEnteringFinals[];
extern u8 MsgKorashiamuSeeYouveMadeThroughYour[];
extern u8 MsgKorashiamuWonBothMatches[];

/*
 * The two flag ids are adjacent but spelled differently, and the spellings
 * are load-bearing.  565 is 0x235 and comes from the owner's single pool
 * word; 564 is built as movs #141 / lsls #2, so it must stay a plain decimal
 * value rather than another pool constant.  The 24-byte owner covers that
 * pool word and the alignment halfword after it.  What the pair gates is not
 * established.
 */
void SceneState_ApplyFlags565And564(void)
{
    GameFlag_Clear(0x235);
    GameFlag_Set(564);
}

/* Reads flag record 0x8a4; when set, runs one short setup on record 17.
 * When clear, runs a longer setup on record 17 plus scene phase/field
 * updates, then checks flag record 0x8a3 to pick a final call. Either path
 * ends with Engine_EventEnd(). */
void FieldScene_RunConditionalSceneSetup(void)
{
    u32 i;
    s32 flag_8a4;
    s32 record;

    Event_Begin();
    flag_8a4 = GameFlag_IsSet(0x8a4);
    if (flag_8a4 != 0) {
        Actor_FaceEachOther(RECORD_17, ACTOR_PARTY_LEADER, 40);
        Event_SetMessage((s32)MsgKorashiamuHavePerfectTechniquesLikeThose);
        FieldScene_CallPairWith10(RECORD_17);
        Actor_FaceDirection(RECORD_17, 0x3000, 20);
    } else {
        Actor_StartRepeatedMotion(RECORD_17, 2);
        Event_SetMessage((s32)MsgKorashiamuSilence);
        Event_ShowMessage(RECORD_17, 0);
        /* Byte at +85 of the record returned by ((u8 *)Engine_EventGetViewCenter()); written
         * with the (already known zero) flag value here. */
        *(u8 *)(((u8 *)Engine_EventGetViewCenter()) + 85) = flag_8a4;
        Task_Wait(1);
        Camera_SetSpeed(0x66666, 0xcccc);
        Camera_MoveTo(0x21c0000, -1, 0xd00000, 1);
        Camera_WaitForMove();
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        gEventWork->transition_frames = 32;
        Event_CloseScreen();
        Event_WaitForScreen();
        if (GameFlag_IsSet(0x8a3) != 0) {
            Event_RequestExit(70);
        } else {
            Event_RequestExit(7);
        }
    }
    Event_End();
}

void FieldScene_BuildActorPresentationGroup(void)
{

    struct FieldActor *rec2;
    s32 shift;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Audio_PlayCue(247);
    Actor_SetAnimation(8, 2);
    Actor_SetAnimation(9, 2);
    Actor_SetAnimation(10, 2);
    Actor_SetAnimation(11, 2);
    Actor_SetAnimation(12, 2);
    Actor_SetAnimation(13, 2);
    Actor_SetAnimation(14, 0);
    Actor_SetAnimation(15, 0);
    Actor_SetAnimation(16, 0);
    Actor_SetAnimation(17, 0);
    Actor_SetAnimation(18, 0);
    rec2 = Object_GetById(21);
    Actor_SetSpriteFlags(rec2, 0);
    rec2 = Object_GetById(19);
    rec2->scale_x = -0x10000;
    rec2 = Object_GetById(20);
    rec2->scale_x = -0x10000;
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Task_Wait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gEventWork->transition_frames = 32;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(16, 1);
    Event_Wait(20);
    Actor_SetSpeed(16, 0xcccc, 0x6666);
    Actor_WalkToAndWait(16, 164, 0x388);
    Event_Wait(20);
    Actor_SetAnimation(16, 9);
    Event_Wait(40);
    Actor_SetAnimation(16, 10);
    Event_Wait(60);
    Actor_SetAnimation(16, 1);
    Event_Wait(20);
    Actor_WalkToAndWait(16, 164, 0x398);
    Actor_WalkToAndWait(16, 185, 0x398);
    Actor_FaceDirection(16, 0xc000, 20);
    Actor_WalkToAndWait(16, 185, 0x394);
    Actor_SetAnimation(16, 11);
    Event_Wait(40);
    Actor_RunRepeatedMotion(16, 1);
    Event_Wait(60);
    Actor_RunRepeatedMotion(16, 3);
    Event_Wait(40);
    Actor_EnableActionCallback(16, (s32)KorashiamuIriguchi_ActionTable9);
    Event_Wait(80);
    Actor_SetAttachedEffect(16, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(14, 0xd000, 0);
    Actor_FaceDirection(15, 0x5000, 0);
    Actor_FaceDirection(17, 0, 0);
    Actor_FaceDirection(18, 0x8000, 20);
    Actor_SetAttachedEffect(14, 0x102);
    Actor_SetAttachedEffect(15, 0x102);
    Actor_SetAttachedEffect(17, 0x102);
    Actor_SetAttachedEffect(18, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(14, 0x3000, 0);
    Actor_FaceDirection(15, 0x3000, 0);
    Actor_FaceDirection(17, 0x3000, 0);
    shift = 0x3000;
    SceneState_ForwardMaskedHalfwordWith10(18, shift);
    Actor_Stop(16);
    rec2 = Object_GetById(16);
    rec2->facing = 0xd000;
    rec2->scale_x = 0x10000;
    rec2->scale_y = 0x10000;
    Event_Wait(20);
    Actor_SetAnimation(16, 0);
    Event_Wait(40);
    Actor_SetAnimation(19, 5);
    Actor_SetAnimation(20, 5);
    Event_Wait(60);
    Actor_FaceDirection(16, shift, 20);
    Actor_SetAnimation(16, 8);
    Event_Wait(20);
    Actor_SetAnimation(14, 4);
    Actor_SetAnimation(15, 4);
    Actor_SetAnimation(17, 4);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(40);
    Actor_SetAnimationAndWait(16, 4);
    Event_Wait(10);
    Actor_SetSpeed(16, 0x20000, 0x10000);
    Actor_WalkToAndWait(16, 162, 0x394);
    Actor_WalkToAndWait(16, 162, 0x37a);
    Actor_SetAnimation(19, 1);
    Actor_SetAnimation(20, 1);
    Actor_WalkToAndWait(16, 184, 0x35f);
    Actor_WalkToAndWait(16, 184, 0x31c);
    Actor_SetPosition(16, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    gEventWork->transition_frames = 16;
    Event_CloseScreen();
    Event_WaitForScreen();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Event_RequestExit(69);
    Event_End();
}

void SceneState_SetRuntimeWord448To513(void)
{

    Event_Begin();
    *(s32 *)((u8 *)gEventWork + 448) = 513;

    Event_OpenScreen();
    Event_WaitForScreen();

    Event_Wait(20);
    SceneState_ForwardMaskedHalfwordWith10(17, 160 << 7);
    Event_SetMessage((s32)MsgKorashiamuHaveMakeThroughCountlessMatches);

    if (GameFlag_IsSet(0x8a4) != 0) {
        *(u16 *)((u8 *)gEventWork + 472) =
            (u16)(*(u16 *)((u8 *)gEventWork + 472) + 1);
    }

    FieldScene_CallPairWith10(17);
    SceneState_ForwardMaskedHalfwordWith10(17, 192 << 6);
    GameFlag_Set(0x8a3);

    Event_End();
}

/* Sets up actors 8-20 (position, pose, or movement/sprite flags), advances
 * two actor records' +24 fields, advances the shared scene phase, then runs
 * a long chain of per-actor moves, pose changes, and waits. */
void FieldScene_RunMiddleSequence(void)
{

    u32 counter;
    struct FieldActor *rec;
    struct FieldActor *rec2;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Audio_PlayCue(247);
    Actor_SetAnimation(8, 2);
    ((void (*)())Engine_ActorSetAnimation)(9, 2);
    Actor_SetAnimation(10, 2);
    Actor_SetAnimation(11, 2);
    Actor_SetAnimation(12, 2);
    Actor_SetAnimation(13, 2);
    Actor_SetAnimation(14, 0);
    Actor_SetAnimation(15, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetAnimation(17, 0);
    Actor_SetAnimation(18, 0);
    rec2 = Object_GetById(21);
    Actor_SetSpriteFlags(rec2, 0);
    rec2 = Object_GetById(19);
    rec2->scale_x = -0x10000;
    rec2 = Object_GetById(20);
    rec2->scale_x = -0x10000;
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Task_Wait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gEventWork->transition_frames = 32;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(17, 1);
    Event_Wait(20);
    Actor_SetSpeed(17, 0xcccc, 0x6666);
    Actor_WalkToAndWait(17, 164, 0x388);
    Event_Wait(20);
    Actor_SetAnimation(17, 9);
    Event_Wait(40);
    Actor_SetAnimation(17, 10);
    Event_Wait(60);
    Actor_SetAnimation(17, 1);
    Event_Wait(20);
    Actor_WalkToAndWait(17, 164, 0x398);
    Actor_WalkToAndWait(17, 185, 0x398);
    Actor_FaceDirection(17, 0xc000, 20);
    Actor_WalkToAndWait(17, 185, 0x394);
    Actor_SetAnimation(17, 11);
    Event_Wait(40);
    Actor_RunRepeatedMotion(17, 1);
    Event_Wait(60);
    Actor_RunRepeatedMotion(17, 3);
    Event_Wait(40);
    Actor_EnableActionCallback(17, (s32)KorashiamuIriguchi_ActionTable9);
    Event_Wait(80);
    Actor_SetAttachedEffect(17, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(14, 0xd000, 0);
    Actor_FaceDirection(15, 0x5000, 0);
    Actor_FaceDirection(18, 0x8000, 20);
    Actor_SetAttachedEffect(14, 0x102);
    Actor_SetAttachedEffect(15, 0x102);
    Actor_SetAttachedEffect(17, 0x102);
    Actor_SetAttachedEffect(18, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(14, 0x3000, 0);
    Actor_FaceDirection(15, 0x3000, 0);
    SceneState_ForwardMaskedHalfwordWith10(18, 0x3000);
    Actor_SetAttachedEffect(17, 0x101);
    /* Clear the motion flags, then step the height up and back down
     * 20 times, waiting between each step. */
    rec = Object_GetById(21);
    rec->motion_flags = 0;
    for (counter = 0; counter < 20; counter++) {
        rec->y.fixed += 0x9999;
        Task_Wait(4);
        rec->y.fixed += -0x4ccc;
        Task_Wait(4);
    }
    Actor_SetAnimation(19, 6);
    Actor_SetAnimation(20, 6);
    Event_Wait(60);
    Actor_SetAttachedEffect(17, 0x100);
    Actor_Stop(17);
    Actor_SetAnimation(17, 1);
    rec2 = Object_GetById(17);
    rec2->facing = 0xd000;
    rec->motion_flags = 3;
    rec->scale_x = 0x10000;
    rec->scale_y = 0x10000;
    Event_Wait(10);
    Audio_PlayCue(107);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Event_Wait(10);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Actor_SetSpeed(17, 0x19999, 0xcccc);
    Actor_WalkToAndWait(17, 208, 0x3a0);
    Audio_PlayCue(92);
    Actor_FaceDirection(17, 0x3000, 20);
    Actor_SetAnimation(17, 9);
    Event_Wait(20);
    Actor_SetAnimation(17, 10);
    Event_Wait(40);
    Actor_SetAnimation(17, 9);
    Event_Wait(20);
    Actor_SetAnimation(17, 10);
    Event_Wait(80);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    gEventWork->transition_frames = 16;
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x8a4);
    Event_RequestExit(69);
    Event_End();
}

/* The Colosso entrance's scene start: the first and third scenes run their
   own setup. */
s32 Scene_Initialize(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KorashiamuIriguchi1) {
        FieldScene_DispatchBySelector();
    } else if (scene == (s32)&SceneId_KorashiamuIriguchi3) {
        SceneState_ApplyRectsByFlatla384And962();
    }
    return 0;
}

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
