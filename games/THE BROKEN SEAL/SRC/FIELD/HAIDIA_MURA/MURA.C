#include "STAGED_MOTION.H"
#include "TYPES.H"
#include "CALL.H"

extern u8 MsgHaidiaDoYouNeedToGo[];
extern u8 MsgHaidiaDontGoBeyondSukuretasCottage[];
extern u8 MsgHaidiaIFeltAnotherOne[];
extern u8 MsgHaidiaIToldGeraldItWas[];
extern u8 MsgHaidiaIllGetYouForMy[];
extern u8 MsgHaidiaIsJasmineBackYet[];
extern u8 MsgHaidiaItWontRainForSome[];
extern u8 MsgHaidiaNoTravelersSinceTheEruption[];
extern u8 MsgHaidiaSukuretaHasntComeBack[];
extern u8 MsgHaidiaSukuretaIsWaitingForUs[];
extern u8 MsgHaidiaTheGroundStillShakes[];
extern u8 MsgHaidiaYouCantBeRobin[];
extern u8 MsgHaidiaYouMakeMeSoMad[];
extern u8 MsgHaidiaYourGrandpaIsTheMayor[];

extern u8 MsgHaidiaPuppiesPlayingOver[];
extern u8 MsgHaidiaRrruffRrrruff[];
extern struct MapRenderWork *gMapWork;
s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Battle_WaitMode0();
void ObjectMotion_SetSpeedParameters();
void Engine_MessageShowCentered();
void Engine_MapRedraw();
void Object_SetModeById();
void Engine_EventEnd();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_ActorWalkToAndWait();

extern u8 MsgHaidiaNotSneakingUpMtAleph[];

s32 MapStagedScene_SelectPrimaryData(void)
{
    return (s32)gHaidiaMuraEntrances;
}

s32 MapStagedScene_GetEmptyData(void)
{
    return 0;
}

s32 MapStagedScene_SelectSecondaryData(void)
{
    return (s32)gHaidiaMuraExits;
}

s32 MapStagedScene_SelectTertiaryData(void)
{
    u8 *scene_state = (u8 *)&gGameState;
    if (*(s16 *)(scene_state + 0x1c2) == 16)
        return (s32)gHaidiaMuraPlacements4;
    if (GameFlag_IsSet(0x87a) != 0)
        return (s32)gHaidiaMuraPlacements3;
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0)
        return (s32)gHaidiaMuraPlacements2;
    return (s32)gHaidiaMuraPlacements;
}

void SceneDialogue_RunActor181Scene(void)
{
    Event_Begin();
    Actor_SetPosition(26, 0, 0);
    GameFlag_Set(0xfd0);
    Item_ShowFound(ITEM_NUT, 3);
    Party_GiveItem(ITEM_NUT, 0);
    Event_End();
}

void FieldScene_RunActor181Scene(void)
{
    Event_Begin();
    Actor_SetPosition(20, 0, 0);
    GameFlag_Set(0xfd0);
    Item_ShowFound(ITEM_NUT, 3);
    Party_GiveItem(ITEM_NUT, 0);
    Event_End();
}

s32 MapStagedScene_SelectQuaternaryData(void)
{
    if (GameFlag_IsSet(0x87a) != 0)
        return (s32)gHaidiaMuraEvents3;
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0)
        return (s32)gHaidiaMuraEvents2;
    return (s32)gHaidiaMuraEvents;
}

void SceneDialogue_RunActorTenFlaggedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Event_SetMessage((s32)MsgHaidiaTheGroundStillShakes);
        Event_ShowMessage(10, 0);
    } else {
        Event_SetMessage((s32)MsgHaidiaYourGrandpaIsTheMayor);
        Actor_FaceEachOther(10, ACTOR_PARTY_LEADER, 4);
        Event_AskYesNo(10, 0);
    }
    Event_End();
}

void SceneDialogue_RunActorFourteenTalk(void)
{
    s32 flag = 0x806;
    Event_Begin();
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Event_SetMessage((s32)MsgHaidiaSukuretaHasntComeBack);
        Event_ShowMessage(14, 0);
    } else if (GameFlag_IsSet(flag) == 0) {
        GameFlag_Set(flag);
        Event_SetMessage((s32)MsgHaidiaDoYouNeedToGo);
        Actor_FaceEachOther(14, ACTOR_PARTY_LEADER, 4);
        Event_AskYesNo(14, 0);
    } else {
        Event_SetMessage((s32)MsgHaidiaDontGoBeyondSukuretasCottage);
        Actor_FaceEachOther(14, ACTOR_PARTY_LEADER, 4);
        Event_ShowMessage(14, 0);
    }
    Event_End();
}

void FieldScene_RunFlag807BranchSequence(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x807) == 0) {
        GameFlag_Set(0x807);
        Event_SetMessage((s32)MsgHaidiaYouMakeMeSoMad);
        Actor_ShowEmote(18, 0x103, 0);
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, 18, 20);
        Event_ShowMessageAndWait(18, 0, 6);
        Actor_FaceDirection(18, 0x8000, 30);
        Actor_Jump(18, 2, 20);
        Event_ShowMessageAndWait(18, 0, 6);
        Actor_FaceEachOther(18, ACTOR_PARTY_LEADER, 10);
        Actor_ShowEmote(18, 0x103, 0);
        Event_ShowMessageAndWait(18, 0, 10);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    } else {
        Actor_ShowEmote(18, 0x103, 0);
        Event_SetMessage((s32)MsgHaidiaIllGetYouForMy);
        Event_ShowMessageAndWait(18, 0, 20);
    }
    Event_End();
}

void SceneDialogue_RunActor21FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x202) != 0) {
        Event_SetMessage((s32)MsgHaidiaIToldGeraldItWas);
    } else {
        Event_SetMessage((s32)MsgHaidiaItWontRainForSome);
    }
    Event_ShowMessage(21, 0);
    Event_End();
}

void SceneDialogue_RunActor10LineAndFlag81f(void)
{
    Event_Begin();
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 10, 20);
    Event_SetMessage((s32)MsgHaidiaIsJasmineBackYet);
    Event_ShowMessage(10, 0);
    GameFlag_Set(0x81f);
    Event_End();
}

void FieldScene_RunScene373_02000cd0(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Task_Wait(10);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_SetMessage((s32)MsgHaidiaIFeltAnotherOne);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_FaceActor(17, ACTOR_PARTY_LEADER, 20);
    Event_ShowMessage(17, 0);
    Event_End();
}

void SceneDialogue_RunActorNineteenDialogue(void)
{
    Event_Begin();
    Actor_RunRepeatedMotion(19, 2);
    Event_Wait(20);
    Actor_FaceActor(19, ACTOR_PARTY_LEADER, 20);
    Event_SetMessage((s32)MsgHaidiaNoTravelersSinceTheEruption);
    Event_AskYesNo(19, 0);
    GameFlag_Set(0x307);
    Event_End();
}

void SceneState_Send210AndApplyRectAt40x84(void)
{
    s32 m, n;
    GameFlag_Set(0x210);
    m = 10;
    n = 84;
    Map_CopyCellAttributes(40, 84, 7, 4, m, n);
}

void SceneState_Send210AndApplyRect(void)
{
    s32 m, n;
    GameFlag_Clear(0x210);
    m = 10;
    n = 84;
    Map_CopyCellAttributes(40, 89, 7, 4, m, n);
}

void FieldScene_RunScene373_02000dc0(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(188);
    Map_AnimateCells((s32)gHaidiaMuraCellAnimC, 45, 11);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x101, 0x1a4);
    Event_RequestExit(11);
}

void SceneState_ApplyFlag801Branch(void)
{
    if (GameFlag_IsSet(0x801) == 0) {
        FieldScene_RunScene373SequenceC();
    } else {
        Audio_PlayCue(123);
        Event_RequestExit(1);
    }
}

void SceneState_SetValue123Mode3(void)
{
    Audio_PlayCue(123);
    Event_RequestExit(3);
}

void SceneState_SetValue123Mode4(void)
{
    Audio_PlayCue(123);
    Event_RequestExit(4);
}

void SceneState_ApplyValues123And2(void)
{
    Audio_PlayCue(123);
    Event_RequestExit(2);
}

void FieldScene_RunScene373_02000e54(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells((u32)gHaidiaMuraCellAnimA, 54, 32);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x196, 0x2d7);
    Event_RequestExit(5);
}

void FieldScene_RunScene373_02000e84(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells((u32)gHaidiaMuraCellAnimB, 45, 39);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x106, 0x325);
    Event_RequestExit(6);
}

void SceneDialogue_RunFlag815GatedStep(void)
{
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0 && GameFlag_IsSet(0x87a) == 0) {
        Event_Begin();
        Event_SetMessage((s32)MsgHaidiaYouCantBeRobin);
        Event_OpenMessage(21, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_ShowMessageAndWait(21, 0, 60);
            Event_ShowMessage(21, 0);
        } else {
            u8 *b = *(u8 **)&gEventWork;
            u16 *h = (u16 *)(b + 0x1d8);
            *h = *h + 2;
            Event_Wait(40);
            Event_ShowMessage(21, 0);
        }
        Event_End();
    } else {
        Audio_PlayCue(0x9e);
        Map_AnimateCells((s32)gHaidiaMuraCellAnimA, 50, 44);
        Actor_WalkTo(ACTOR_PARTY_LEADER, 0x154, 0x378);
        Event_RequestExit(7);
    }
}

/* Runs four scene primitives in sequence: one single-argument call, one call
 * passing the address of gHaidiaMuraCellAnimB plus two small constants, one call
 * with a byte-flag-sized first argument (0) and two larger constants, and a
 * final single-argument call. */
void FieldScene_RunPrimarySequence(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)gHaidiaMuraCellAnimB, 49, 69); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 326, 0x466); /* object_id 0, x 326, z 0x466 */
    Event_RequestExit(8); /* main:0808a248 */
}

/* Runs a short scripted step, then two 3-argument setup calls, then another
 * short scripted step; none of the callees' effects are visible here. */
void FieldScene_RunScene373SequenceA(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)gHaidiaMuraCellAnimD, 52, 76); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x176, 0x4d6); /* object_id 0, x 0x176, z 0x4d6 */
    Event_RequestExit(9); /* main:0808a248 */
}

void FieldScene_RunPrimarySequenceSecond(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)gHaidiaMuraCellAnimA, 35, 74); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 0x4b6);
    Event_RequestExit(10); /* main:0808a248 */
}

void FieldScene_RunScene373SequenceC(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec8 = Object_GetById(ACTOR_PARTY_LEADER);
    rec7 = Object_GetById(ACTOR_JASMINE);
    Event_Begin();
    *(s32 *)(rec7 + 8) = *(s32 *)(rec8 + 8);
    *(s32 *)(rec7 + 12) = *(s32 *)(rec8 + 12);
    *(s32 *)(rec7 + 16) = *(s32 *)(rec8 + 16);
    *(s32 *)(rec7 + 56) = -0x80000000;
    *(s32 *)(rec7 + 60) = -0x80000000;
    *(s32 *)(rec7 + 64) = -0x80000000;
    *(s32 *)(rec7 + 36) = 0;
    *(s32 *)(rec7 + 40) = 0;
    *(s32 *)(rec7 + 44) = 0;
    *(s32 *)(rec7 + 20) = *(s32 *)(rec8 + 12);
    Task_Wait(1);
    Actor_SetSpeed(ACTOR_JASMINE, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_JASMINE, 110, 0x11b);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JASMINE, 2);
    Event_SetMessage((s32)MsgHaidiaSukuretaIsWaitingForUs);
    if (*(s32 *)(rec8 + 8) < *(s32 *)(rec7 + 8)) {
        Event_ShowMessageAndWait(0xa005, 0, 2);
    } else {
        Event_ShowMessageAndWait(0x8005, 0, 2);
    }
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(2);
    Actor_SetAnimation(ACTOR_JASMINE, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_JASMINE, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_JASMINE);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 110, 0x12f);
    Event_End();
}

/* Until flag 0x808 is set, raise the camera 40 steps above an actor while
 * MsgHaidiaRrruffRrrruff plays, show MsgHaidiaPuppiesPlayingOver, lower it again and return the
 * camera to its target. */
void HaidiaMura_RunCameraRiseScene(void)
{
    s32 **cam;
    s32 *saved;
    s32 *rec;
    s32 n;
    s32 pos[3];

    if (Engine_GameFlagIsSet(0x808) == 0) {
        cam = *(s32 ***)&gMapWork;
        Engine_EventBegin();
        Call3(ObjectMotion_SetSpeedParameters, 0, 0x10000, 0x8000);
        Object_SetModeById(0, 1);
        Battle_WaitMode0(2);
        Engine_EventSetMessage((s32)MsgHaidiaRrruffRrrruff);
        Engine_EventShowMessageAndWait(15, 0, 2);
        Engine_EventShowMessageAndWait(16, 0, 2);
        rec = (s32 *)Object_GetById(0);
        pos[0] = rec[2];
        pos[1] = rec[3];
        pos[2] = rec[4];
        saved = *cam;
        *cam = pos;
        n = 0;
        do {
            pos[2] += 0x20000;
            Battle_WaitMode0(1);
            n++;
            Engine_MapRedraw();
        } while (n != 40);
        Battle_WaitMode0(60);
        Engine_MessageShowCentered((s32)MsgHaidiaPuppiesPlayingOver, 1);
        n = 0;
        Battle_WaitMode0(6);
        do {
            pos[2] -= 0x20000;
            Battle_WaitMode0(1);
            n++;
            Engine_MapRedraw();
        } while (n != 40);
        *cam = saved;
        Battle_WaitMode0(60);
        Call3(Engine_ActorWalkToAndWait, 0, 70, 0x2e5);
        Engine_EventEnd();
    }
}

void FieldScene_RunPuppyBarks(void)
{
    s32 msg;

    if (GameFlag_IsSet(0x808) == 0) {
        Event_Begin();
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
        msg = (s32)MsgHaidiaRrruffRrrruff;
        Event_SetMessage(msg);
        Event_ShowMessageAndWait(15, 0, 2);
        Event_ShowMessageAndWait(16, 0, 2);
        Message_ShowCentered(msg + 2, 1);
        Event_Wait(6);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 69, 0x366);
        Event_End();
    }
}

void SceneState_RunFlag204Step(void)
{
    s32 m, n;
    Event_Begin();
    m = 20;
    n = 50;
    Map_CopyCellAttributes(49, 53, 8, 4, m, n);
    HaidiaMura_RunWalkScene032B0(0, 10, 11, 1);
    GameFlag_Set(0x204);
    Event_End();
}

void SceneState_SetFlag204AndConfigureRegion49_46(void)
{
    s32 p5, p6;
    Event_Begin();
    HaidiaMura_RunWalkScene03380(0, 13, 10, 1);
    GameFlag_Clear(0x204);
    p5 = 20;
    p6 = 50;
    Map_CopyCellAttributes(49, 46, 8, 4, p5, p6);
    Event_End();
}

void FieldScene_RunScene373SequenceE(void)
{
    u32 i;
    u8 *rec7;
    s32 record;

    rec7 = Object_GetById(22);
    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x20000);
    Actor_Jump(ACTOR_PARTY_LEADER, 5, 0);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 215, 0x193);
    rec7[90] |= 1;
    Actor_SetPosition(22, 0xa60000, 0x1770000);
    Actor_FaceDirection(22, 0x2000, 20);
    rec7[90] = (rec7[90] ^ 1);
    Actor_SetSpeed(22, 0x28000, 0x28000);
    Actor_Jump(22, 4, 0);
    Actor_WalkToAndWait(22, 202, 0x18b);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
    Actor_FaceDirection(22, 0x3000, 24);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    Actor_SetSpeed(22, 0x18000, 0x10000);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, gHaidiaMuraLeaderWalkActions);
    Event_Wait(10);
    Actor_ShowEmote(22, 0x103, 0);
    Actor_EnableActionCallback(22, gHaidiaMuraActor22WalkActions);
    Object_RefreshSelectorById(0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x100, 0x1da);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Object_RefreshSelectorById(22);
    Actor_WalkToAndWait(22, 0x100, 0x1c8);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(22, 0x4000, 20);
    Actor_StartRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_SetMessage((s32)MsgHaidiaNotSneakingUpMtAleph);
    Event_AskYesNo(22, 0);
    record = Actor_Get(22);
    *(s32 *)(record + 108) = (s32)SceneActor_RunStep18WhenTargetSet;
    Actor_EnableActionCallback(22, gHaidiaMuraActor22Actions);
    GameFlag_Set(0x823);
    Event_End();
}

void SceneState_RunTablePairWhenActor22State1(void)
{
    u8 *p = Actor_Get(22);
    if (GameFlag_IsSet(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 1) {
            FieldScene_RunScene373_02001490((s32)gHaidiaMuraPairTableA, (s32)gHaidiaMuraPairTableB);
        }
    }
}

void FieldScene_RunScene373_02001490(s32 a0, s32 a1)
{
    u32 i;
    s32 p8;
    s32 rec8;
    s32 record;

    p8 = a1;
    rec8 = Object_GetById(22);
    Event_Begin();
    Actor_StartRepeatedMotion(22, 2);
    Actor_ShowEmote(22, 0x100, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 40);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, a0);
    Event_Wait(10);
    Actor_ShowEmote(22, 0x103, 0);
    Object_SetActionCallbackAndRefreshById(22, p8);
    Object_RefreshSelectorById(0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(22, 2);
    *(s32 *)(rec8 + 24) = 0x10000;
    *(s32 *)(rec8 + 28) = 0x10000;
    record = Object_GetById(ACTOR_PARTY_LEADER);
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;
    Event_SetMessage((s32)MsgHaidiaNotSneakingUpMtAleph);
    Event_AskYesNo(22, 0);
    record = Actor_Get(22);
    *(s32 *)(record + 108) = (s32)SceneActor_RunStep18WhenTargetSet;
    Actor_EnableActionCallback(22, gHaidiaMuraActor22Actions);
    Event_End();
}

void SceneState_RunTablePairWhenActor22State2(void)
{
    u8 *p = Actor_Get(22);
    if (GameFlag_IsSet(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 2) {
            FieldScene_RunScene373_02001490((s32)gHaidiaMuraPairTableC, (s32)gHaidiaMuraPairTableD);
        }
    }
}

void SceneState_RunTablePairByActor22State(void)
{
    u8 *rec = Actor_Get(22);
    if (GameFlag_IsSet(0x823) != 0) {
        u8 *q = rec;
        s32 v;
        q += 100;
        v = *(s16 *)q;
        if (v == 1) {
            FieldScene_RunScene373_02001490((s32)gHaidiaMuraPairTableC, (s32)gHaidiaMuraPairTableB);
        } else if (v == 2) {
            FieldScene_RunScene373_02001490((s32)gHaidiaMuraPairTableC, (s32)gHaidiaMuraPairTableD);
        }
    }
}
