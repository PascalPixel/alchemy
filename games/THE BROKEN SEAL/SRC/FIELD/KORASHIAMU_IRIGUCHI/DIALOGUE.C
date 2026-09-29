#include "STATUS.H"

void SceneState_ForwardMaskedHalfwordWith10();

/* The Colosso entrance's talk with its visitors and competitors. */
void SceneDialogue_RunActor10MessageByFlag962(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x962)) {
        Event_SetMessage(MSG_COLOSSEUM_ALREADY_FULL_IF_REALLY);
        Event_ShowMessage(10, 0);
    } else {
        Event_SetMessage(MSG_HOPING_GET_IN_SEE_COLOSSO);
        Event_AskYesNo(10, 0);
    }
    Event_End();
}

void SceneDialogue_RunActor13MessageByFlag962(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x962)) {
        Actor_ShowEmote(13, 258, 40);
        Event_SetMessage(MSG_WANT_WATCH_FINALS_FROM_GOOD);
        Event_ShowMessage(13, 0);
    } else {
        Event_SetMessage(MSG_COLOSSOS_LUCKY_GUESS_MAKES_BATTLES);
        Event_ShowMessage(13, 0);
    }
    Event_End();
}

void FieldScene_RunScene3b9_02000334(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x962) != 0) {
        Actor_RunRepeatedMotion(14, 2);
        Event_SetMessage(MSG_HOLD_ON_SECOND);
        FieldScene_CallPairWith10(14);
        Actor_FaceEachOther(14, ACTOR_PARTY_LEADER, 0);
        Event_Wait(20);
        Event_AskYesNo(14, 0);
        SceneState_ForwardMaskedHalfwordWith10(14, 0);
    } else {
        Event_SetMessage(MSG_WANT_WIN_BIG_PUT_ON);
        Event_ShowMessage(14, 0);
    }
    Event_End();
}

void FieldScene_RunScene3b9_0200039c(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x962) != 0) {
        if (GameFlag_IsSet(0x3c0) != 0) {
            Event_SetMessage(MSG_LOOK_IM_SORRY_BUT_WE);
        } else {
            Event_SetMessage(MSG_WANT_TRY_OUT_LUCKY_GUESS);
            Event_OpenMessage(16, 0);
            if (Event_ChooseYesNo(0, 0) == 0) {
                bump_step(1);
                Actor_ShowEmote(16, 0x100, 40);
                Event_OpenMessage(16, 0);
                if (Event_ChooseYesNo(0, 0) == 0) {
                    bump_step(1);
                }
                ((void (*)())Engine_EventWait)(40);
                Event_ShowMessage(16, 0);
                GameFlag_Set(0x3c0);
                goto L_02000448;
            }
        }
        Event_ShowMessage(16, 0);
    } else {
        Event_SetMessage(MSG_DO_KNOW_LUCKY_GUESS);
        Event_AskYesNo(16, 0);
    }
    L_02000448:;
    Event_End();
}

void FieldScene_RunScene3b9_02000468(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 13);
    Event_Begin();
    Actor_Stop(13);
    Actor_FaceEachOther(13, ACTOR_PARTY_LEADER, 20);
    Event_SetMessage(MSG_IM_RATED_AS_SECOND_BEST);
    FieldScene_CallPairWith10(13);
    Actor_RunRepeatedMotion(13, 1);
    Event_ShowMessage(13, 0);
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
    Event_End();
}

void FieldScene_RunScene3b9_020004c8(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetAttachedEffect(14, 0x102);
    Actor_RunRepeatedMotion(14, 2);
    Event_SetMessage(MSG_WAAH_DONT_FRIGHTEN_ME);
    FieldScene_CallPairWith10(14);
    Actor_ShowEmote(14, 0x102, 40);
    Event_ShowMessage(14, 0);
    Event_End();
}

void SceneDialogue_ShowLine2118WithActor15Steps(void)
{
    Event_Begin();
    Event_SetMessage(MSG_WHO_HAVE_COME_QUESTION_ME);
    FieldScene_CallPairWith10(15);
    Actor_FaceEachOther(15, ACTOR_PARTY_LEADER, 20);
    FieldScene_CallPairWith10(15);
    Actor_SetAnimationAndWait(15, 3);
    Actor_SetAnimation(15, 0);
    FieldScene_CallPairWith10(15);
    SceneState_ForwardMaskedHalfwordWith10(15, 20480);
    Event_End();
}

void FieldScene_RunScene3b9_0200055c(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_RunRepeatedMotion(16, 2);
    Event_SetMessage(MSG_DEKKA_MUST_WIN_FINALS_DEKKA);
    Event_ShowMessageAndWait(16, 0, 20);
    if (GameFlag_IsSet(0x3c1) != 0) {
        Event_Wait(20);
    } else {
        SceneState_ForwardMaskedHalfwordWith10(17, 0);
        Actor_RunRepeatedMotion(17, 1);
        FieldScene_CallPairWith10(17);
        Actor_FaceEachOther(17, ACTOR_PARTY_LEADER, 20);
        Actor_SetAnimation(17, 4);
        FieldScene_CallPairWith10(17);
        Actor_ShowEmote(17, 0x105, 40);
        FieldScene_CallPairWith10(17);
        Call2(SceneState_ForwardMaskedHalfwordWith10, 17, 0x5000);
        GameFlag_Set(0x3c1);
    }
    Event_End();
}

void FieldScene_RunActorSeventeenDialogueSteps(void)
{
    Event_Begin();
    Actor_FaceEachOther(17, ACTOR_PARTY_LEADER, 20);
    Event_SetMessage(MSG_AM_NAVAMPA_GONDOWAN_SIXTH_RANKED);
    FieldScene_CallPairWith10(17);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(17, 3);
    FieldScene_CallPairWith10(17);
    Actor_RunRepeatedMotion(17, 1);
    FieldScene_CallPairWith10(17);
    SceneState_ForwardMaskedHalfwordWith10(17, 20480);
    Event_End();
}

void FieldScene_RunScene3b9_02000648(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_FaceEachOther(18, ACTOR_PARTY_LEADER, 20);
    Event_SetMessage(MSG_IM_BUFORD_SEVENTH_SEED);
    FieldScene_CallPairWith10(18);
    Actor_FaceDirection(18, 0xd000, 20);
    Actor_FaceDirection(18, 0xb000, 20);
    Actor_FaceDirection(18, 0x8000, 40);
    Actor_FaceEachOther(18, ACTOR_PARTY_LEADER, 20);
    FieldScene_CallPairWith10(18);
    Actor_SetAnimationAndWait(18, 3);
    FieldScene_CallPairWith10(18);
    Value2(SceneState_ForwardMaskedHalfwordWith10, 18, 0x5000);
    Event_End();
}

void FieldScene_RunScene3b9_020006bc(void)
{
    Event_Begin();
    Actor_FaceEachOther(8, ACTOR_PARTY_LEADER, 20);
    Event_SetMessage(MSG_YOU_READY_FOR_FINALS);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) != 0)
        bump_step_now(1);
    Event_ShowMessage(8, 0);
    Event_End();
}
