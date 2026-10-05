#include "STATUS.H"
#include "FIELD_SCENE.H"
#include "text/MSG_IDS.H"

extern const struct SceneEntrance gKorashiamuIriguchiEntrances1[];
extern const struct SceneEntrance gKorashiamuIriguchiEntrances3[];
extern const struct SceneEntrance gKorashiamuIriguchiEntrancesOther[];

extern const u32 gKorashiamuIriguchiExits[];

extern const struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance5[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance7[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance8[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance12[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance66[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements3Flag950[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements3Flag962[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements3[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacementsOther[];
extern const struct SceneEvent gKorashiamuIriguchiEvents2[];
extern const struct SceneEvent gKorashiamuIriguchiEvents1Entrance12[];
extern const struct SceneEvent gKorashiamuIriguchiEvents1[];
extern const struct SceneEvent gKorashiamuIriguchiEvents3[];
extern const struct SceneEvent gKorashiamuIriguchiEventsOther[];

void SceneState_ForwardMaskedHalfwordWith10();

const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KorashiamuIriguchi1) {
        return gKorashiamuIriguchiEntrances1;
    }
    if (scene == (s32)&SceneId_KorashiamuIriguchi3) {
        return gKorashiamuIriguchiEntrances3;
    }
    return gKorashiamuIriguchiEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gKorashiamuIriguchiExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    const struct ScenePlacement *table;

    if (gGameState.scene == (s32)&SceneId_KorashiamuIriguchi1) {
        switch (gGameState.entrance) {
        case 5:
        case 69:
            table = gKorashiamuIriguchiPlacements1Entrance5;
            break;
        case 7:
        case 70:
            table = gKorashiamuIriguchiPlacements1Entrance7;
            break;
        case 8:
        case 21:
        case 31:
        case 64:
        case 65:
        case 67:
            table = gKorashiamuIriguchiPlacements1Entrance8;
            break;
        case 12:
            table = gKorashiamuIriguchiPlacements1Entrance12;
            break;
        case 66:
        case 68:
            table = gKorashiamuIriguchiPlacements1Entrance66;
            break;
        default:
            table = gKorashiamuIriguchiPlacements1;
            break;
        }
    } else if (gGameState.scene == (s32)&SceneId_KorashiamuIriguchi3) {
        if (Engine_GameFlagIsSet(0x950) != 0) {
            table = gKorashiamuIriguchiPlacements3Flag950;
        } else if (Engine_GameFlagIsSet(0x962) != 0) {
            table = gKorashiamuIriguchiPlacements3Flag962;
        } else {
            table = gKorashiamuIriguchiPlacements3;
        }
    } else {
        table = gKorashiamuIriguchiPlacementsOther;
    }
    return table;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KorashiamuIriguchi2) {
        return gKorashiamuIriguchiEvents2;
    }
    if (scene == (s32)&SceneId_KorashiamuIriguchi1) {
        if (gGameState.entrance == 12) {
            return gKorashiamuIriguchiEvents1Entrance12;
        }
        return gKorashiamuIriguchiEvents1;
    }
    if (scene == (s32)&SceneId_KorashiamuIriguchi3) {
        return gKorashiamuIriguchiEvents3;
    }
    return gKorashiamuIriguchiEventsOther;
}

void SceneDialogue_RunActor10MessageByFlag962(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x962)) {
        Engine_EventSetMessage((s32)MsgKorashiamuColosseumAlreadyFullIfReally);
        Engine_EventShowMessage(10, 0);
    } else {
        Engine_EventSetMessage((s32)MsgKorashiamuHopingGetInSeeColosso);
        Engine_EventAskYesNo(10, 0);
    }
    Engine_EventEnd();
}

void SceneDialogue_RunActor13MessageByFlag962(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x962)) {
        Engine_ActorShowEmote(13, 258, 40);
        Engine_EventSetMessage((s32)MsgKorashiamuWantWatchFinalsFromGood);
        Engine_EventShowMessage(13, 0);
    } else {
        Engine_EventSetMessage((s32)MsgKorashiamuColossosLuckyGuessMakesBattles);
        Engine_EventShowMessage(13, 0);
    }
    Engine_EventEnd();
}

void KorashiamuIriguchi_TalkToActorFourteen(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x962) != 0) {
        Engine_ActorRunRepeatedMotion(14, 2);
        Engine_EventSetMessage((s32)MsgKorashiamuHoldOnSecond);
        FieldScene_CallPairWith10(14);
        Engine_ActorFaceEachOther(14, ACTOR_PARTY_LEADER, 0);
        Engine_EventWait(20);
        Engine_EventAskYesNo(14, 0);
        SceneState_ForwardMaskedHalfwordWith10(14, 0);
    } else {
        Engine_EventSetMessage((s32)MsgKorashiamuWantWinBigPutOn);
        Engine_EventShowMessage(14, 0);
    }
    Engine_EventEnd();
}

void KorashiamuIriguchi_TryLuckyGuess(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x962) != 0) {
        if (GameFlag_IsSet(0x3c0) != 0) {
            Engine_EventSetMessage((s32)MsgKorashiamuLookImSorryButWe);
        } else {
            Engine_EventSetMessage((s32)MsgKorashiamuWantTryOutLuckyGuess);
            Event_OpenMessage(16, 0);
            if (Engine_EventChooseYesNo(0, 0) == 0) {
                gEventWork->message++;
                Actor_ShowEmote(16, 0x100, 40);
                Event_OpenMessage(16, 0);
                if (Engine_EventChooseYesNo(0, 0) == 0) {
                    gEventWork->message++;
                }
                Engine_EventWait(40);
                Event_ShowMessage(16, 0);
                GameFlag_Set(0x3c0);
                goto L_02000448;
            }
        }
        Event_ShowMessage(16, 0);
    } else {
        Engine_EventSetMessage((s32)MsgKorashiamuDoKnowLuckyGuess);
        Event_AskYesNo(16, 0);
    }
    L_02000448:;
    Engine_EventEnd();
}

void KorashiamuIriguchi_IntroduceSecondSeed(void)
{
    u32 i;
    struct FieldActor *rec7;
    s32 record;

    rec7 = Object_GetById(13);
    Engine_EventBegin();
    Engine_ActorStop(13);
    Engine_ActorFaceEachOther(13, ACTOR_PARTY_LEADER, 20);
    Engine_EventSetMessage((s32)MsgKorashiamuImRatedAsSecondBest);
    FieldScene_CallPairWith10(13);
    Engine_ActorRunRepeatedMotion(13, 1);
    Engine_EventShowMessage(13, 0);
    {
        u16 *target = (u16 *)&rec7->timer;
        s32 shown = 0x2d0;

        *target = shown;
    }
    {
        u16 *target = (u16 *)&rec7->work;
        s32 shown = 112;

        *target = shown;
    }
    Engine_ActorEnableActionCallback(13, 2);
    Engine_EventEnd();
}

void KorashiamuIriguchi_FrightenActorFourteen(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_SetAttachedEffect(14, 0x102);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventSetMessage((s32)MsgKorashiamuWaahDontFrightenMe);
    FieldScene_CallPairWith10(14);
    Actor_ShowEmote(14, 0x102, 40);
    Event_ShowMessage(14, 0);
    Engine_EventEnd();
}

void SceneDialogue_ShowLine2118WithActor15Steps(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKorashiamuWhoHaveComeQuestionMe);
    FieldScene_CallPairWith10(15);
    Engine_ActorFaceEachOther(15, ACTOR_PARTY_LEADER, 20);
    FieldScene_CallPairWith10(15);
    Engine_ActorSetAnimationAndWait(15, 3);
    Engine_ActorSetAnimation(15, 0);
    FieldScene_CallPairWith10(15);
    SceneState_ForwardMaskedHalfwordWith10(15, 20480);
    Engine_EventEnd();
}

void KorashiamuIriguchi_CheerForDekka(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(16, 2);
    Engine_EventSetMessage((s32)MsgKorashiamuDekkaMustWinFinalsDekka);
    Event_ShowMessageAndWait(16, 0, 20);
    if (GameFlag_IsSet(0x3c1) != 0) {
        Engine_EventWait(20);
    } else {
        SceneState_ForwardMaskedHalfwordWith10(17, 0);
        Engine_ActorRunRepeatedMotion(17, 1);
        FieldScene_CallPairWith10(17);
        Engine_ActorFaceEachOther(17, ACTOR_PARTY_LEADER, 20);
        Engine_ActorSetAnimation(17, 4);
        FieldScene_CallPairWith10(17);
        Actor_ShowEmote(17, 0x105, 40);
        FieldScene_CallPairWith10(17);
        SceneState_ForwardMaskedHalfwordWith10(17, 0x5000);
        GameFlag_Set(0x3c1);
    }
    Engine_EventEnd();
}

void FieldScene_RunActorSeventeenDialogueSteps(void)
{
    Engine_EventBegin();
    Engine_ActorFaceEachOther(17, ACTOR_PARTY_LEADER, 20);
    Engine_EventSetMessage((s32)MsgKorashiamuAmNavampaGondowanSixthRanked);
    FieldScene_CallPairWith10(17);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(17, 3);
    FieldScene_CallPairWith10(17);
    Engine_ActorRunRepeatedMotion(17, 1);
    FieldScene_CallPairWith10(17);
    SceneState_ForwardMaskedHalfwordWith10(17, 20480);
    Engine_EventEnd();
}

void KorashiamuIriguchi_IntroduceSeventhSeed(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_ActorFaceEachOther(18, ACTOR_PARTY_LEADER, 20);
    Engine_EventSetMessage((s32)MsgKorashiamuImBufordSeventhSeed);
    FieldScene_CallPairWith10(18);
    Actor_FaceDirection(18, 0xd000, 20);
    Actor_FaceDirection(18, 0xb000, 20);
    Actor_FaceDirection(18, FACING_WEST, 40);
    Engine_ActorFaceEachOther(18, ACTOR_PARTY_LEADER, 20);
    FieldScene_CallPairWith10(18);
    Engine_ActorSetAnimationAndWait(18, 3);
    FieldScene_CallPairWith10(18);
    SceneState_ForwardMaskedHalfwordWith10(18, 0x5000);
    Engine_EventEnd();
}

void KorashiamuIriguchi_AskReadyForFinals(void)
{
    Engine_EventBegin();
    Engine_ActorFaceEachOther(8, ACTOR_PARTY_LEADER, 20);
    Engine_EventSetMessage((s32)MsgKorashiamuYouReadyForFinals);
    Engine_EventOpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0)
        gEventWork->message++;
    Engine_EventShowMessage(8, 0);
    Engine_EventEnd();
}
