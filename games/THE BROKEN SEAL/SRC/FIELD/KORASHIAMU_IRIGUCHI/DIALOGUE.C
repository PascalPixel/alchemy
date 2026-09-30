#include "STATUS.H"
extern u8 MsgKorashiamuAmNavampaGondowanSixthRanked[];
extern u8 MsgKorashiamuColosseumAlreadyFullIfReally[];
extern u8 MsgKorashiamuColossosLuckyGuessMakesBattles[];
extern u8 MsgKorashiamuDekkaMustWinFinalsDekka[];
extern u8 MsgKorashiamuDoKnowLuckyGuess[];
extern u8 MsgKorashiamuHoldOnSecond[];
extern u8 MsgKorashiamuHopingGetInSeeColosso[];
extern u8 MsgKorashiamuImBufordSeventhSeed[];
extern u8 MsgKorashiamuImRatedAsSecondBest[];
extern u8 MsgKorashiamuLookImSorryButWe[];
extern u8 MsgKorashiamuWaahDontFrightenMe[];
extern u8 MsgKorashiamuWantTryOutLuckyGuess[];
extern u8 MsgKorashiamuWantWatchFinalsFromGood[];
extern u8 MsgKorashiamuWantWinBigPutOn[];
extern u8 MsgKorashiamuWhoHaveComeQuestionMe[];
extern u8 MsgKorashiamuYouReadyForFinals[];

void SceneState_ForwardMaskedHalfwordWith10();

/* The Colosso entrance's talk with its visitors and competitors. */
void SceneDialogue_RunActor10MessageByFlag962(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x962)) {
        Event_SetMessage((s32)MsgKorashiamuColosseumAlreadyFullIfReally);
        Event_ShowMessage(10, 0);
    } else {
        Event_SetMessage((s32)MsgKorashiamuHopingGetInSeeColosso);
        Event_AskYesNo(10, 0);
    }
    Event_End();
}

void SceneDialogue_RunActor13MessageByFlag962(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x962)) {
        Actor_ShowEmote(13, 258, 40);
        Event_SetMessage((s32)MsgKorashiamuWantWatchFinalsFromGood);
        Event_ShowMessage(13, 0);
    } else {
        Event_SetMessage((s32)MsgKorashiamuColossosLuckyGuessMakesBattles);
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
        Event_SetMessage((s32)MsgKorashiamuHoldOnSecond);
        FieldScene_CallPairWith10(14);
        Actor_FaceEachOther(14, ACTOR_PARTY_LEADER, 0);
        Event_Wait(20);
        Event_AskYesNo(14, 0);
        SceneState_ForwardMaskedHalfwordWith10(14, 0);
    } else {
        Event_SetMessage((s32)MsgKorashiamuWantWinBigPutOn);
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
            Event_SetMessage((s32)MsgKorashiamuLookImSorryButWe);
        } else {
            Event_SetMessage((s32)MsgKorashiamuWantTryOutLuckyGuess);
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
        Event_SetMessage((s32)MsgKorashiamuDoKnowLuckyGuess);
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

    rec7 = Object_GetById(13);
    Event_Begin();
    Actor_Stop(13);
    Actor_FaceEachOther(13, ACTOR_PARTY_LEADER, 20);
    Event_SetMessage((s32)MsgKorashiamuImRatedAsSecondBest);
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
    Event_SetMessage((s32)MsgKorashiamuWaahDontFrightenMe);
    FieldScene_CallPairWith10(14);
    Actor_ShowEmote(14, 0x102, 40);
    Event_ShowMessage(14, 0);
    Event_End();
}

void SceneDialogue_ShowLine2118WithActor15Steps(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgKorashiamuWhoHaveComeQuestionMe);
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
    Event_SetMessage((s32)MsgKorashiamuDekkaMustWinFinalsDekka);
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
        SceneState_ForwardMaskedHalfwordWith10(17, 0x5000);
        GameFlag_Set(0x3c1);
    }
    Event_End();
}

void FieldScene_RunActorSeventeenDialogueSteps(void)
{
    Event_Begin();
    Actor_FaceEachOther(17, ACTOR_PARTY_LEADER, 20);
    Event_SetMessage((s32)MsgKorashiamuAmNavampaGondowanSixthRanked);
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
    Event_SetMessage((s32)MsgKorashiamuImBufordSeventhSeed);
    FieldScene_CallPairWith10(18);
    Actor_FaceDirection(18, 0xd000, 20);
    Actor_FaceDirection(18, 0xb000, 20);
    Actor_FaceDirection(18, 0x8000, 40);
    Actor_FaceEachOther(18, ACTOR_PARTY_LEADER, 20);
    FieldScene_CallPairWith10(18);
    Actor_SetAnimationAndWait(18, 3);
    FieldScene_CallPairWith10(18);
    SceneState_ForwardMaskedHalfwordWith10(18, 0x5000);
    Event_End();
}

void FieldScene_RunScene3b9_020006bc(void)
{
    Event_Begin();
    Actor_FaceEachOther(8, ACTOR_PARTY_LEADER, 20);
    Event_SetMessage((s32)MsgKorashiamuYouReadyForFinals);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) != 0)
        bump_step_now(1);
    Event_ShowMessage(8, 0);
    Event_End();
}
