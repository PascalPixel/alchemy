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

    Engine_EventBegin();
    flag_8a4 = GameFlag_IsSet(0x8a4);
    if (flag_8a4 != 0) {
        Engine_ActorFaceEachOther(RECORD_17, ACTOR_PARTY_LEADER, 40);
        Engine_EventSetMessage((s32)MsgKorashiamuHavePerfectTechniquesLikeThose);
        FieldScene_CallPairWith10(RECORD_17);
        Actor_FaceDirection(RECORD_17, 0x3000, 20);
    } else {
        Engine_ActorStartRepeatedMotion(RECORD_17, 2);
        Engine_EventSetMessage((s32)MsgKorashiamuSilence);
        Event_ShowMessage(RECORD_17, 0);
        /* Byte at +85 of the record returned by ((u8 *)Engine_EventGetViewCenter()); written
         * with the (already known zero) flag value here. */
        *(u8 *)(((u8 *)Engine_EventGetViewCenter()) + 85) = flag_8a4;
        Engine_TaskWait(1);
        Camera_SetSpeed(0x66666, 0xcccc);
        Camera_MoveTo(0x21c0000, -1, 0xd00000, 1);
        Engine_CameraWaitForMove();
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        gEventWork->transition_frames = 32;
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
        if (GameFlag_IsSet(0x8a3) != 0) {
            Engine_EventRequestExit(70);
        } else {
            Engine_EventRequestExit(7);
        }
    }
    Engine_EventEnd();
}

void FieldScene_BuildActorPresentationGroup(void)
{

    struct FieldActor *rec2;
    s32 shift;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Audio_PlayCue(247);
    Engine_ActorSetAnimation(8, 2);
    Engine_ActorSetAnimation(9, 2);
    Engine_ActorSetAnimation(10, 2);
    Engine_ActorSetAnimation(11, 2);
    Engine_ActorSetAnimation(12, 2);
    Engine_ActorSetAnimation(13, 2);
    Engine_ActorSetAnimation(14, 0);
    Engine_ActorSetAnimation(15, 0);
    Engine_ActorSetAnimation(16, 0);
    Engine_ActorSetAnimation(17, 0);
    Engine_ActorSetAnimation(18, 0);
    rec2 = Object_GetById(21);
    Engine_ActorSetSpriteFlags(rec2, 0);
    rec2 = Object_GetById(19);
    rec2->scale_x = -0x10000;
    rec2 = Object_GetById(20);
    rec2->scale_x = -0x10000;
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Engine_TaskWait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gEventWork->transition_frames = 32;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(16, 1);
    Engine_EventWait(20);
    Actor_SetSpeed(16, 0xcccc, 0x6666);
    Actor_WalkToAndWait(16, 164, 0x388);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(16, 9);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(16, 10);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(16, 1);
    Engine_EventWait(20);
    Actor_WalkToAndWait(16, 164, 0x398);
    Actor_WalkToAndWait(16, 185, 0x398);
    Actor_FaceDirection(16, 0xc000, 20);
    Actor_WalkToAndWait(16, 185, 0x394);
    Engine_ActorSetAnimation(16, 11);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(16, 1);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(16, 3);
    Engine_EventWait(40);
    Engine_ActorEnableActionCallback(16, (s32)KorashiamuIriguchi_ActionTable9);
    Engine_EventWait(80);
    Actor_SetAttachedEffect(16, 0x102);
    Engine_EventWait(60);
    Actor_FaceDirection(14, 0xd000, 0);
    Actor_FaceDirection(15, 0x5000, 0);
    Actor_FaceDirection(17, 0, 0);
    Actor_FaceDirection(18, 0x8000, 20);
    Actor_SetAttachedEffect(14, 0x102);
    Actor_SetAttachedEffect(15, 0x102);
    Actor_SetAttachedEffect(17, 0x102);
    Actor_SetAttachedEffect(18, 0x102);
    Engine_EventWait(60);
    Actor_FaceDirection(14, 0x3000, 0);
    Actor_FaceDirection(15, 0x3000, 0);
    Actor_FaceDirection(17, 0x3000, 0);
    shift = 0x3000;
    SceneState_ForwardMaskedHalfwordWith10(18, shift);
    Engine_ActorStop(16);
    rec2 = Object_GetById(16);
    rec2->facing = 0xd000;
    rec2->scale_x = 0x10000;
    rec2->scale_y = 0x10000;
    Engine_EventWait(20);
    Engine_ActorSetAnimation(16, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(19, 5);
    Engine_ActorSetAnimation(20, 5);
    Engine_EventWait(60);
    Actor_FaceDirection(16, shift, 20);
    Engine_ActorSetAnimation(16, 8);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(14, 4);
    Engine_ActorSetAnimation(15, 4);
    Engine_ActorSetAnimation(17, 4);
    Engine_ActorSetAnimationAndWait(18, 4);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(16, 4);
    Engine_EventWait(10);
    Actor_SetSpeed(16, 0x20000, 0x10000);
    Actor_WalkToAndWait(16, 162, 0x394);
    Actor_WalkToAndWait(16, 162, 0x37a);
    Engine_ActorSetAnimation(19, 1);
    Engine_ActorSetAnimation(20, 1);
    Actor_WalkToAndWait(16, 184, 0x35f);
    Actor_WalkToAndWait(16, 184, 0x31c);
    Actor_SetPosition(16, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    gEventWork->transition_frames = 16;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_EventRequestExit(69);
    Engine_EventEnd();
}

void SceneState_SetRuntimeWord448To513(void)
{

    Engine_EventBegin();
    *(s32 *)((u8 *)gEventWork + 448) = 513;

    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();

    Engine_EventWait(20);
    SceneState_ForwardMaskedHalfwordWith10(17, 160 << 7);
    Engine_EventSetMessage((s32)MsgKorashiamuHaveMakeThroughCountlessMatches);

    if (Engine_GameFlagIsSet(0x8a4) != 0) {
        *(u16 *)((u8 *)gEventWork + 472) =
            (u16)(*(u16 *)((u8 *)gEventWork + 472) + 1);
    }

    FieldScene_CallPairWith10(17);
    SceneState_ForwardMaskedHalfwordWith10(17, 192 << 6);
    Engine_GameFlagSet(0x8a3);

    Engine_EventEnd();
}

/* Sets up actors 8-20 (position, pose, or movement/sprite flags), advances
 * two actor records' +24 fields, advances the shared scene phase, then runs
 * a long chain of per-actor moves, pose changes, and waits. */
void FieldScene_RunMiddleSequence(void)
{

    u32 counter;
    struct FieldActor *rec;
    struct FieldActor *rec2;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Audio_PlayCue(247);
    Engine_ActorSetAnimation(8, 2);
    Engine_ActorSetAnimation(9, 2);
    Engine_ActorSetAnimation(10, 2);
    Engine_ActorSetAnimation(11, 2);
    Engine_ActorSetAnimation(12, 2);
    Engine_ActorSetAnimation(13, 2);
    Engine_ActorSetAnimation(14, 0);
    Engine_ActorSetAnimation(15, 0);
    Actor_SetPosition(16, 0, 0);
    Engine_ActorSetAnimation(17, 0);
    Engine_ActorSetAnimation(18, 0);
    rec2 = Object_GetById(21);
    Engine_ActorSetSpriteFlags(rec2, 0);
    rec2 = Object_GetById(19);
    rec2->scale_x = -0x10000;
    rec2 = Object_GetById(20);
    rec2->scale_x = -0x10000;
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Engine_TaskWait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gEventWork->transition_frames = 32;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(17, 1);
    Engine_EventWait(20);
    Actor_SetSpeed(17, 0xcccc, 0x6666);
    Actor_WalkToAndWait(17, 164, 0x388);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(17, 9);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(17, 10);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(17, 1);
    Engine_EventWait(20);
    Actor_WalkToAndWait(17, 164, 0x398);
    Actor_WalkToAndWait(17, 185, 0x398);
    Actor_FaceDirection(17, 0xc000, 20);
    Actor_WalkToAndWait(17, 185, 0x394);
    Engine_ActorSetAnimation(17, 11);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(17, 1);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(17, 3);
    Engine_EventWait(40);
    Engine_ActorEnableActionCallback(17, (s32)KorashiamuIriguchi_ActionTable9);
    Engine_EventWait(80);
    Actor_SetAttachedEffect(17, 0x102);
    Engine_EventWait(60);
    Actor_FaceDirection(14, 0xd000, 0);
    Actor_FaceDirection(15, 0x5000, 0);
    Actor_FaceDirection(18, 0x8000, 20);
    Actor_SetAttachedEffect(14, 0x102);
    Actor_SetAttachedEffect(15, 0x102);
    Actor_SetAttachedEffect(17, 0x102);
    Actor_SetAttachedEffect(18, 0x102);
    Engine_EventWait(60);
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
        Engine_TaskWait(4);
        rec->y.fixed += -0x4ccc;
        Engine_TaskWait(4);
    }
    Engine_ActorSetAnimation(19, 6);
    Engine_ActorSetAnimation(20, 6);
    Engine_EventWait(60);
    Actor_SetAttachedEffect(17, 0x100);
    Engine_ActorStop(17);
    Engine_ActorSetAnimation(17, 1);
    rec2 = Object_GetById(17);
    rec2->facing = 0xd000;
    rec->motion_flags = 3;
    rec->scale_x = 0x10000;
    rec->scale_y = 0x10000;
    Engine_EventWait(10);
    Audio_PlayCue(107);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Engine_EventWait(10);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Actor_SetSpeed(17, 0x19999, 0xcccc);
    Actor_WalkToAndWait(17, 208, 0x3a0);
    Audio_PlayCue(92);
    Actor_FaceDirection(17, 0x3000, 20);
    Engine_ActorSetAnimation(17, 9);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(17, 10);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(17, 9);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(17, 10);
    Engine_EventWait(80);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    gEventWork->transition_frames = 16;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    GameFlag_Set(0x8a4);
    Engine_EventRequestExit(69);
    Engine_EventEnd();
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

    Engine_TaskWait(1);
    no = gGameState.entrance;
    switch (no) {
    case 5:
        Engine_ActorSetAnimation(8, 2);
        Engine_ActorSetAnimation(9, 2);
        break;
    case 69:
        Engine_ActorSetAnimation(8, 2);
        Engine_ActorSetAnimation(9, 2);
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
    Engine_EventWait(10);
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

    Engine_EventBegin();
    Actor_SetPosition(ACTOR_GERALD, 0x3180000, 0x880000);
    Actor_SetPosition(ACTOR_IVAN, 0x3380000, 0x880000);
    Actor_SetPosition(ACTOR_MIA, 0x3280000, 0x980000);
    Engine_TaskWait(1);
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_ColorBufferApplySource(0, 0);
    ColorBuffer_ApplyTarget(0, 0);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
    gEventWork->transition_frames = 1;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_ColorBufferApplySource(0, 0);
    ColorBuffer_ApplyTarget(0x10002, 0);
    Engine_ColorBufferInterpolate(40);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventWait(40);
    SceneState_ForwardMaskedHalfwordWith10(8, 0x3000);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(40);
    Engine_EventWait(80);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgKorashiamuRobinOnlyOneEnteringFinals);
    FieldScene_CallPairWith10(2);
    Engine_ActorSetAnimationAndWait(8, 3);
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
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    FieldScene_CallPairWith10(1);
    Actor_ShowEmote(10, 0x102, 40);
    FieldScene_CallPairWith10(10);
    Engine_ActorSetAnimationAndWait(11, 3);
    FieldScene_CallPairWith10(11);
    SceneState_ForwardMaskedHalfwordWith10(2, 0xa000);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 4);
    FieldScene_CallPairWith10(2);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    FieldScene_CallPairWith10(3);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 20);
    SceneState_ForwardMaskedHalfwordWith10(1, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    FieldScene_CallPairWith10(1);
    SceneState_ForwardMaskedHalfwordWith10(0, 0x6000);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    SceneState_ForwardMaskedHalfwordWith10(2, 0x8000);
    FieldScene_CallPairWith10(2);
    Engine_ActorSetAnimation(ACTOR_MIA, 4);
    Engine_EventWait(20);
    FieldScene_CallPairWith10(3);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
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
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    SceneState_ForwardMaskedHalfwordWith10(2, 0xc000);
    FieldScene_CallPairWith10(2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 20);
    SceneState_ForwardMaskedHalfwordWith10(8, 0x3000);
    Engine_ActorSetAnimationAndWait(8, 3);
    FieldScene_CallPairWith10(8);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 40);
    Engine_ActorRunRepeatedMotion(9, 1);
    FieldScene_CallPairWith10(9);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 40);
    Engine_ActorRunRepeatedMotion(10, 1);
    FieldScene_CallPairWith10(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    SceneState_ForwardMaskedHalfwordWith10(3, 0xc000);
    Engine_ActorSetAnimationAndWait(11, 3);
    FieldScene_CallPairWith10(11);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x105, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x105, 80);
    Actor_ShowEmote(ACTOR_IVAN, 0x106, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
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
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        FieldScene_CallPairWith10(2);
        flag = 1;
    } else {
        gEventWork->message++;
        Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
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
    Engine_ActorRunRepeatedMotion(8, 1);
    SceneState_ForwardMaskedHalfwordWith10(8, 0x3000);
    FieldScene_CallPairWith10(8);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 20);
    Engine_ActorRunRepeatedMotion(9, 1);
    SceneState_ForwardMaskedHalfwordWith10(9, 0x3000);
    FieldScene_CallPairWith10(9);
    Engine_ActorRunRepeatedMotion(10, 1);
    SceneState_ForwardMaskedHalfwordWith10(10, 0x5000);
    FieldScene_CallPairWith10(10);
    SceneState_ForwardMaskedHalfwordWith10(11, 0x8000);
    Engine_ActorSetAnimationAndWait(11, 3);
    FieldScene_CallPairWith10(11);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Engine_ActorEnableActionCallback(ACTOR_GERALD, (s32)KorashiamuIriguchi_ActionTable3);
    Engine_ActorEnableActionCallback(ACTOR_IVAN, (s32)KorashiamuIriguchi_ActionTable3);
    Object_SetActionCallbackAndRefreshById(3, (s32)KorashiamuIriguchi_ActionTable3);
    Engine_EventWait(20);
    SceneState_ForwardMaskedHalfwordWith10(0, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(11, 3);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Engine_ActorSetAnimation(11, 2);
    Actor_MoveToAndWait(11, 830, 152);
    Actor_MoveToAndWait(11, 808, 164);
    Actor_SetDestination(11, 808, 312);
    Engine_EventWait(20);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x3280000, -1, 0x1380000, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 808, 164);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 312);
    Engine_EventWait(60);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 40;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(64);
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
    Engine_EventBegin();
    Actor_SetPosition(11, 0x2c80000, 0x24c0000);
    Engine_TaskWait(1);
    Engine_CameraFollowActor(11, 1);
    Actor_SetSpeed(11, 0x19999, 0xcccc);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    record = Object_GetById(11);
    record->facing = 0;
    Engine_EventOpenScreen();
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Engine_ActorSetAnimation(11, 2);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x30c, 0x24c);
    Actor_MoveToAndWait(11, 0x32c, 0x24c);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x370, 0x24c);
    Actor_MoveToAndWait(11, 0x390, 0x24c);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x3d4, 0x24c);
    Actor_SetDestination(11, 0x3f4, 0x24c);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    if (GameFlag_IsSet(0x90f) != 0) {
        Engine_EventRequestExit(31);
    } else {
        Engine_EventRequestExit(65);
    }
}

void FieldScene_RunScene3b9_020024d8(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Object_GetById(13);
    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_ActorStop(13);
    Engine_TaskWait(1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Engine_ActorSetAnimation(13, 1);
    Actor_FaceDirection(12, 0xd000, 0);
    Actor_FaceDirection(13, 0, 0);
    Actor_FaceDirection(14, 0x8000, 0);
    Actor_FaceDirection(15, 0xd000, 0);
    Actor_FaceDirection(16, 0x8000, 0);
    Actor_FaceDirection(17, 0xb000, 0);
    Actor_FaceDirection(18, 0xb000, 0);
    Engine_EventSetMessage((s32)MsgKorashiamuPrepareYourselvesContestantsFinalsWill);
    FieldScene_CallPairWith10(8);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
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
    Engine_ActorEnableActionCallback(13, 2);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(15, 0x5000, 0);
    Actor_FaceDirection(16, 0, 0);
    Actor_FaceDirection(17, 0x5000, 0);
    Actor_FaceDirection(18, 0x5000, 0);
    Engine_EventEnd();
}

void FieldScene_RunScene3b9_020025f0(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    Engine_EventOpenScreen();
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x30c, 0x1ac);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x370, 0x1ac);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x3d4, 0x1ac);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    if (GameFlag_IsSet(0x90f) != 0) {
        Engine_EventRequestExit(32);
    } else {
        Engine_EventRequestExit(12);
    }
}

void FieldScene_RunScene3b9_02002668(void)
{
    u32 i;
    s32 record;
    s32 base5_200adac;

    Engine_EventBegin();
    Actor_SetPosition(ACTOR_GERALD, 0x3180000, 0x880000);
    Actor_SetPosition(ACTOR_IVAN, 0x3380000, 0x880000);
    Actor_SetPosition(ACTOR_MIA, 0x3280000, 0x980000);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorSetAnimation(8, 3);
    Engine_EventSetMessage((s32)MsgKorashiamuSeeYouveMadeThroughYour);
    FieldScene_CallPairWith10(8);
    Engine_ActorRunRepeatedMotion(9, 1);
    FieldScene_CallPairWith10(9);
    Engine_ActorRunRepeatedMotion(10, 1);
    FieldScene_CallPairWith10(10);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_ActorSetAnimation(11, 3);
    FieldScene_CallPairWith10(11);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    base5_200adac = (s32)KorashiamuIriguchi_ActionTable3;
    Engine_ActorEnableActionCallback(ACTOR_GERALD, base5_200adac);
    Engine_ActorEnableActionCallback(2, base5_200adac);
    Object_SetActionCallbackAndRefreshById(3, base5_200adac);
    Engine_EventWait(20);
    SceneState_ForwardMaskedHalfwordWith10(0, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(11, 3);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Engine_ActorSetAnimation(11, 2);
    Actor_MoveToAndWait(11, 0x33e, 152);
    Actor_MoveToAndWait(11, 0x328, 164);
    Actor_SetDestination(11, 0x328, 0x138);
    Engine_EventWait(20);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x3280000, -1, 0x1380000, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x328, 164);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x328, 0x138);
    Engine_EventWait(60);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(67);
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
    Engine_EventBegin();
    Actor_SetPosition(11, 0x3640000, 0x24c0000);
    Engine_TaskWait(1);
    Engine_CameraFollowActor(11, 1);
    Actor_SetSpeed(11, 0x19999, 0xcccc);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    record = Object_GetById(11);
    record->facing = 0x8000;
    Engine_EventOpenScreen();
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Engine_ActorSetAnimation(11, 2);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x320, 0x24c);
    Actor_MoveToAndWait(11, 0x300, 0x24c);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x2bc, 0x24c);
    Actor_MoveToAndWait(11, 0x29c, 0x24c);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x258, 0x24c);
    Actor_SetDestination(11, 0x238, 0x24c);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(21);
}

void FieldScene_RunScene3b9_02002904(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    Engine_EventOpenScreen();
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x320, 0x1ac);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x2bc, 0x1ac);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x258, 0x1ac);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(22);
}

void FieldScene_RunScene3b9_02002964(void)
{
    u32 i;
    s32 record;
    s32 base5_200adac;

    Engine_EventBegin();
    Actor_SetPosition(ACTOR_GERALD, 0x3180000, 0x880000);
    Actor_SetPosition(ACTOR_IVAN, 0x3380000, 0x880000);
    Actor_SetPosition(ACTOR_MIA, 0x3280000, 0x980000);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorSetAnimation(8, 3);
    Engine_EventSetMessage((s32)MsgKorashiamuWonBothMatches);
    FieldScene_CallPairWith10(8);
    Engine_ActorRunRepeatedMotion(9, 1);
    FieldScene_CallPairWith10(9);
    Engine_ActorRunRepeatedMotion(10, 1);
    FieldScene_CallPairWith10(10);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_ActorSetAnimation(11, 3);
    FieldScene_CallPairWith10(11);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    base5_200adac = (s32)KorashiamuIriguchi_ActionTable3;
    Engine_ActorEnableActionCallback(ACTOR_GERALD, base5_200adac);
    Engine_ActorEnableActionCallback(2, base5_200adac);
    Object_SetActionCallbackAndRefreshById(3, base5_200adac);
    Engine_EventWait(20);
    SceneState_ForwardMaskedHalfwordWith10(0, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(11, 3);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Engine_ActorSetAnimation(11, 2);
    Actor_MoveToAndWait(11, 0x33e, 152);
    Actor_MoveToAndWait(11, 0x328, 164);
    Actor_SetDestination(11, 0x328, 0x138);
    Engine_EventWait(20);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x3280000, -1, 0x1380000, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x328, 164);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x328, 0x138);
    Engine_EventWait(60);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(64);
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
    Engine_ActorSetAnimation(15, 0);
    Object_SetActionById(16, 40);
    Object_SetActionById(17, 50);
    Object_SetActionById(18, 60);
}
