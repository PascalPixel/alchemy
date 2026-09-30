#include "STATUS.H"
extern u8 MsgKorashiamuHaveMakeThroughCountlessMatches[];
extern u8 MsgKorashiamuHavePerfectTechniquesLikeThose[];
extern u8 MsgKorashiamuSilence[];

void SceneState_ForwardMaskedHalfwordWith10();

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
    rec2 = Engine_ActorGet(21);
    Actor_SetSpriteFlags(rec2, 0);
    rec2 = Engine_ActorGet(19);
    rec2->scale_x = -0x10000;
    rec2 = Value1(Engine_ActorGet, 20);
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
    Value2(SceneState_ForwardMaskedHalfwordWith10, 18, shift);
    Actor_Stop(16);
    rec2 = Engine_ActorGet(16);
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
    Call2((void (*)())Engine_ActorSetAnimation, 9, 2);
    Actor_SetAnimation(10, 2);
    Actor_SetAnimation(11, 2);
    Actor_SetAnimation(12, 2);
    Actor_SetAnimation(13, 2);
    Actor_SetAnimation(14, 0);
    Actor_SetAnimation(15, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetAnimation(17, 0);
    Actor_SetAnimation(18, 0);
    rec2 = Engine_ActorGet(21);
    Actor_SetSpriteFlags(rec2, 0);
    rec2 = Engine_ActorGet(19);
    rec2->scale_x = -0x10000;
    rec2 = Value1(Engine_ActorGet, 20);
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
    Value2(SceneState_ForwardMaskedHalfwordWith10, 18, 0x3000);
    Actor_SetAttachedEffect(17, 0x101);
    /* Clear the motion flags, then step the height up and back down
     * 20 times, waiting between each step. */
    rec = Value1(Engine_ActorGet, 21);
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
    rec2 = Engine_ActorGet(17);
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
