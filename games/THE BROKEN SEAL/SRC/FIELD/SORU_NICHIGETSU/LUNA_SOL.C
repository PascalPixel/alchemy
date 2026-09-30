#include "SANCTUM.H"
#include "CALL.H"
extern u8 MsgSoruLookSymbolFloor[];
extern u8 MsgSoruThePictureOfLunaChanged[];
extern u8 MsgSoruWhatsHappening[];
extern u8 MsgSoruWhatsHappeningAtTheTrap[];
extern u8 MsgSoruYouFoundIt[];

void UpdateStatueLight1(void)
{
    if ((Random_Next() & 3) != 0) {
        switch (SoruNichigetsu_Light1Timer) {
        case 0:
            Audio_PlayCue(0xbb);
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x21, 1, 5);
            break;
        case 1:
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x21, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x22, 1, 5);
            break;
        case 2:
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x22, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x23, 1, 5);
            break;
        case 3:
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x23, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x24, 1, 5);
            break;
        case 4:
            SoruNichigetsu_FlashState = 2;
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x24, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x25, 1, 5);
            break;
        case 0x50:
            Map_CopyCellsTo(0x2e, 0x31, 30, 0x21, 1, 10);
            break;
        }
        SoruNichigetsu_Light1Timer++;
        if (SoruNichigetsu_Light1Timer > ((u32)(Random_Next() * 40) >> 16) + 90) {
            SoruNichigetsu_Light1Timer = 0;
        }
    }
    if (SoruNichigetsu_FlashState != 0) {
        if (SoruNichigetsu_FlashState == 2) {
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        } else if (SoruNichigetsu_FlashState == 1) {
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        }
        SoruNichigetsu_FlashState--;
    }
}

void UpdateStatueLight2(void)
{
    if ((Random_Next() & 3) != 0) {
        switch (SoruNichigetsu_Light2Timer) {
        case 0:
            Audio_PlayCue(0xbb);
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x21, 1, 5);
            break;
        case 1:
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x21, 1, 1);
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x22, 1, 5);
            break;
        case 2:
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x22, 1, 1);
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x23, 1, 5);
            break;
        case 3:
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x23, 1, 1);
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x24, 1, 5);
            break;
        case 4:
            SoruNichigetsu_FlashState = 2;
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x24, 1, 1);
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x25, 1, 5);
            break;
        case 0x5a:
            Map_CopyCellsTo(0x2f, 0x31, 42, 0x21, 1, 10);
            break;
        }
        SoruNichigetsu_Light2Timer++;
        if (SoruNichigetsu_Light2Timer > ((u32)(Random_Next() * 40) >> 16) + 100) {
            SoruNichigetsu_Light2Timer = 0;
        }
    }
}

void UpdateStatueLight3(void)
{
    if ((Random_Next() & 3) != 0) {
        switch (SoruNichigetsu_Light3Timer) {
        case 0:
            Audio_PlayCue(0xbb);
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x24, 1, 5);
            break;
        case 1:
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x24, 1, 1);
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x25, 1, 5);
            break;
        case 2:
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x25, 1, 1);
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x26, 1, 5);
            break;
        case 3:
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x26, 1, 1);
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x27, 1, 5);
            break;
        case 4:
            SoruNichigetsu_FlashState = 2;
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x27, 1, 1);
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x28, 1, 5);
            break;
        case 0x5f:
            Map_CopyCellsTo(0x30, 0x31, 31, 0x24, 1, 10);
            break;
        }
        SoruNichigetsu_Light3Timer++;
        if (SoruNichigetsu_Light3Timer > ((u32)(Random_Next() * 40) >> 16) + 105) {
            SoruNichigetsu_Light3Timer = 0;
        }
    }
}

void UpdateStatueLight4(void)
{
    if ((Random_Next() & 3) != 0) {
        switch (SoruNichigetsu_Light4Timer) {
        case 0:
            Audio_PlayCue(0xbb);
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x24, 1, 5);
            break;
        case 1:
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x24, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x25, 1, 5);
            break;
        case 2:
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x25, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x26, 1, 5);
            break;
        case 3:
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x26, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x27, 1, 5);
            break;
        case 4:
            SoruNichigetsu_FlashState = 2;
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x27, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x28, 1, 5);
            break;
        case 0x55:
            Map_CopyCellsTo(0x2e, 0x31, 41, 0x24, 1, 10);
            break;
        }
        SoruNichigetsu_Light4Timer++;
        if (SoruNichigetsu_Light4Timer > ((u32)(Random_Next() * 40) >> 16) + 95) {
            SoruNichigetsu_Light4Timer = 0;
        }
    }
}

void FieldScene_PrepareStatueTransition(void)
{
    u32 i;
    s32 record;

    Camera_MoveTo(-1, -1, -1, 0);
    Map_CopyCellsTo(30, 43, 32, 40, 8, 3);
    Map_CopyCellsTo(30, 43, 33, 39, 8, 1);
    Map_CopyCellsTo(30, 43, 36, 38, 3, 3);
    Map_CopyCellsTo(14, 41, 32, 41, 8, 4);
    Camera_MoveTo(0x23e0000, -1, 0x9e0000, 0);
    Map_Redraw();
    Actor_SetPosition(ACTOR_SUKURETA, 0x23e0000, 0x780000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Task_Wait(1);
    ColorBuffer_ApplyTarget(0x2051cc, 1);
    ColorBuffer_Interpolate(20);
    GameFlag_Set(0x201);
    GameFlag_Clear(0x200);
    GameFlag_Clear(0x202);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Event_OpenScreen();
    ((void (*)())Engine_EventWaitForScreen)();
    Event_Wait(40);
    Audio_PlayCue(171);
    ColorBuffer_ApplyTarget(0x10005, 1);
    ColorBuffer_Interpolate(8);
    Event_Wait(32);
    Engine_ColorBufferApplyTarget(0x2051cc, 1);
    ColorBuffer_Interpolate(24);
}

void Scene_SpringStatueTrap(void)
{
    s32 outer_pair;
    s32 second_pair;
    s32 middle_pair;
    s32 fourth_pair;
    s32 inner_pair;

    Event_Begin();
    FieldScene_PrepareStatueTransition();
    Event_SetMessage((s32)MsgSoruWhatsHappeningAtTheTrap);
    SetInitialScale(ACTOR_SUKURETA, 0x4000, 20);
    SetInitialDirection(ACTOR_SUKURETA, 256, 0);
    Actor_Jump(ACTOR_SUKURETA, 6, 30);
    Camera_MoveTo(37617664, -1, 11403264, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    SetSolShindenActorStep(32784, 20);
    for (outer_pair = 0; outer_pair != 4; outer_pair++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(12);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(12);
    }
    for (second_pair = 0; second_pair != 6; second_pair++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(8);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(8);
    }
    for (middle_pair = 0; middle_pair != 8; middle_pair++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(6);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(6);
    }
    for (fourth_pair = 0; fourth_pair != 10; fourth_pair++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(4);
        Engine_AudioPlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(4);
    }
    for (inner_pair = 0; inner_pair != 12; inner_pair++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(2);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(2);
    }
    SetStatueLightGroup1();
    Event_Wait(6);
    SetSolShindenActorStep(32784, 6);
    SetFinalScale(ACTOR_SUKURETA, 0x20000, 0x10000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 576, 280);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(FLAG_STATUE_TRAP_SPRUNG);
    Event_RequestExit(3);
}

void FieldScene_RunClosingSequence(void)
{
    s32 i;
    Event_Begin();
    FieldScene_PrepareStatueTransition();
    SoruNichigetsu_Light1Timer = 0;
    SoruNichigetsu_Light2Timer = 0;
    SoruNichigetsu_Light3Timer = 0;
    SoruNichigetsu_Light4Timer = 0;
    Event_SetMessage((s32)MsgSoruWhatsHappening);
    Actor_FaceDirection(ACTOR_SUKURETA, 16384, 20);
    Actor_ShowEmote(ACTOR_SUKURETA, 256, 0);
    Actor_Jump(ACTOR_SUKURETA, 6, 30);
    Camera_MoveTo(37617664, -1, 11403264, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    SetSolShindenActorStep(32784, 20);
    for (i = 0; i != 4; i++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(12);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(12);
    }
    SetSolShindenActorStep(32784, 6);
    SoruNichigetsu_Light1Timer = (((u32)Engine_RandomNext() * 60) >> 16) + 20;
    SoruNichigetsu_Light2Timer = (((u32)Engine_RandomNext() * 60) >> 16) + 20;
    SoruNichigetsu_Light3Timer = (((u32)Engine_RandomNext() * 60) >> 16) + 20;
    SoruNichigetsu_Light4Timer = (((u32)Engine_RandomNext() * 60) >> 16) + 20;
    SoruNichigetsu_FlashState = 0;
    Value2(Engine_TaskAddCallback, (s32)UpdateStatueLight1, 3200);
    Value2(Engine_TaskAddCallback, (s32)UpdateStatueLight2, 3200);
    Value2(Engine_TaskAddCallback, (s32)UpdateStatueLight3, 3200);
    Engine_TaskAddCallback((s32)UpdateStatueLight4, 3200);
    for (i = 0; i != 6; i++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(5);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(5);
    }
    for (i = 0; i != 8; i++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(4);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(4);
    }
    for (i = 0; i != 10; i++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(3);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(3);
    }
    for (i = 0; i != 12; i++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(2);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(2);
    }
    Map_CopyCellsTo(45, 30, 34, 10, 4, 2);
    Actor_Jump(ACTOR_SUKURETA, 6, 40);
    SetSolShindenActorStep(32784, 6);
    Actor_SetSpeed(ACTOR_SUKURETA, 131072, 65536);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 576, 280);
    Engine_TaskRemoveCallback((s32)UpdateStatueLight1);
    Engine_TaskRemoveCallback((s32)UpdateStatueLight2);
    Engine_TaskRemoveCallback((s32)UpdateStatueLight3);
    Engine_TaskRemoveCallback((s32)UpdateStatueLight4);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(4);
}

void FieldScene_RunFlaggedSequence(void)
{
    s32 base;
    s32 i6;
    s32 i7;
    s32 i8;
    s32 i9;
    s32 i10;

    Event_Begin();
    if (GameFlag_IsSet(2059) != 0 && GameFlag_IsSet(2086) != 0) {
        GameFlag_Clear(2086);
        Map_CopyCellsTo(45, 28, 34, 10, 2, 1);
    } else if (GameFlag_IsSet(2059) != 0 && GameFlag_IsSet(2086) == 0) {
        GameFlag_Set(2086);
    }
    if (GameFlag_IsSet(2060) != 0 && GameFlag_IsSet(2087) != 0) {
        GameFlag_Clear(2087);
        Map_CopyCellsTo(47, 28, 36, 10, 2, 1);
    } else if (GameFlag_IsSet(2060) != 0 && GameFlag_IsSet(2087) == 0) {
        GameFlag_Set(2087);
    }
    if (GameFlag_IsSet(2061) != 0 && GameFlag_IsSet(2088) != 0) {
        GameFlag_Clear(2088);
        Map_CopyCellsTo(45, 29, 34, 11, 2, 1);
    } else if (GameFlag_IsSet(2061) != 0 && GameFlag_IsSet(2088) == 0) {
        GameFlag_Set(2088);
    }
    if (GameFlag_IsSet(2062) != 0 && GameFlag_IsSet(2089) != 0) {
        GameFlag_Clear(2089);
        Map_CopyCellsTo(47, 29, 36, 11, 2, 1);
    } else if (GameFlag_IsSet(2062) != 0 && GameFlag_IsSet(2089) == 0) {
        GameFlag_Set(2089);
    }
    FieldScene_PrepareStatueTransition();
    Actor_FaceDirection(ACTOR_SUKURETA, 16384, 20);
    Actor_Jump(ACTOR_SUKURETA, 6, 30);
    Camera_MoveTo(37617664, -1, 11403264, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    for (i6 = 0; i6 != 4; i6++) {
        Audio_PlayCue(246);
        SetStatueLightGroup2();
        Event_Wait(12);
        Audio_PlayCue(246);
        SetStatueLightGroup4();
        Event_Wait(12);
    }
    for (i7 = 0; i7 != 6; i7++) {
        Audio_PlayCue(246);
        SetStatueLightGroup2();
        Event_Wait(8);
        Audio_PlayCue(246);
        SetStatueLightGroup4();
        Event_Wait(8);
    }
    for (i8 = 0; i8 != 8; i8++) {
        Audio_PlayCue(246);
        SetStatueLightGroup2();
        Event_Wait(6);
        Audio_PlayCue(246);
        SetStatueLightGroup4();
        Event_Wait(6);
    }
    for (i9 = 0; i9 != 10; i9++) {
        Audio_PlayCue(246);
        SetStatueLightGroup2();
        Event_Wait(4);
        Audio_PlayCue(246);
        SetStatueLightGroup4();
        Event_Wait(4);
    }
    for (i10 = 0; i10 != 12; i10++) {
        Audio_PlayCue(246);
        SetStatueLightGroup2();
        Event_Wait(2);
        Audio_PlayCue(246);
        SetStatueLightGroup4();
        Event_Wait(2);
    }
    Audio_PlayCue(246);
    SetStatueLightGroup2();
    Event_Wait(6);
    if (GameFlag_IsSet(2082) == 0) {
        base = 32784;
        Event_SetMessage((s32)MsgSoruYouFoundIt);
        SetSolShindenActorStep(base, 6);
        Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
        SetSolShindenActorStep(base, 6);
    }
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Event_CloseScreen();
    Engine_EventWaitForScreen();
    Event_RequestExit(5);
}

void Scene_ChangeLunaPictureToSol(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(FLAG_LUNA_PICTURE_CHANGED_TO_SOL) != 0) {
    } else {
        if (CheckAllStatueLights() == 0) {
        } else {
            Event_Begin();
            Actor_SetPosition(ACTOR_SUKURETA, 0x2410000, 0x930000);
            Actor_FaceDirection(ACTOR_SUKURETA, 0x4000, 1);
            Camera_MoveTo(0x23e0000, -1, 0xb80000, 1);
            Event_SetMessage((s32)MsgSoruThePictureOfLunaChanged);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x240, 232);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
            Camera_WaitForMove();
            Event_Wait(10);
            Actor_SetSpeed(ACTOR_SUKURETA, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 152);
            Event_Wait(6);
            Actor_Jump(ACTOR_SUKURETA, 6, 30);
            SetSolShindenActorStep(16, 6);
            Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
            Event_Wait(2);
            Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
            SetSolShindenActorStep(16, 6);
            Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
            Event_Wait(40);
            Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
            Event_Wait(30);
            SetSolShindenActorStep(16, 6);
            Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
            Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 184);
            Event_Wait(6);
            Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
            Event_Wait(40);
            SetSolShindenActorStep(0x4010, 6);
            Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 208);
            Event_Wait(40);
            Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
            Event_Wait(6);
            Actor_SetSpeed(ACTOR_SUKURETA, 0x8000, 0x4000);
            Actor_SetAnimation(ACTOR_SUKURETA, 2);
            record = Engine_ActorGet(0);
            if (record != 0) {
                Actor_SetDestination(ACTOR_SUKURETA, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Actor_WaitForMove(ACTOR_SUKURETA);
            Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
            GameFlag_Set(FLAG_LUNA_PICTURE_CHANGED_TO_SOL);
            Event_End();
        }
    }
}

void FieldScene_RunActorPositionTransition(void)
{
    u32 i;
    s32 record;

    Engine_AudioPlayCue(21);
    Call3(Engine_ActorWalkToAndWait, 0, 0x178, 184);
    Engine_ActorSetAnimation(0, 0);
    Call3(Engine_ActorSetPosition, 16, 0x1780000, 0xb80000);
    Call3(Engine_ActorSetSpeed, 16, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 16, 0x188, 168);
    Call3(Engine_ActorFaceDirection, 16, 0x8000, 30);
    Engine_ActorSetAnimation(16, 1);
    Event_SetMessage((s32)MsgSoruLookSymbolFloor);
    Engine_ActorJump(16, 4, 30);
    SetSolShindenActorStep(16, 6);
    Engine_ActorRunRepeatedMotion(0, 2);
    Engine_EventWait(6);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    SetSolShindenActorStep(16, 6);
    Call3(Engine_ActorWalkToAndWait, 16, 0x178, 184);
    Call3(Engine_ActorSetPosition, 16, 0x6480000, 0x6480000);
    Engine_EventWait(4);
    Engine_GameFlagSet(0x811);
}
