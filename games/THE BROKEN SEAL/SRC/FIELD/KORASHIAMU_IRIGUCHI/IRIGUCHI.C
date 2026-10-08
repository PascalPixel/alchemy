#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "text/MSG_IDS.H"
#include "CALL.H"
#include "FIELDOBJ.H"

#define RECORD_17 17

extern u8 KorashiamuIriguchi_ActionTable1[];
extern u8 KorashiamuIriguchi_ActionTable2[];
extern u8 KorashiamuIriguchi_ActionTable3[];
extern u8 KorashiamuIriguchi_ActionTable4[];
extern u8 KorashiamuIriguchi_ActionTable5[];
extern u8 KorashiamuIriguchi_ActionTable6[];
extern u8 KorashiamuIriguchi_ActionTable7[];
extern u8 KorashiamuIriguchi_ActionTable8[];
extern u8 KorashiamuIriguchi_ActionTable9[];

void Party_AddActiveOwner();
void Party_RemoveActiveOwner();
void Object_SetActionCallbackAndRefreshById();
void Object_SetActionById();
void InventorySnapshot_Take();

void FieldScene_RunMiddleSequence(void);

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
	void SceneState_ForwardMaskedHalfwordWith10();
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
        u16 shown = 0x2d0;

        *target = shown;
    }
    {
        u16 *target = (u16 *)&rec7->work;
        u16 shown = 112;

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
	void SceneState_ForwardMaskedHalfwordWith10();
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
	void SceneState_ForwardMaskedHalfwordWith10();
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
	void SceneState_ForwardMaskedHalfwordWith10();
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
	void SceneState_ForwardMaskedHalfwordWith10();
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

void Scene_RunBranchingActorPresentation(void)
{
	void FieldScene_CallPairWith10();
	void SceneState_ForwardMaskedHalfwordWith10();
    s32 record;

    if (Engine_GameFlagIsSet(0x962) == 0) {
        return;
    }

    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(17, 1);
    Actor_FaceDirection(17, 0x3000, 20);
    Engine_ActorFaceDirection(17, FACING_EAST, 60);
    Actor_ShowEmote(17, 0x100, 40);
    SceneState_ForwardMaskedHalfwordWith10(17, 0x3000);
    Engine_EventSetMessage((s32)MsgKorashiamuHeyYouRe);
    Engine_ActorStartRepeatedMotion(17, 2);
    FieldScene_CallPairWith10(17);
    Engine_ActorFaceDirection(18, 0x3000, 0);
    Engine_ActorFaceDirection(19, 0x3000, 0);
    Engine_ActorFaceDirection(20, 0x3000, 0);
    Engine_EventGetViewCenter()->motion_flags = 0;
    Engine_CameraSetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x1000000, -1, 0xac0000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Actor_ShowEmote(18, 0x102, 40);
    FieldScene_CallPairWith10(18);
    Engine_ActorSetAnimationAndWait(17, 3);
    FieldScene_CallPairWith10(17);
    Actor_SetAttachedEffect(19, 0x102);
    Engine_EventWait(40);
    SceneState_ForwardMaskedHalfwordWith10(19, 0);
    FieldScene_CallPairWith10(19);
    Actor_ShowEmote(20, 0x103, 40);
    SceneState_ForwardMaskedHalfwordWith10(20, 0);
    Engine_ActorStartRepeatedMotion(20, 2);
    Engine_EventShowMessageAndWait(20, 0, 20);
    Engine_ActorRunRepeatedMotion(17, 1);
    Engine_EventWait(10);
    Actor_FaceDirection(17, 0xb000, 20);
    Engine_EventShowMessageAndWait(17, 0, 20);
    Engine_ActorRunRepeatedMotion(18, 1);
    Engine_ActorSetAnimationAndWait(18, 4);
    Engine_EventWait(20);
    SceneState_ForwardMaskedHalfwordWith10(17, 0x8000);
    Engine_EventShowMessageAndWait(17, 0, 20);
    Engine_ActorRunRepeatedMotion(19, 1);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(19, 4);
    Engine_EventShowMessageAndWait(17, 0, 20);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(20, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(17, 3);
    Engine_ActorSetAnimation(18, 3);
    Engine_ActorSetAnimation(19, 3);
    Engine_ActorSetAnimationAndWait(20, 3);
    Engine_ActorFaceDirection(17, 0x3000, 0);
    Engine_ActorFaceDirection(18, 0x3000, 0);
    Engine_ActorFaceDirection(19, 0x3000, 0);
    SceneState_ForwardMaskedHalfwordWith10(20, 0x3000);
    Actor_SetSpeed(17, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(17, 0x102, 172);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_WalkToAndWait(0, 0x106, 188);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(1, ((struct FieldActor *)record)->x.fixed, ((struct FieldActor *)record)->z.fixed);
    }
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(2, ((struct FieldActor *)record)->x.fixed, ((struct FieldActor *)record)->z.fixed);
    }
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(3, ((struct FieldActor *)record)->x.fixed, ((struct FieldActor *)record)->z.fixed);
    }
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    Actor_SetSpeed(2, 0xcccc, 0x6666);
    Actor_SetSpeed(3, 0xcccc, 0x6666);
    Engine_ActorWalkTo(1, 246, 200);
    Actor_WalkTo(2, 0x106, 200);
    Actor_WalkToAndWait(3, 0x116, 200);
    Engine_ActorSetAnimation(2, 1);
    Engine_ActorSetAnimation(1, 1);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Actor_FaceDirection(2, FACING_NORTH, 0);
    Actor_FaceDirection(3, FACING_NORTH, 40);
    Engine_ActorRunRepeatedMotion(17, 1);
    FieldScene_CallPairWith10(17);
    Engine_ActorStartRepeatedMotion(0, 2);
    Engine_ActorStartRepeatedMotion(1, 2);
    Engine_ActorStartRepeatedMotion(2, 2);
    Engine_ActorRunRepeatedMotion(3, 2);
    Engine_ActorSetAnimationAndWait(17, 3);
    /* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
    Event_OpenMessage(17, 0);
    Actor_FaceDirection(1, FACING_NORTHEAST, 0);
    Actor_FaceDirection(3, FACING_NORTHWEST, 0);

    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(17, 3);
        FieldScene_CallPairWith10(17);
        Engine_ActorEnableActionCallback(1, KorashiamuIriguchi_ActionTable1);
        Engine_ActorEnableActionCallback(2, KorashiamuIriguchi_ActionTable1);
        Object_SetActionCallbackAndRefreshById(3, (s32)KorashiamuIriguchi_ActionTable1);
        Engine_CameraSetSpeed(0x6666, 0xccc);
        Engine_CameraMoveTo(0x1000000, -1, 0x640000, 1);
        Actor_SetSpeed(17, 0x10000, 0x8000);
        Engine_ActorEnableActionCallback(17, KorashiamuIriguchi_ActionTable4);
        Engine_EventWait(10);
        Actor_SetSpeed(0, 0x10000, 0x8000);
        Engine_ActorEnableActionCallback(0, KorashiamuIriguchi_ActionTable4);
        Engine_EventWait(80);
        gEventWork->transition_frames = 40;
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
    } else {
        Event_SkipMessages(1);
        Actor_ShowEmote(17, 0x102, 40);
        FieldScene_CallPairWith10(17);
        Actor_ShowEmote(18, 0x102, 40);
        FieldScene_CallPairWith10(18);
        Actor_ShowEmote(19, 0x102, 40);
        FieldScene_CallPairWith10(19);
        Actor_ShowEmote(20, 0x102, 40);
        FieldScene_CallPairWith10(19);
        Engine_ActorSetAnimationAndWait(17, 4);
        FieldScene_CallPairWith10(17);
        Engine_ActorRunRepeatedMotion(2, 1);
        Engine_EventWait(20);
        FieldScene_CallPairWith10(2);
        Actor_ShowEmote(3, 0x102, 40);
        FieldScene_CallPairWith10(3);
        Actor_FaceDirection(17, 0xb000, 0);
        Engine_ActorFaceDirection(19, FACING_WEST, 0);
        Engine_ActorFaceDirection(20, FACING_EAST, 60);
        Engine_ActorFaceDirection(17, 0x3000, 0);
        Engine_ActorFaceDirection(19, 0x3000, 0);
        Engine_ActorFaceDirection(20, 0x3000, 20);
        Engine_ActorSetAnimation(17, 3);
        Engine_ActorSetAnimation(18, 3);
        Engine_ActorSetAnimation(19, 3);
        Engine_ActorSetAnimationAndWait(20, 3);
        Actor_ShowEmote(3, 0x100, 60);
        Call2(SceneState_ForwardMaskedHalfwordWith10, 3, 0xa000);
        FieldScene_CallPairWith10(3);
        Engine_ActorSetAnimation(1, 4);
        Engine_EventWait(20);
        /* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
        Event_OpenMessage(1, 0);

        record = Engine_EventChooseYesNo(0, 0);
        if (record == 0) {
            Engine_ActorSetAnimationAndWait(17, 3);
            FieldScene_CallPairWith10(17);
            Engine_ActorEnableActionCallback(1, KorashiamuIriguchi_ActionTable1);
            Engine_ActorEnableActionCallback(2, KorashiamuIriguchi_ActionTable1);
            Object_SetActionCallbackAndRefreshById(3, (s32)KorashiamuIriguchi_ActionTable1);
            Engine_CameraSetSpeed(0x6666, 0xccc);
            Engine_CameraMoveTo(0x1000000, -1, 0x640000, 1);
            Actor_SetSpeed(17, 0x10000, 0x8000);
            Engine_ActorEnableActionCallback(17, KorashiamuIriguchi_ActionTable4);
            Engine_EventWait(10);
            Actor_SetSpeed(0, 0x10000, 0x8000);
            Engine_ActorEnableActionCallback(0, KorashiamuIriguchi_ActionTable4);
            Engine_EventWait(80);
            gEventWork->transition_frames = 40;
            Engine_EventCloseScreen();
            Engine_EventWaitForScreen();
        } else {
            Event_SkipMessages(1);
            Engine_EventWait(20);
            Engine_ActorStartRepeatedMotion(1, 2);
            Engine_EventShowMessageAndWait(1, 0, 20);
            Engine_ActorShowEmote(2, 0x102, 60);
            FieldScene_CallPairWith10(2);
            SceneState_ForwardMaskedHalfwordWith10(3, 0x8000);
            Engine_ActorSetAnimation(3, 3);
            FieldScene_CallPairWith10(3);
            Engine_ActorEnableActionCallback(2, KorashiamuIriguchi_ActionTable2);
            Object_SetActionCallbackAndRefreshById(3, (s32)KorashiamuIriguchi_ActionTable2);
            Engine_EventWait(20);
            Object_SetActionCallbackAndRefreshById(0, (s32)KorashiamuIriguchi_ActionTable2);
            Actor_WalkToAndWait(1, 0x106, 188);
            SceneState_ForwardMaskedHalfwordWith10(1, 0xc000);
            Engine_ActorSetAnimationAndWait(1, 3);
            FieldScene_CallPairWith10(1);
            Engine_CameraSetSpeed(0x6666, 0xccc);
            Engine_CameraMoveTo(0x1000000, -1, 0x640000, 1);
            Actor_SetSpeed(17, 0x10000, 0x8000);
            Engine_ActorEnableActionCallback(17, KorashiamuIriguchi_ActionTable4);
            Engine_EventWait(10);
            Actor_SetSpeed(1, 0x10000, 0x8000);
            Engine_ActorEnableActionCallback(1, KorashiamuIriguchi_ActionTable4);
            Engine_EventWait(80);
            gEventWork->transition_frames = 40;
            Engine_EventCloseScreen();
            Engine_EventWaitForScreen();
        }
    }

    Engine_EventRequestExit(2);
    Engine_GameFlagSet(0x93f);
    Engine_EventEnd();
}

void KorashiamuIriguchi_RunGatherScene(void)
{
	void FieldScene_CallPairWith10();
	void SceneState_ForwardMaskedHalfwordWith10();
    struct FieldActor *actor;
    s32 script;

    if (Engine_GameFlagIsSet(0x234) != 0) {
        Engine_GameFlagSet(0x235);
        Engine_EventBegin();
        Actor_SetSpeed(0, 0x10000, 0x8000);
        Actor_WalkToAndWait(0, 0x368, 120);
        SceneState_ForwardMaskedHalfwordWith10(0, 0xa000);
        Camera_SetSpeed(0x19999, 0x3333);
        Camera_MoveTo(0x3300000, -1, 0x700000, 1);
        Engine_CameraWaitForMove();
        Actor_FaceDirection(8, 0x3000, 20);
        Engine_ActorRunRepeatedMotion(8, 1);
        Engine_EventSetMessage((s32)MsgKorashiamuYouReadyForFinals);
        /* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
        Event_OpenMessage(0x8008, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventWait(20);
            SceneState_ForwardMaskedHalfwordWith10(12, 0xd000);
            Engine_ActorRunRepeatedMotion(12, 1);
            Engine_EventSetMessage((s32)MsgKorashiamuDayTiredWaiting);
            FieldScene_CallPairWith10(0x400c);
            SceneState_ForwardMaskedHalfwordWith10(17, 0);
            SceneState_ForwardMaskedHalfwordWith10(0, 0x8000);
            Engine_ActorSetAnimationAndWait(17, 3);
            Engine_ActorSetAnimationAndWait(0, 3);
            Engine_ActorSetAnimation(15, 1);
            Engine_ActorFaceDirection(15, 0xd000, 20);
            Engine_ActorRunRepeatedMotion(15, 1);
            FieldScene_CallPairWith10(15);
            SceneState_ForwardMaskedHalfwordWith10(16, 0x8000);
            Engine_ActorSetAnimationAndWait(16, 3);
            Engine_ActorRunRepeatedMotion(17, 2);
            SceneState_ForwardMaskedHalfwordWith10(17, 0xa000);
            FieldScene_CallPairWith10(0x4011);
            SceneState_ForwardMaskedHalfwordWith10(18, 0xb000);
            Engine_ActorStartRepeatedMotion(18, 2);
            FieldScene_CallPairWith10(0x4012);
            SceneState_ForwardMaskedHalfwordWith10(11, 0x3000);
            Engine_ActorSetAttachedEffect(11, 0x102);
            Engine_EventWait(60);
            Engine_ActorStartRepeatedMotion(11, 2);
            FieldScene_CallPairWith10(0x800b);
            Engine_ActorStop(13);
            Engine_TaskWait(1);
            Engine_ActorStartRepeatedMotion(13, 2);
            Engine_ActorStartRepeatedMotion(14, 2);
            Engine_ActorRunRepeatedMotion(16, 2);
            Engine_EventWait(20);
            Engine_ActorFaceDirection(13, FACING_EAST, 0);
            Engine_ActorFaceDirection(14, FACING_WEST, 0);
            Engine_ActorFaceDirection(16, 0xb000, 40);
            Engine_ActorSetAnimation(13, 3);
            Engine_ActorSetAnimation(14, 3);
            Engine_ActorSetAnimationAndWait(16, 3);
            Actor_SetSpeed(13, 0x19999, 0xcccc);
            Actor_SetSpeed(14, 0x19999, 0xcccc);
            Actor_SetSpeed(16, 0x19999, 0xcccc);
            Engine_ActorEnableActionCallback(13, KorashiamuIriguchi_ActionTable5);
            Engine_ActorEnableActionCallback(16, KorashiamuIriguchi_ActionTable7);
            Engine_EventWait(20);
            Engine_ActorFaceDirection(15, 0xd000, 0);
            Engine_ActorFaceDirection(17, 0xb000, 0);
            Engine_ActorFaceDirection(0, FACING_NORTHWEST, 0);
            Engine_ActorFaceDirection(12, 0xd000, 0);
            Engine_ActorFaceDirection(18, 0xb000, 0);
            Object_SetActionCallbackAndRefreshById(14, (s32)KorashiamuIriguchi_ActionTable6);
            Engine_EventWait(20);
            Engine_ActorFaceDirection(8, FACING_EAST, 0);
            Engine_ActorFaceDirection(11, FACING_WEST, 40);
            Engine_ActorSetAnimation(8, 3);
            Engine_ActorSetAnimationAndWait(11, 3);
            Engine_CameraMoveTo(0x3280000, -1, 0x560000, 1);
            Engine_CameraWaitForMove();
            Engine_ActorRunRepeatedMotion(8, 2);
            FieldScene_CallPairWith10(8);
            Engine_ActorSetSpeed(8, 0x10000, 0x8000);
            Object_GetById(8)->steering_flags &= ~ACTOR_FACES_MOTION;
            Engine_ActorWalkToAndWait(8, 0x318, 72);
            Engine_EventWait(1);
            Object_GetById(8)->steering_flags |= ACTOR_FACES_MOTION;
            SceneState_ForwardMaskedHalfwordWith10(8, 0);
            Engine_ActorStop(13);
            actor = Object_GetById(13);
            actor->update = NULL;
            actor->timer = 0;
            actor->work = 0;
            actor->velocity_x = 0;
            actor->velocity_y = 0;
            actor->velocity_z = 0;
            actor->target_x = ACTOR_NO_TARGET;
            actor->target_y = ACTOR_NO_TARGET;
            actor->target_z = ACTOR_NO_TARGET;
            Engine_TaskWait(1);
            script = (s32)KorashiamuIriguchi_ActionTable8;
            Engine_ActorEnableActionCallback(15, script);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(13, script);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(17, script);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(14, script);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(16, script);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(12, script);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(18, script);
            Engine_EventWait(60);
            Engine_ActorEnableActionCallback(0, script);
            Engine_EventWait(80);
            Engine_EventRequestExit(66);
        } else {
            gEventWork->message++;
            Engine_EventShowMessage(0x8008, 0);
        }
        Engine_EventEnd();
    }
}

void SceneState_ApplyFlags565And564(void)
{
    Engine_GameFlagClear(0x235);
    Engine_GameFlagSet(564);
}

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
    Actor_FaceDirection(16, FACING_NORTH, 20);
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
    Actor_FaceDirection(17, FACING_EAST, 0);
    Actor_FaceDirection(18, FACING_WEST, 20);
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

void KorashiamuIriguchi_ShowQualifyingTrialMessage(void)
{

    Engine_EventBegin();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);

    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();

    Engine_EventWait(20);
    SceneState_ForwardMaskedHalfwordWith10(17, 160 << 7);
    Engine_EventSetMessage((s32)MsgKorashiamuHaveMakeThroughCountlessMatches);

    if (Engine_GameFlagIsSet(0x8a4) != 0) {
        gEventWork->message =
            (u16)(gEventWork->message + 1);
    }

    FieldScene_CallPairWith10(17);
    SceneState_ForwardMaskedHalfwordWith10(17, 192 << 6);
    Engine_GameFlagSet(0x8a3);

    Engine_EventEnd();
}

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
    Actor_FaceDirection(17, FACING_NORTH, 20);
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
    Actor_FaceDirection(18, FACING_WEST, 20);
    Actor_SetAttachedEffect(14, 0x102);
    Actor_SetAttachedEffect(15, 0x102);
    Actor_SetAttachedEffect(17, 0x102);
    Actor_SetAttachedEffect(18, 0x102);
    Engine_EventWait(60);
    Actor_FaceDirection(14, 0x3000, 0);
    Actor_FaceDirection(15, 0x3000, 0);
    SceneState_ForwardMaskedHalfwordWith10(18, 0x3000);
    Actor_SetAttachedEffect(17, 0x101);
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
        if (Engine_GameFlagIsSet(0x109) != 0)
            break;
        KorashiamuIriguchi_ShowQualifyingTrialMessage();
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
        KorashiamuIriguchi_EscortToEastExit();
        break;
    case 66:
        KorashiamuIriguchi_WalkToEastExit();
        break;
    case 12:
        Engine_GameFlagSet(324);
        ActorPresentation_SetActorsTwelveToEighteen();
        if (Engine_GameFlagIsSet(0x109) != 0)
            break;
        KorashiamuIriguchi_AnnounceFinals();
        break;
    case 21:
        Party_AddActiveOwner(1);
        Party_AddActiveOwner(2);
        Party_AddActiveOwner(3);
        Engine_GameFlagSet(0x90e);
        KorashiamuIriguchi_CelebrateAdvancement();
        break;
    case 67:
        KorashiamuIriguchi_EscortToWestExit();
        break;
    case 68:
        KorashiamuIriguchi_WalkToWestExit();
        break;
    case 31:
        Party_AddActiveOwner(1);
        Party_AddActiveOwner(2);
        Party_AddActiveOwner(3);
        Engine_GameFlagSet(0x90f);
        KorashiamuIriguchi_CelebrateBothWins();
        break;
    default:
        break;
    }
}

void FieldScene_CallPairWith10(s32 no)
{
    Engine_EventShowMessage(no, 0);
    Engine_EventWait(10);
}

void SceneState_ForwardMaskedHalfwordWith10(s32 arg0, s32 arg1)
{
    Engine_ActorFaceDirection(arg0, (u16)arg1, 10);
}

void SceneState_ApplyRectsByFlatla384And962(void)
{
    s32 pair;
    s32 a, b;

    if (Engine_GameFlagIsSet(2384) != 0) {
        pair = 2;
        Engine_MapCopyCellsTo(64, 0, 48, 5, pair, pair);
        a = 16;
        b = 8;
        Engine_MapCopyCellAttributes(14, 8, 2, 1, a, b);
    } else {
        Engine_ActorSetChildValue(16, 2);
        if (Engine_GameFlagIsSet(0x962) != 0) {
            a = 14;
            b = 11;
            Engine_MapCopyCellAttributes(30, 22, 1, 2, a, b);
        }
    }
}

void FieldScene_BuildActorPresentationSequence(void)
{
    s32 flag;

    Engine_EventBegin();
    Actor_SetPosition(GERALD, 0x3180000, 0x880000);
    Actor_SetPosition(IWAN, 0x3380000, 0x880000);
    Actor_SetPosition(MEARI, 0x3280000, 0x980000);
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
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventWait(40);
    SceneState_ForwardMaskedHalfwordWith10(8, 0x3000);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(40);
    Engine_EventWait(80);
    Actor_ShowEmote(IWAN, 0x100, 0);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgKorashiamuRobinOnlyOneEnteringFinals);
    FieldScene_CallPairWith10(2);
    Engine_ActorSetAnimationAndWait(8, 3);
    FieldScene_CallPairWith10(8);
    Actor_ShowEmote(MEARI, 0x102, 40);
    FieldScene_CallPairWith10(3);
    Actor_FaceDirection(8, FACING_WEST, 0);
    Actor_FaceDirection(9, FACING_EAST, 40);
    Actor_FaceDirection(8, 0x5000, 0);
    SceneState_ForwardMaskedHalfwordWith10(9, 0x3000);
    Actor_ShowEmote(9, 0x105, 20);
    FieldScene_CallPairWith10(9);
    Actor_ShowEmote(GERALD, 0x103, 20);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    FieldScene_CallPairWith10(1);
    Actor_ShowEmote(10, 0x102, 40);
    FieldScene_CallPairWith10(10);
    Engine_ActorSetAnimationAndWait(11, 3);
    FieldScene_CallPairWith10(11);
    SceneState_ForwardMaskedHalfwordWith10(2, 0xa000);
    Engine_ActorSetAnimationAndWait(IWAN, 4);
    FieldScene_CallPairWith10(2);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    FieldScene_CallPairWith10(3);
    Actor_ShowEmote(GERALD, 0x103, 20);
    SceneState_ForwardMaskedHalfwordWith10(1, 0);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    FieldScene_CallPairWith10(1);
    SceneState_ForwardMaskedHalfwordWith10(0, 0x6000);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    SceneState_ForwardMaskedHalfwordWith10(2, 0x8000);
    FieldScene_CallPairWith10(2);
    Engine_ActorSetAnimation(MEARI, 4);
    Engine_EventWait(20);
    FieldScene_CallPairWith10(3);
    Engine_ActorRunRepeatedMotion(GERALD, 1);
    SceneState_ForwardMaskedHalfwordWith10(1, 0x2000);
    FieldScene_CallPairWith10(1);
    Actor_ShowEmote(IWAN, 0x105, 60);
    Actor_FaceDirection(IWAN, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(GERALD, FACING_EAST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(GERALD, 0x102, 0);
    Actor_ShowEmote(MEARI, 0x102, 80);
    Actor_ShowEmote(IWAN, 0x106, 0);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    SceneState_ForwardMaskedHalfwordWith10(2, 0xc000);
    FieldScene_CallPairWith10(2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(MEARI, FACING_NORTH, 20);
    SceneState_ForwardMaskedHalfwordWith10(8, 0x3000);
    Engine_ActorSetAnimationAndWait(8, 3);
    FieldScene_CallPairWith10(8);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(GERALD, 0x100, 0);
    Actor_ShowEmote(MEARI, 0x100, 40);
    Engine_ActorRunRepeatedMotion(9, 1);
    FieldScene_CallPairWith10(9);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 40);
    Engine_ActorRunRepeatedMotion(10, 1);
    FieldScene_CallPairWith10(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    SceneState_ForwardMaskedHalfwordWith10(3, 0xc000);
    Engine_ActorSetAnimationAndWait(11, 3);
    FieldScene_CallPairWith10(11);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 20);
    Actor_ShowEmote(GERALD, 0x105, 0);
    Actor_ShowEmote(IWAN, 0x105, 0);
    Actor_ShowEmote(MEARI, 0x105, 80);
    Actor_ShowEmote(IWAN, 0x106, 0);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    SceneState_ForwardMaskedHalfwordWith10(2, 0xe000);
    FieldScene_CallPairWith10(2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(GERALD, FACING_EAST, 20);
    Actor_ShowEmote(11, 0x101, 60);
    Event_OpenMessage(IWAN, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTH, 0);
    flag = 0;
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        FieldScene_CallPairWith10(2);
        flag = 1;
    } else {
        gEventWork->message++;
        Engine_ActorSetAnimationAndWait(IWAN, 3);
        SceneState_ForwardMaskedHalfwordWith10(2, 0xc000);
        FieldScene_CallPairWith10(2);
    }
    if (flag != 0) {
        gEventWork->message++;
    }
    Actor_FaceDirection(8, FACING_WEST, 0);
    Actor_FaceDirection(9, FACING_EAST, 0);
    Actor_FaceDirection(10, 0x3000, 0);
    Actor_FaceDirection(11, 0xb000, 20);
    Actor_ShowEmote(8, 0x105, 0);
    Actor_ShowEmote(9, 0x105, 0);
    Actor_ShowEmote(10, 0x105, 0);
    Actor_ShowEmote(11, 0x105, 60);
    Engine_ActorRunRepeatedMotion(8, 1);
    SceneState_ForwardMaskedHalfwordWith10(8, 0x3000);
    FieldScene_CallPairWith10(8);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    Actor_FaceDirection(MEARI, FACING_NORTH, 20);
    Engine_ActorRunRepeatedMotion(9, 1);
    SceneState_ForwardMaskedHalfwordWith10(9, 0x3000);
    FieldScene_CallPairWith10(9);
    Engine_ActorRunRepeatedMotion(10, 1);
    SceneState_ForwardMaskedHalfwordWith10(10, 0x5000);
    FieldScene_CallPairWith10(10);
    SceneState_ForwardMaskedHalfwordWith10(11, 0x8000);
    Engine_ActorSetAnimationAndWait(11, 3);
    FieldScene_CallPairWith10(11);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 20);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Actor_SetSpeed(GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(IWAN, 0x10000, 0x8000);
    Actor_SetSpeed(MEARI, 0x10000, 0x8000);
    Engine_ActorEnableActionCallback(GERALD, (s32)KorashiamuIriguchi_ActionTable3);
    Engine_ActorEnableActionCallback(IWAN, (s32)KorashiamuIriguchi_ActionTable3);
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

void KorashiamuIriguchi_EscortToEastExit(void)
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

void KorashiamuIriguchi_AnnounceFinals(void)
{
    u32 i;
    struct FieldActor *rec7;
    s32 record;

    rec7 = Object_GetById(13);
    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_ActorStop(13);
    Engine_TaskWait(1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
    Engine_ActorSetAnimation(13, 1);
    Actor_FaceDirection(12, 0xd000, 0);
    Actor_FaceDirection(13, FACING_EAST, 0);
    Actor_FaceDirection(14, FACING_WEST, 0);
    Actor_FaceDirection(15, 0xd000, 0);
    Actor_FaceDirection(16, FACING_WEST, 0);
    Actor_FaceDirection(17, 0xb000, 0);
    Actor_FaceDirection(18, 0xb000, 0);
    Engine_EventSetMessage((s32)MsgKorashiamuPrepareYourselvesContestantsFinalsWill);
    FieldScene_CallPairWith10(8);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    {
        u16 *target = (u16 *)&rec7->timer;
        u16 shown = 0x2d0;

        *target = shown;
    }
    {
        u16 *target = (u16 *)&rec7->work;
        u16 shown = 112;

        *target = shown;
    }
    Engine_ActorEnableActionCallback(13, 2);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(15, 0x5000, 0);
    Actor_FaceDirection(16, FACING_EAST, 0);
    Actor_FaceDirection(17, 0x5000, 0);
    Actor_FaceDirection(18, 0x5000, 0);
    Engine_EventEnd();
}

void KorashiamuIriguchi_WalkToEastExit(void)
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

void KorashiamuIriguchi_CelebrateAdvancement(void)
{
    u32 i;
    s32 record;
    s32 base5_200adac;

    Engine_EventBegin();
    Actor_SetPosition(GERALD, 0x3180000, 0x880000);
    Actor_SetPosition(IWAN, 0x3380000, 0x880000);
    Actor_SetPosition(MEARI, 0x3280000, 0x980000);
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
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 20);
    Actor_SetSpeed(GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(IWAN, 0x10000, 0x8000);
    Actor_SetSpeed(MEARI, 0x10000, 0x8000);
    base5_200adac = (s32)KorashiamuIriguchi_ActionTable3;
    Engine_ActorEnableActionCallback(GERALD, base5_200adac);
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

void KorashiamuIriguchi_EscortToWestExit(void)
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

void KorashiamuIriguchi_WalkToWestExit(void)
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

void KorashiamuIriguchi_CelebrateBothWins(void)
{
    u32 i;
    s32 record;
    s32 base5_200adac;

    Engine_EventBegin();
    Actor_SetPosition(GERALD, 0x3180000, 0x880000);
    Actor_SetPosition(IWAN, 0x3380000, 0x880000);
    Actor_SetPosition(MEARI, 0x3280000, 0x980000);
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
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 20);
    Actor_SetSpeed(GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(IWAN, 0x10000, 0x8000);
    Actor_SetSpeed(MEARI, 0x10000, 0x8000);
    base5_200adac = (s32)KorashiamuIriguchi_ActionTable3;
    Engine_ActorEnableActionCallback(GERALD, base5_200adac);
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
    Engine_ActorSetChildValue(12, 3);
    Engine_ActorSetChildValue(13, 0);
    Engine_ActorSetChildValue(14, 4);
    Engine_ActorSetChildValue(15, 1);
    Engine_ActorSetChildValue(16, 5);
    Engine_ActorSetChildValue(17, 2);
    Engine_ActorSetChildValue(18, 6);
    Object_SetActionById(13, 10);
    Object_SetActionById(14, 20);
    Engine_ActorSetAnimation(15, 0);
    Object_SetActionById(16, 40);
    Object_SetActionById(17, 50);
    Object_SetActionById(18, 60);
}
