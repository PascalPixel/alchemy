#include "RUNTIME_MEM.H"
/* Near miss: the ES/IT sprite-priority direct call loads r0 before r1; the full resource differs in those four instruction bytes. */
#include "STAGED_MOTION.H"
#include "TYPES.H"
#include "CALL.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "IWRAM_CALL.H"

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

extern u8 MsgHaidiaAsStubbornAsYourFather[];
extern u8 MsgHaidiaDevastatedWhenKyle[];
extern u8 MsgHaidiaGoodJob[];
extern u8 MsgHaidiaWorkingYourselvesBone[];

enum HouseActor {
    ACTOR_BOARD = 23,
    ACTOR_LAST_BOARD = 24
};

enum {
    ANIM_HAMMER = 11
};

void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode);

/*
 * An actor climbs between the yard and the ledge in front of the house at
 * x 392, playing one animation for each half of the climb.
 */
void HaidiaMura_RunWalkScene032B0(s32 actor, s32 animation, s32 next_animation, s32 grounded);
void HaidiaMura_RunWalkScene03380(s32 actor, s32 animation, s32 next_animation, s32 grounded);
void PaletteGlow_Update(s32 a, s32 b);
void FieldScene_RunLargeStagingSequence(void);
void HaidiaMura_OpenVillagerLane(void);
void Scene_RepairTheHouse(void);
void SceneState_Send210AndApplyRectAt40x84(void);
void BattleFx_SetQueuedSoundAndPlay(s32 value);
void SceneActor_SetFlagByteBySlotZeroPosition(void);
void FieldScene_RunScene373SequenceB(void);
s32 SceneActor_RunStep18WhenTargetSet();
extern u8 gHaidiaMuraActor22Actions[];

extern u8 MsgHaidiaAh[];
extern u8 MsgHaidiaEverProtectFamily[];
extern u8 MsgHaidiaHowHaveYouBeen[];
extern u8 MsgHaidiaIUsedToPlayHere[];
void Engine_ActorSetSpeed();
void Engine_ActorFaceDirection();
void Engine_ActorSetAnimation();
void Engine_ActorMoveToAndWait();
void Engine_EventWait();

struct Flags38 {
    u8 pad[38];
    u8 flags;
};

struct Flags85 {
    u8 pad[85];
    u8 flags;
};

extern u8 MsgHaidiaRepairCaption[];

s32 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position);
u16 ArcTan2(s32 z, s32 x);

extern u8 MsgHaidiaDoorWontOpen[];

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
    Engine_EventBegin();
    Actor_SetPosition(26, 0, 0);
    GameFlag_Set(0xfd0);
    Engine_ItemShowFound(ITEM_NUT, 3);
    Engine_PartyGiveItem(ITEM_NUT, 0);
    Engine_EventEnd();
}

void FieldScene_RunActor181Scene(void)
{
    Engine_EventBegin();
    Actor_SetPosition(20, 0, 0);
    GameFlag_Set(0xfd0);
    Engine_ItemShowFound(ITEM_NUT, 3);
    Engine_PartyGiveItem(ITEM_NUT, 0);
    Engine_EventEnd();
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
    Engine_EventBegin();
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaTheGroundStillShakes);
        Event_ShowMessage(10, 0);
    } else {
        Engine_EventSetMessage((s32)MsgHaidiaYourGrandpaIsTheMayor);
        Engine_ActorFaceEachOther(10, ACTOR_PARTY_LEADER, 4);
        Event_AskYesNo(10, 0);
    }
    Engine_EventEnd();
}

void SceneDialogue_RunActorFourteenTalk(void)
{
    s32 flag = 0x806;
    Engine_EventBegin();
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaSukuretaHasntComeBack);
        Event_ShowMessage(14, 0);
    } else if (GameFlag_IsSet(flag) == 0) {
        GameFlag_Set(flag);
        Engine_EventSetMessage((s32)MsgHaidiaDoYouNeedToGo);
        Engine_ActorFaceEachOther(14, ACTOR_PARTY_LEADER, 4);
        Event_AskYesNo(14, 0);
    } else {
        Engine_EventSetMessage((s32)MsgHaidiaDontGoBeyondSukuretasCottage);
        Engine_ActorFaceEachOther(14, ACTOR_PARTY_LEADER, 4);
        Event_ShowMessage(14, 0);
    }
    Engine_EventEnd();
}

void FieldScene_RunFlag807BranchSequence(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x807) == 0) {
        GameFlag_Set(0x807);
        Engine_EventSetMessage((s32)MsgHaidiaYouMakeMeSoMad);
        Actor_ShowEmote(18, 0x103, 0);
        Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 18, 20);
        Event_ShowMessageAndWait(18, 0, 6);
        Actor_FaceDirection(18, 0x8000, 30);
        Engine_ActorJump(18, 2, 20);
        Event_ShowMessageAndWait(18, 0, 6);
        Engine_ActorFaceEachOther(18, ACTOR_PARTY_LEADER, 10);
        Actor_ShowEmote(18, 0x103, 0);
        Event_ShowMessageAndWait(18, 0, 10);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    } else {
        Actor_ShowEmote(18, 0x103, 0);
        Engine_EventSetMessage((s32)MsgHaidiaIllGetYouForMy);
        Event_ShowMessageAndWait(18, 0, 20);
    }
    Engine_EventEnd();
}

void SceneDialogue_RunActor21FlaggedLine(void)
{
    Engine_EventBegin();
    if (GameFlag_IsSet(0x202) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaIToldGeraldItWas);
    } else {
        Engine_EventSetMessage((s32)MsgHaidiaItWontRainForSome);
    }
    Event_ShowMessage(21, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor10LineAndFlag81f(void)
{
    Engine_EventBegin();
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 10, 20);
    Engine_EventSetMessage((s32)MsgHaidiaIsJasmineBackYet);
    Event_ShowMessage(10, 0);
    GameFlag_Set(0x81f);
    Engine_EventEnd();
}

void FieldScene_RunScene373_02000cd0(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Engine_TaskWait(10);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventSetMessage((s32)MsgHaidiaIFeltAnotherOne);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_FaceActor(17, ACTOR_PARTY_LEADER, 20);
    Event_ShowMessage(17, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActorNineteenDialogue(void)
{
    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(19, 2);
    Engine_EventWait(20);
    Actor_FaceActor(19, ACTOR_PARTY_LEADER, 20);
    Engine_EventSetMessage((s32)MsgHaidiaNoTravelersSinceTheEruption);
    Event_AskYesNo(19, 0);
    GameFlag_Set(0x307);
    Engine_EventEnd();
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
    Engine_EventRequestExit(11);
}

void SceneState_ApplyFlag801Branch(void)
{
    if (GameFlag_IsSet(0x801) == 0) {
        FieldScene_RunScene373SequenceC();
    } else {
        Audio_PlayCue(123);
        Engine_EventRequestExit(1);
    }
}

void SceneState_SetValue123Mode3(void)
{
    Audio_PlayCue(123);
    Engine_EventRequestExit(3);
}

void SceneState_SetValue123Mode4(void)
{
    Audio_PlayCue(123);
    Engine_EventRequestExit(4);
}

void SceneState_ApplyValues123And2(void)
{
    Audio_PlayCue(123);
    Engine_EventRequestExit(2);
}

void FieldScene_RunScene373_02000e54(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells((u32)gHaidiaMuraCellAnimA, 54, 32);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x196, 0x2d7);
    Engine_EventRequestExit(5);
}

void FieldScene_RunScene373_02000e84(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells((u32)gHaidiaMuraCellAnimB, 45, 39);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x106, 0x325);
    Engine_EventRequestExit(6);
}

void SceneDialogue_RunFlag815GatedStep(void)
{
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0 && GameFlag_IsSet(0x87a) == 0) {
        Engine_EventBegin();
        Engine_EventSetMessage((s32)MsgHaidiaYouCantBeRobin);
        Event_OpenMessage(21, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Event_ShowMessageAndWait(21, 0, 60);
            Event_ShowMessage(21, 0);
        } else {
            u8 *b = *(u8 **)&gEventWork;
            u16 *h = (u16 *)(b + 0x1d8);
            *h = *h + 2;
            Engine_EventWait(40);
            Event_ShowMessage(21, 0);
        }
        Engine_EventEnd();
    } else {
        Audio_PlayCue(0x9e);
        Map_AnimateCells((s32)gHaidiaMuraCellAnimA, 50, 44);
        Actor_WalkTo(ACTOR_PARTY_LEADER, 0x154, 0x378);
        Engine_EventRequestExit(7);
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
    Engine_EventRequestExit(8); /* main:0808a248 */
}

/* Runs a short scripted step, then two 3-argument setup calls, then another
 * short scripted step; none of the callees' effects are visible here. */
void FieldScene_RunScene373SequenceA(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)gHaidiaMuraCellAnimD, 52, 76); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x176, 0x4d6); /* object_id 0, x 0x176, z 0x4d6 */
    Engine_EventRequestExit(9); /* main:0808a248 */
}

void FieldScene_RunPrimarySequenceSecond(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)gHaidiaMuraCellAnimA, 35, 74); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 0x4b6);
    Engine_EventRequestExit(10); /* main:0808a248 */
}

void FieldScene_RunScene373SequenceC(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec8 = Object_GetById(ACTOR_PARTY_LEADER);
    rec7 = Object_GetById(ACTOR_JASMINE);
    Engine_EventBegin();
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
    Engine_TaskWait(1);
    Actor_SetSpeed(ACTOR_JASMINE, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_JASMINE, 110, 0x11b);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JASMINE, 2);
    Engine_EventSetMessage((s32)MsgHaidiaSukuretaIsWaitingForUs);
    if (*(s32 *)(rec8 + 8) < *(s32 *)(rec7 + 8)) {
        Event_ShowMessageAndWait(0xa005, 0, 2);
    } else {
        Event_ShowMessageAndWait(0x8005, 0, 2);
    }
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(2);
    Engine_ActorSetAnimation(ACTOR_JASMINE, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_JASMINE, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(ACTOR_JASMINE);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 110, 0x12f);
    Engine_EventEnd();
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

/* Until flag 0x808 is set, the two puppies bark and the leader walks on. */
void FieldScene_RunPuppyBarks(void)
{
    s32 msg;

    if (GameFlag_IsSet(0x808) == 0) {
        Engine_EventBegin();
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
        msg = (s32)MsgHaidiaRrruffRrrruff;
        Engine_EventSetMessage(msg);
        Event_ShowMessageAndWait(15, 0, 2);
        Event_ShowMessageAndWait(16, 0, 2);
        Engine_MessageShowCentered(msg + 2, 1);
        Engine_EventWait(6);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 69, 0x366);
        Engine_EventEnd();
    }
}

void SceneState_RunFlag204Step(void)
{
    s32 m, n;
    Engine_EventBegin();
    m = 20;
    n = 50;
    Map_CopyCellAttributes(49, 53, 8, 4, m, n);
    HaidiaMura_RunWalkScene032B0(0, 10, 11, 1);
    GameFlag_Set(0x204);
    Engine_EventEnd();
}

void SceneState_SetFlag204AndConfigureRegion49_46(void)
{
    s32 p5, p6;
    Engine_EventBegin();
    HaidiaMura_RunWalkScene03380(0, 13, 10, 1);
    GameFlag_Clear(0x204);
    p5 = 20;
    p6 = 50;
    Map_CopyCellAttributes(49, 46, 8, 4, p5, p6);
    Engine_EventEnd();
}

void FieldScene_RunScene373SequenceE(void)
{
    u32 i;
    u8 *rec7;
    s32 record;

    rec7 = Object_GetById(22);
    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x20000);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 5, 0);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 215, 0x193);
    rec7[90] |= 1;
    Actor_SetPosition(22, 0xa60000, 0x1770000);
    Actor_FaceDirection(22, 0x2000, 20);
    rec7[90] = (rec7[90] ^ 1);
    Actor_SetSpeed(22, 0x28000, 0x28000);
    Engine_ActorJump(22, 4, 0);
    Actor_WalkToAndWait(22, 202, 0x18b);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
    Actor_FaceDirection(22, 0x3000, 24);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(20);
    Actor_SetSpeed(22, 0x18000, 0x10000);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gHaidiaMuraLeaderWalkActions);
    Engine_EventWait(10);
    Actor_ShowEmote(22, 0x103, 0);
    Engine_ActorEnableActionCallback(22, gHaidiaMuraActor22WalkActions);
    Object_RefreshSelectorById(0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x100, 0x1da);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Object_RefreshSelectorById(22);
    Actor_WalkToAndWait(22, 0x100, 0x1c8);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(22, 0x4000, 20);
    Engine_ActorStartRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgHaidiaNotSneakingUpMtAleph);
    Event_AskYesNo(22, 0);
    record = Actor_Get(22);
    *(s32 *)(record + 108) = (s32)SceneActor_RunStep18WhenTargetSet;
    Engine_ActorEnableActionCallback(22, gHaidiaMuraActor22Actions);
    GameFlag_Set(0x823);
    Engine_EventEnd();
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
    Engine_EventBegin();
    Engine_ActorStartRepeatedMotion(22, 2);
    Actor_ShowEmote(22, 0x100, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 40);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, a0);
    Engine_EventWait(10);
    Actor_ShowEmote(22, 0x103, 0);
    Object_SetActionCallbackAndRefreshById(22, p8);
    Object_RefreshSelectorById(0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(22, 2);
    *(s32 *)(rec8 + 24) = 0x10000;
    *(s32 *)(rec8 + 28) = 0x10000;
    record = Object_GetById(ACTOR_PARTY_LEADER);
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;
    Engine_EventSetMessage((s32)MsgHaidiaNotSneakingUpMtAleph);
    Event_AskYesNo(22, 0);
    record = Actor_Get(22);
    *(s32 *)(record + 108) = (s32)SceneActor_RunStep18WhenTargetSet;
    Engine_ActorEnableActionCallback(22, gHaidiaMuraActor22Actions);
    Engine_EventEnd();
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

/*
 * Outside Robin's house in Vale. He boards up three holes in the wall
 * while his mother Dora watches, then she talks with him about his father
 * until Gerald and Jasmine arrive to fetch him for the trip to Mt. Aleph with
 * Sukureta.
 */
void HouseScene_RunRepairMorning(void)
{
    struct FieldActor *leader;
    struct FieldActor *actor;
    u8 leader_motion_flags;
    u8 *leader_motion;
    s32 i;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    actor = Engine_EventGetViewCenter();
    actor->motion_flags = 0;
    Camera_MoveTo(PIXELS(383), PIXELS(160), PIXELS(877), 0);
    Engine_EventWait(1);
    Engine_MapRedraw();
    Map_CopyCellAttributes(49, 41, 7, 3, 20, 50);
    Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
    Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
    Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    leader_motion = &leader->motion_flags;
    leader_motion_flags = *leader_motion;
    *leader_motion = 0;
    Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(407), PIXELS(690));
    Actor_SetPosition(ACTOR_DORA, PIXELS(392), PIXELS(896));
    Actor_SetPosition(ACTOR_GERALD, PIXELS(298), PIXELS(736));
    Actor_SetPosition(ACTOR_JASMINE, PIXELS(298), PIXELS(760));
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_WEST, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gLeaderHammerAction);
    SceneActor_ResetActorRun(ACTOR_BOARD, 2, 1);
    gEventWork->start_transition = 0;
    gEventWork->transition_frames = 32;
    Engine_EventOpenScreen();
    Actor_SetSpeed(ACTOR_JASMINE, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Engine_ActorEnableActionCallback(ACTOR_JASMINE, gJasmineAction);
    Engine_ActorEnableActionCallback(ACTOR_GERALD, gGeraldAction);

    /* The first hole. */
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, ACTION_TABLE_STOP);
    leader->scale_x = 0x10000;
    leader->scale_y = 0x10000;
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 404, 843);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(20);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 2);
    Actor_SetChildValue(ACTOR_BOARD, 2);
    Actor_SetSpeed(ACTOR_BOARD, 0x4ccc, 0x2666);
    Actor_MoveToAndWait(ACTOR_BOARD, 390, 832);
    Engine_EventWait(80);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_MoveToAndWait(ACTOR_BOARD, 402, 828);
    Engine_EventWait(80);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_SetChildValue(ACTOR_BOARD, 0);
    Actor_SetPosition(ACTOR_BOARD, PIXELS(390), PIXELS(842));
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gLeaderHammerAction);
    Engine_EventWait(200);
    Map_CopyCellsTo(7, 102, 84, 41, 2, 1);

    /* The second hole. */
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, ACTION_TABLE_STOP);
    leader->scale_x = 0x10000;
    leader->scale_y = 0x10000;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(20);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 2);
    Actor_SetChildValue(ACTOR_BOARD, 2);
    Actor_SetSpeed(ACTOR_BOARD, 0x4ccc, 0x2666);
    Actor_MoveToAndWait(ACTOR_BOARD, 390, 832);
    Engine_EventWait(80);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_MoveToAndWait(ACTOR_BOARD, 377, 828);
    Engine_EventWait(80);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_SetChildValue(ACTOR_BOARD, 0);
    Actor_SetPosition(ACTOR_BOARD, 0, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gLeaderHammerAction);
    Engine_EventWait(200);
    Map_CopyCellsTo(6, 102, 83, 41, 1, 1);

    /* The third hole. */
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, ACTION_TABLE_STOP);
    leader->scale_x = 0x10000;
    leader->scale_y = 0x10000;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(20);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
    Actor_FaceDirection(ACTOR_DORA, FACING_NORTHWEST + FACING_STEP, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 20);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 2);
    Actor_SetChildValue(ACTOR_LAST_BOARD, 2);
    Actor_SetSpeed(ACTOR_LAST_BOARD, 0x4ccc, 0x2666);
    Actor_MoveToAndWait(ACTOR_LAST_BOARD, 390, 832);
    Engine_EventWait(80);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_MoveToAndWait(ACTOR_LAST_BOARD, 360, 842);
    Engine_EventWait(80);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_SetChildValue(ACTOR_LAST_BOARD, 0);
    Actor_SetPosition(ACTOR_LAST_BOARD, 0, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gLeaderHammerAction);
    Engine_EventWait(200);
    Map_CopyCellsTo(5, 103, 82, 42, 1, 1);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, ACTION_TABLE_STOP);
    leader->scale_x = 0x10000;
    leader->scale_y = 0x10000;

    /* His mother praises the work and talks about his father. */
    Engine_EventSetMessage((s32)MsgHaidiaGoodJob);
    Engine_ActorJump(ACTOR_DORA, 2, 20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 20);
    HaidiaMura_RunWalkScene032B0(ACTOR_DORA, 5, 6, 0);
    Actor_SetSpeed(ACTOR_DORA, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(ACTOR_DORA, 397, 832);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
    Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 40);
    Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_OpenMessage(ACTOR_DORA, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
        gEventWork->message++;
    }
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_EventSetMessage((s32)MsgHaidiaWorkingYourselvesBone);
    Actor_WalkToAndWait(ACTOR_DORA, 386, 841);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
    Event_OpenMessage(ACTOR_DORA, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
        gEventWork->message++;
    }
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgHaidiaDevastatedWhenKyle);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_WalkToAndWait(ACTOR_DORA, 386, 825);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(60);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);

    /* Gerald and Jasmine arrive. */
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(860), 1);
    Actor_SetSpeed(ACTOR_JASMINE, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_GERALD, 369, 904);
    Actor_WalkToAndWait(ACTOR_JASMINE, 392, 904);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_STAND);
    HaidiaMura_RunWalkScene032B0(ACTOR_JASMINE, 10, 11, 0);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_NORTHWEST, 0);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 30);
    Engine_ActorJump(ACTOR_JASMINE, 4, 0);
    Actor_WalkToAndWait(ACTOR_JASMINE, 392, 843);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_WEST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    HaidiaMura_RunWalkScene032B0(ACTOR_GERALD, 10, 11, 0);
    Actor_SetSpeed(ACTOR_JASMINE, 0x4ccc, 0x2666);
    Actor_SetSpeed(ACTOR_GERALD, 0x4ccc, 0x2666);
    Actor_WalkTo(ACTOR_GERALD, 392, 843);
    Actor_Get(ACTOR_JASMINE)->unknown_5a &= 0xfe;
    Actor_WalkToAndWait(ACTOR_JASMINE, 408, 843);
    Engine_EventWait(1);
    Actor_Get(ACTOR_JASMINE)->unknown_5a |= 1;
    Actor_FaceDirection(ACTOR_JASMINE, FACING_WEST, 0);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_STAND);
    Actor_FaceDirection(ACTOR_GERALD, FACING_WEST, 30);
    Engine_ActorJump(ACTOR_DORA, 4, 30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 30);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 1, 80);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 80);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 40);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    Engine_EventWait(100);
    Engine_ActorFaceEachOther(ACTOR_JASMINE, ACTOR_GERALD, 30);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 5, 60);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_WEST, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, ANIM_NOD);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, ANIM_NOD);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, ANIM_NOD);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(PIXELS(373), PIXELS(160), PIXELS(837), 1);
    Actor_WalkToAndWait(ACTOR_DORA, 364, 816);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 40);
    Engine_ActorFaceEachOther(ACTOR_JASMINE, ACTOR_GERALD, 30);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_GERALD, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_WEST, 30);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(10);
    Event_OpenMessage(ACTOR_DORA, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
        gEventWork->message++;
    }
    Engine_EventWait(40);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);

    /* She slips from the step; Gerald catches her. */
    Engine_EventSetMessage((s32)MsgHaidiaAsStubbornAsYourFather);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 3, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorJump(ACTOR_DORA, 4, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
    Engine_ActorSetAnimation(ACTOR_DORA, 7);
    Engine_EventWait(5);
    Event_ShowTwoMessagesAndWait(ACTOR_DORA, 14, 2, 24, 2, ACTOR_GERALD, 10, 14, 4, 14, 0);
    actor = Actor_Get(ACTOR_DORA);
    actor->sprite->flags = 0;
    actor->unknown_5a &= 0xfe;
    Actor_SetSpeed(ACTOR_DORA, 0x30000, 0x18000);
    Actor_WalkToAndWait(ACTOR_DORA, 364, 815);
    Engine_EventWait(4);
    for (i = 0; i != 4; i++) {
        actor->z.fixed += 0x18000;
        actor->scale_y -= 0x1999;
        Engine_EventWait(1);
    }
    Actor_SetPosition(ACTOR_DORA, 0, 0);
    Actor_SetSpeed(ACTOR_GERALD, 0x30000, 0x18000);
    Engine_ActorJump(ACTOR_GERALD, 6, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, 374, 827);
    Event_ShowMessage(ACTOR_JASMINE, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_ShowEmote(ACTOR_JASMINE, EMOTE_IN_FRONT | 0, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 0, 10);
    Engine_ActorSetAnimation(ACTOR_GERALD, 13);
    Engine_ActorJump(ACTOR_GERALD, 2, 5);
    MapRender_SetValues(0, 0x40000, 0x10000);
    Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_GERALD), 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 3);
    MapRender_SetValues(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 80);
    Engine_ActorSetAnimation(ACTOR_DORA, 8);
    actor->scale_y = 0x8000;
    Actor_SetPosition(ACTOR_DORA, PIXELS(364), PIXELS(811));
    for (i = 0; i != 5; i++) {
        actor->scale_y += 0x1999;
        Engine_EventWait(1);
    }
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 30);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Camera_SetSpeed(0x4ccc, 0x999);
    Camera_MoveTo(PIXELS(372), PIXELS(160), PIXELS(859), 1);
    Actor_SetSpeed(ACTOR_DORA, 0x30000, 0x18000);
    Engine_ActorJump(ACTOR_DORA, 6, 0);
    Actor_WalkToAndWait(ACTOR_DORA, 359, 835);
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(30);
    actor = Actor_Get(ACTOR_DORA);
    actor->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 80);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 1, 80);
    Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_SetAttachedEffect(ACTOR_DORA, EMOTE_IN_FRONT | 2);
    Engine_EventWait(80);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 30);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 80);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_WEST, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_GERALD), 1);
    Engine_ActorJump(ACTOR_GERALD, 6, 0);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_STAND);
    Actor_SetSpeed(ACTOR_GERALD, 0x40000, 0x20000);
    actor = Actor_Get(ACTOR_GERALD);
    actor->unknown_5a &= 0xfe;
    Actor_SetDestination(ACTOR_GERALD, 403, 827);
    Actor_SetAttachedEffect(ACTOR_JASMINE, EMOTE_IN_FRONT | 2);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_NORTH, 20);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 1);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 0, 0);
    Engine_ActorSetAnimation(ACTOR_GERALD, 13);
    Engine_ActorJump(ACTOR_GERALD, 2, 5);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_GERALD), 0);
    Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
    MapRender_SetValues(0, 0x40000, 0x10000);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    MapRender_SetValues(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 30);
    Actor_SetSpeed(ACTOR_JASMINE, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(ACTOR_JASMINE, 408, 855);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 5, 60);
    Engine_ActorStartRepeatedMotion(ACTOR_JASMINE, 3);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(80);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, ANIM_SHAKE_HEAD);
    Engine_EventWait(80);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 80);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 5, 80);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(ACTOR_JASMINE, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 1, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 70);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_WEST, 60);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, ANIM_NOD);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 0, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_GERALD), 0);
    Engine_ActorJump(ACTOR_GERALD, 4, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, 398, 828);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(60);

    /* The three set off together. */
    actor = Actor_Get(ACTOR_GERALD);
    actor->unknown_5a |= 1;
    actor = Actor_Get(ACTOR_JASMINE);
    actor->unknown_5a |= 1;
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_JASMINE, 0x10000, 0x8000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_WalkTo(ACTOR_JASMINE, actor->x.part.pixel + 16, actor->z.part.pixel);
    Actor_WalkToAndWait(ACTOR_GERALD, actor->x.part.pixel + 16, actor->z.part.pixel - 16);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 30);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_JASMINE, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(40);
    Actor_WalkToAndWait(ACTOR_JASMINE, actor->x.part.pixel, actor->z.part.pixel);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, actor->x.part.pixel, actor->z.part.pixel);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Engine_PartyAddMembers(ACTOR_GERALD, ACTOR_JASMINE);
    Camera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(887), 1);
    HaidiaMura_RunWalkScene03380(ACTOR_PARTY_LEADER, 13, 10, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_Get(ACTOR_DORA)->unknown_5a |= 1;
    HaidiaMura_RunWalkScene03380(ACTOR_DORA, 6, 5, 0);
    Actor_WalkToAndWait(ACTOR_DORA, 373, 887);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
    Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(20);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(100);
    GameFlag_Set(0x202);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    *leader_motion = leader_motion_flags;
    Engine_EventEnd();
}

/* Vale entry: from entrance 16 run the staging scene; otherwise place the story actors and cells for the flags, start the village tasks and redraw. */
s32 HaidiaMura_ApplyEntryState(void)
{
    struct FieldActor *actor;
    s32 set;

    if (gGameState.entrance == 16) {
        PaletteGlow_Update(((u8 *)&gGameState)[517], ((u8 *)&gGameState)[518]);
        FieldScene_RunLargeStagingSequence();
    } else {
        if (!Engine_GameFlagIsSet(0xfd0)) {
            if (!Engine_GameFlagIsSet(0x87a)) {
                InitializeStagedActorSceneOrbitingEffect(26);
            } else {
                InitializeStagedActorSceneOrbitingEffect(20);
            }
        }
        Engine_MapCopyCellsTo(2, 102, 84, 41, 2, 1);
        Engine_MapCopyCellsTo(1, 102, 83, 41, 1, 1);
        actor = Object_GetById((Engine_GameFlagIsSet(0x87a) != 0) + 20);
        Engine_ActorSetSpriteFlags(actor, 0);
        if (Engine_GameFlagIsSet(0x314)) {
            actor->x.fixed = 181 << 17;
        } else if (Engine_GameFlagIsSet(0x316)) {
            actor->x.fixed = 197 << 17;
        } else {
            actor->x.fixed = 189 << 17;
        }
        actor->z.fixed = 0x2480000;
        actor->y.fixed = 0xc00000;
        HaidiaMura_OpenVillagerLane();
        actor->unknown_22 = 3;
        actor->motion_flags = 0;
        Engine_TaskAddCallback(SceneActor_SetFlagByteBySlotZeroPosition, 0xc80);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
        if (Engine_GameFlagIsSet(0x87a)) {
            if (Engine_GameFlagIsSet(0x109)) {
                if (Engine_GameFlagIsSet(0x204)) {
                    Call6(Engine_MapCopyCellAttributes, 49, 53, 8, 4, 20, 50);
                }
                if (Engine_GameFlagIsSet(0x210)) {
                    SceneState_Send210AndApplyRectAt40x84();
                }
            }
        } else {
#else
        if (!Engine_GameFlagIsSet(0x87a)) {
#endif
            if (Value1(Engine_GameFlagIsSet, 0x815)) {
                actor = Object_GetById(21);
                Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
                actor->scale_x = 0x28f;
                actor->scale_y = 0x28f;
            }
            if (Engine_GameFlagIsSet(0x808)) {
                Engine_ActorSetPosition(15, 0, 0);
                Engine_ActorSetPosition(16, 0, 0);
                Engine_ActorSetPosition(17, 0, 0);
            }
            set = Engine_GameFlagIsSet(0x815);
            if (set == 0) {
                if (!Engine_GameFlagIsSet(0x109)) {
                    if (Engine_GameFlagIsSet(0x823)) {
                        Call3(Engine_ActorSetPosition, 22, 0x1000000, 0x1c80000);
                        Object_GetById(22)->update = (void *)SceneActor_RunStep18WhenTargetSet;
                        Engine_ActorEnableActionCallback(22, gHaidiaMuraActor22Actions);
                    }
                } else {
                    Object_GetById(22)->unknown_5b = set;
                    Engine_GameFlagClear(0x241);
                }
                if (gGameState.entrance != 16 && !Engine_GameFlagIsSet(0x87a)) {
                    Engine_TaskAddCallback(FieldScene_RunScene373SequenceB, 0xc80);
                }
            }
            if (!Value1(Engine_GameFlagIsSet, 0x308) && gGameState.entrance == 17) {
                Scene_RepairTheHouse();
                Engine_GameFlagSet(0x308);
            }
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
            if (Engine_GameFlagIsSet(0x109)) {
                if (Engine_GameFlagIsSet(0x204)) {
                    Call6(Engine_MapCopyCellAttributes, 49, 53, 8, 4, 20, 50);
                }
                if (Engine_GameFlagIsSet(0x210)) {
                    SceneState_Send210AndApplyRectAt40x84();
                }
            }
            Engine_ActorSetSpritePriority(ACTOR_DORA, 2);
#endif
        }
#if !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_IT)
        if (Engine_GameFlagIsSet(0x109)) {
            if (Engine_GameFlagIsSet(0x204)) {
                Call6(Engine_MapCopyCellAttributes, 49, 53, 8, 4, 20, 50);
            }
            if (Engine_GameFlagIsSet(0x210)) {
                SceneState_Send210AndApplyRectAt40x84();
            }
        }
#endif
        BattleFx_SetQueuedSoundAndPlay(170);
        Engine_MapRedraw();
        Engine_TaskWait(1);
    }
    return 0;
}

/* Gerald's two talks beside the leader, with actor 8 and then actor 9. Each
 * places Gerald at the leader's position, and ends with him walking back to
 * the leader's tile and a story flag set. */
void FieldScene_RunSecondaryActorSequence(void)
{
    s32 record;
    s32 msg;

    Engine_EventBegin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 20);
    msg = (s32)MsgHaidiaAh;
    Engine_EventSetMessage(msg);
    Engine_ActorStartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 20);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x18e0000, -1, 0x2460000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1a4, 0x260);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_FaceDirection(8, 0x3000, 0);
    record = Object_GetById(0);
    if (record != 0) {
        /* Copy the record's fields at +8 and +16 onto actor 1. */
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_WalkToAndWait(ACTOR_GERALD, 0x192, 0x260);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 20);
    Event_ShowMessage(0x1001, 0);
    Actor_FaceDirection(8, 0x5000, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessage(0x4008, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(8, 2);
    Event_OpenMessage(0x4008, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        bump_step(1);
        Engine_ActorStartRepeatedMotion(8, 1);
    }
    Event_ShowMessageAndWait(0x4008, 0, 40);
    Actor_ShowEmote(8, 0x105, 60);
    Engine_EventSetMessage(msg + 6);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(0x1001, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0xd000, 20);
    Event_ShowMessage(0x4008, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Event_ShowMessageAndWait(0x1001, 0, 120);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x105, 40);
    Event_ShowMessageAndWait(0x1001, 0, 40);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(40);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_ShowMessageAndWait(0x4008, 0, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    record = Object_GetById(0);
    if (record != 0) {
        /* Copy the record's fields at +10 and +18 onto actor 1. */
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    GameFlag_Set(0x303);
    Engine_EventEnd();
}

void FieldScene_RunPrimaryActorSequence(void)
{
    s32 record;

    Engine_EventBegin();
    Camera_MoveTo(0x1650000, -1, 0x2e20000, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x16f, 0x2e9);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    record = Object_GetById(0);
    if (record != 0) {
        /* Copy the s32 coordinate pair at +8/+16 of the looked-up record. */
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_WalkToAndWait(ACTOR_GERALD, 0x15a, 0x2e9);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 20);
    Engine_EventSetMessage((s32)MsgHaidiaHowHaveYouBeen);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_ActorRunRepeatedMotion(9, 2);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_FaceDirection(9, 0x3000, 10);
    Actor_FaceDirection(9, 0x5000, 10);
    Actor_FaceDirection(9, 0x3000, 40);
    Event_ShowMessageAndWait(9, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_ActorRunRepeatedMotion(9, 1);
    Actor_FaceDirection(9, 0x5000, 10);
    Event_ShowMessageAndWait(9, 0, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 40);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Engine_ActorSetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x1000, 40);
    Engine_ActorSetAnimationAndWait(9, 4);
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Engine_ActorSetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 80);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0x1000, 20);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Actor_ShowEmote(ACTOR_GERALD, 0x105, 60);
    } else {
        bump_step(1);
    }
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 10);
    Engine_EventSetMessage((s32)MsgHaidiaEverProtectFamily);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_ActorSetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(9, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x1000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    record = Object_GetById(0);
    if (record != 0) {
        /* Copy the s16 coordinate pair at +10/+18 of the looked-up record. */
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    GameFlag_Set(0x304);
    Engine_EventEnd();
}

/* Sets up actors 12, 13, 14 and 20 with shared data and movement/speed
 * parameters, then drives actor 11 through a further sequence of moves. */
void FieldScene_RunCompanionActorSequence(void)
{
    u32 i;
    s32 actor_data;
    s32 shared_data;

    Engine_EventBegin();
    actor_data = Actor_Get(ACTOR_A);
    Engine_ActorSetSpriteFlags(actor_data, 0);
    actor_data = Actor_Get(ACTOR_B);
    Engine_ActorSetSpriteFlags(actor_data, 0);
    actor_data = Actor_Get(ACTOR_C);
    Engine_ActorSetSpriteFlags(actor_data, 0);
    Engine_ActorSetAnimation(ACTOR_A, 0);
    Engine_ActorSetAnimation(ACTOR_B, 0);
    Engine_ActorSetAnimation(ACTOR_C, 0);
    Engine_TaskWait(20);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    shared_data = ((s32)gVillagerAction);
    Engine_ActorEnableActionCallback(ACTOR_A, shared_data);
    Engine_TaskWait(10);
    Engine_ActorEnableActionCallback(ACTOR_B, shared_data);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_TaskWait(20);
    Object_SetActionCallbackAndRefreshById(ACTOR_C, shared_data);
    Actor_ShowEmote(ACTOR_D, 0x100, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_D, 2);
    Actor_FaceDirection(ACTOR_D, 0xd000, 10);
    Engine_EventSetMessage((s32)MsgHaidiaIUsedToPlayHere);
    Event_ShowMessageAndWait(ACTOR_D, 0, 40);
    Actor_FaceActor(ACTOR_D, ACTOR_PARTY_LEADER, 20);
    Event_ShowMessage(ACTOR_D, 0);
    Actor_FaceDirection(ACTOR_D, 0x8000, 10);
    GameFlag_Set(0x305);
    Engine_EventEnd();
}

void HaidiaMura_RunWalkScene032B0(s32 a0, s32 a1, s32 a2, s32 a3)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p8;
    s32 p9;
    s32 p9b;
    u8 *rec8;
    s32 record;
    u8 *p5;

    p9 = a3;
    p8 = a1;
    p10 = a2;
    rec8 = ((u8 * (*)())Object_GetById)();
    p5 = *(s32 *)((s32)rec8 + 80);
    Call3(Engine_ActorSetSpeed, a0, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, a0, 0x188, 0x376);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 10);
    ((struct Flags85 *)rec8)->flags = 0;
    ((struct Flags38 *)p5)->flags = 0;
    Engine_ActorSetAnimation(a0, p8);
    Call3(Engine_ActorSetSpeed, a0, 0x4ccc, 0x2666);
    Call3(Engine_ActorMoveToAndWait, a0, 0x188, 0x36b);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(a0, p10);
    Call3(Engine_ActorSetSpeed, a0, 0x10000, 0x8000);
    Call3(Engine_ActorMoveToAndWait, a0, 0x188, 0x35b);
    p5[38] = 1;
    if (p9 != 0) {
        rec8[85] = 3;
    }
    Engine_EventWait(10);
    Engine_ActorSetAnimation(a0, 1);
    p9b = (s32)p5 + 38;
    p10b = (s32)rec8 + 85;
}

void HaidiaMura_RunWalkScene03380(s32 a0, s32 a1, s32 a2, s32 a3)
{
    u32 i;
    s32 p10;
    u8 *p11;
    s32 p11b;
    s32 p8;
    s32 p9;
    s32 p9b;
    s32 rec8;
    s32 record;
    u8 *p5;

    p9 = a3;
    p8 = a1;
    p10 = a2;
    rec8 = ((s32 (*)())Object_GetById)();
    p5 = *(s32 *)(rec8 + 80);
    Call3(Engine_ActorSetSpeed, a0, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, a0, 0x188, 0x35b);
    Call3(Engine_ActorFaceDirection, a0, 0xc000, 10);
    ((struct Flags85 *)rec8)->flags = 0;
    ((struct Flags38 *)p5)->flags = 0;
    Engine_ActorSetAnimation(a0, p8);
    Call3(Engine_ActorSetSpeed, a0, 0x10000, 0x8000);
    Call3(Engine_ActorMoveToAndWait, a0, 0x188, 0x36b);
    Engine_EventWait(10);
    Call3(Engine_ActorSetSpeed, a0, 0x4ccc, 0x2666);
    Engine_ActorSetAnimation(a0, p10);
    Call3(Engine_ActorMoveToAndWait, a0, 0x188, 0x37a);
    *(s32 *)(rec8 + 40) = 0x20000;
    p5[38] = 1;
    if (p9 != 0) {
        ((struct Flags85 *)rec8)->flags = 3;
    }
    Engine_ActorSetAnimation(a0, 1);
    p9b = (s32)p5 + 38;
    p11b = a0;
}

void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode)
{
    s32 selector = first;
    u32 i;

    if (mode == 0) {
        for (i = 0; i < count; i++) {
            struct Resource373Actor *actor = Actor_Get(selector);

            actor->flag55 = 0;
            Engine_ActorSetSpriteFlags(actor, 0);
            actor->field08 = 0x01860000;   /* 0xc3 << 17. */
            actor->field0c = 0x00a00000;   /* 0xa0 << 16. */
            actor->field10 = 0x034a0000;   /* The literal pool word. */
            selector++;
        }
        return;
    }

    for (i = 0; i < count; i++) {
        Actor_SetPosition(selector, 0, 0);
        selector++;
    }
}

/* Three years later: the opening of the repair morning. */
void FieldScene_RunLargeStagingSequence(void)
{
    u32 i;
    /* FAKEMATCH: rec's dead zero initialiser, and k, none2 and facing below,
     * park values in locals only to keep the reference's register
     * assignment; folding any of them into its uses changes it. */
    s32 rec = 0;
    s32 rec3;
    u8 *rec8;
    u8 *record;
    s32 none2;
    s32 msg;
    s32 k;
    s32 facing;

    rec3 = Object_GetById(ACTOR_PARTY_LEADER);
    rec8 = Object_GetById(14);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    *((u8 *)Engine_EventGetViewCenter() + 85) = 0;
    Engine_TaskWait(1);
    Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
    Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
    Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
    Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
    rec = Object_GetById(11);
    *(u8 *)(rec + 85) = 0;
    k = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3480000;
    *(s32 *)(rec + 8) = k;
    Engine_ActorSetSpriteFlags(rec, 0);
    rec = Object_GetById(12);
    *(u8 *)(rec + 85) = 0;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x34c0000;
    *(s32 *)(rec + 8) = k;
    Engine_ActorSetSpriteFlags(rec, 0);
    rec = Object_GetById(13);
    *(u8 *)(rec + 85) = 0;
    *(s32 *)(rec + 16) = 0x3500000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 8) = k;
    Engine_ActorSetSpriteFlags(rec, 0);
    record = Object_GetById(11);
    Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
    record = Object_GetById(12);
    Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, (s32)gLeaderHammerAction);
    Graphics_EnableObjLayerAndCallbacks();
    msg = (s32)MsgHaidiaRepairCaption;
    UiText_ShowCenteredMessage(msg, 0, 0);
    ObjectDispatch_StopCallbacksAndHideLayers();
    Camera_MoveTo(0x1530000, 0xa00000, 0x4950000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Camera_SetSpeed(0x547a, 0xa8f);
    Camera_MoveTo(0x1280000, 0xa00000, 0x3990000, 1);
    Actor_SetPosition(ACTOR_JASMINE, 0x1990000, 0x46e0000);
    Engine_TaskWait(1);
    Actor_SetSpeed(ACTOR_JASMINE, 0xb333, 0x5999);
    Actor_WalkTo(ACTOR_JASMINE, 0x1a4, 0x42c);
    Audio_PlayCueFromEventWork();
    gEventWork->transition_frames = 60;
    Engine_EventOpenScreen();
    Engine_ActorWaitForMove(ACTOR_JASMINE);
    Actor_SetSpeed(ACTOR_JASMINE, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x155, 0x428);
    Actor_SetSpeed(ACTOR_JASMINE, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x167, 0x409);
    Actor_SetSpeed(8, 0x8000, 0x4000);
    Actor_WalkTo(8, 0x13e, 0x3b3);
    Engine_ActorSetAnimation(8, 2);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x19c, 0x409);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x19c, 0x3fb);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x176, 0x3f0);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x15b, 0x3bb);
    Actor_WalkToAndWait(8, 0x13e, 0x3b3);
    Engine_ActorFaceEachOther(ACTOR_JASMINE, 8, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
    Engine_EventWait(10);
    Actor_WalkTo(8, 0x17b, 0x3f9);
    Camera_SetSpeed(0x8000, 0x1000);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x14d, 0x398);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x12b, 0x39c);
    Engine_CameraWaitForMove();
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_JASMINE, 0xf000, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(20);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x1830000, 0xa00000, 0x3620000, 1);
    Engine_CameraWaitForMove();
    Engine_ActorJump(10, 2, 20);
    Engine_EventSetMessage(msg + 1);
    Event_ShowMessageAndWait(0x100a, 0, 10);
    *(s32 *)(rec3 + 24) = 0x10000;
    *(s32 *)(rec3 + 28) = 0x10000;
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, 1);
    Engine_ActorFaceEachOther(10, ACTOR_PARTY_LEADER, 40);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(40);
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(0x100a, 0, 40);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, (s32)gLeaderHammerAction);
    Engine_CameraFollowActor(ACTOR_JASMINE, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
    Actor_FaceDirection(ACTOR_JASMINE, 0xd000, 10);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x138, 0x2f7);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x169, 0x2f8);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 40);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 40);
    Event_ShowMessageAndWait(0x6001, 0, 10);
    Actor_ShowEmote(ACTOR_JASMINE, 0x100, 0);
    Engine_ActorJump(ACTOR_JASMINE, 4, 40);
    Actor_FaceDirection(ACTOR_JASMINE, 0xc000, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
    Engine_EventWait(10);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0x18c0000, -1, 0x24c0000, 1);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x1c8, 0x2e3);
    Engine_CameraWaitForMove();
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_GERALD, 0x105, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    FieldScene_RunStep8C();
    Engine_ActorSetAnimation(ACTOR_GERALD, 17);
    Event_ShowMessageAndWait(0x2001, 0, 20);
    Audio_PlayCue(131);
    for (i = 0; i < 60; i++) {
        OverlayObject_UpdateOnFrameBit1(Object_GetById(ACTOR_GERALD));
        Engine_TaskWait(1);
    }
    Engine_ActorSetSpritePriority(ACTOR_GERALD, 1);
    Call2(Engine_TaskAddCallback, (s32)SceneState_SetValue1ThenCall, 0xc80);
    Value2(Engine_TaskAddCallback, (s32)FieldScene_RunStep9, 0xc80);
    record = Object_GetById(14);
    Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
    none2 = 0;
    rec8[85] = none2;
    *(s32 *)(rec8 + 8) = 0x1ac0000;
    *(s32 *)(rec8 + 12) = 0xd00000;
    *(s32 *)(rec8 + 16) = 0x2480000;
    facing = 0x8000;
    *(u16 *)(rec8 + 6) = facing;
    *(s32 *)(rec8 + 108) = (s32)Effect_ConfigureSpawnedParticle;
    Engine_EventWait(4);
    Actor_SetSpeed(14, 0x20000, 0x20000);
    Call4(Object_SetPosition, (s32)rec8, 0x1980000, 0xd00000, 0x2480000);
    Engine_EventWait(40);
    Actor_SetSpeed(9, 0x2666, 0x1333);
    Actor_SetSpeed(14, 0x2666, 0x1333);
    /* FAKEMATCH: the reference looks actor 9 up here and drops the result. */
    Object_GetById(9);
    Object_SetPosition((s32)rec8, 0x1880000, 0xd00000, 0x2480000);
    Actor_MoveToAndWait(9, 0x17a, 0x248);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x2005, 0, 10);
    *(s32 *)(rec8 + 108) = none2;
    Engine_ActorSetSpritePriority(ACTOR_GERALD, 2);
    Actor_Get(ACTOR_GERALD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Scheduler_RemoveCallback((s32)SceneState_SetValue1ThenCall);
    Scheduler_RemoveCallback((s32)FieldScene_RunStep9);
    Engine_TaskWait(1);
    Actor_SetChildValue(ACTOR_GERALD, 0);
    Actor_SetChildValue(9, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 0);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Effect_SpawnRisingDustBurst((s32)rec8);
    FieldScene_RunSingleStep();
    Engine_EventWait(10);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x1a8, 0x270);
    Engine_ActorFaceEachOther(ACTOR_GERALD, ACTOR_JASMINE, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(0x6001, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 4);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 80);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 1);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 80);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 4);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(10);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x184, 0x25c);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 4);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_GERALD, 0x105, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, facing, 40);
    Call11(Engine_EventShowTwoMessagesAndWait, 1, 1, 2, 25, 2, 5, 10, 14, 4, 14, none2);
    Engine_EventWait(40);
    Actor_SetAttachedEffect(ACTOR_JASMINE, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Engine_EventWait(80);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 40);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Engine_EventWait(80);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 40);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 80);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 4);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(0x1005, 0, 10);
    Actor_FaceDirection(ACTOR_JASMINE, 0x1000, 40);
    Actor_FaceActor(ACTOR_JASMINE, ACTOR_GERALD, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(10);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x17c, 0x26c);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(0x1005, 0, 10);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 30);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(0x6001, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 4);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 80);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 4);
    Engine_EventWait(10);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x19c, 0x25c);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 1);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 80);
    Engine_ActorJump(ACTOR_JASMINE, 4, 30);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(0x6001, 0, 20);
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_JASMINE, 0xe000, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 80);
    Actor_ShowEmote(ACTOR_JASMINE, 0x103, 40);
    Actor_SetSpeed(ACTOR_JASMINE, 0xcccc, 0x6666);
    Actor_WalkTo(ACTOR_JASMINE, 0x1ac, 0x274);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 0);
    Engine_ActorWaitForMove(ACTOR_JASMINE);
    Event_ShowMessage(0x5001, 0);
    Engine_ActorSetAnimation(ACTOR_JASMINE, 1);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x1ac, 0x274);
    Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 4);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(0x2005, 0, 20);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(0x5001, 0, 20);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
    Engine_EventWait(10);
    Actor_SetSpeed(ACTOR_JASMINE, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Actor_WalkTo(ACTOR_JASMINE, 0x1c2, 0x2ee);
    Actor_WalkTo(ACTOR_GERALD, 0x1c2, 0x2ee);
    Engine_EventWait(60);
    gEventWork->transition_frames = 60;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(12);
    Engine_EventEnd();
}

void Scene_RepairTheHouse(void)
{
    u8 *scene;
    u8 *rec;
    u32 i;
    s32 turn_back;
    s32 turn_side;
    u8 *turned;
    s32 none;
    s32 flag;
    s32 callback_a;
    s32 callback_b;
    s32 callback_c;
    s32 callback_d;
    s32 callback_e;
    s32 callback_f;

    scene = Actor_Get(ACTOR_PARTY_LEADER);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
    Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
    Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
    Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
    Actor_SetPosition(ACTOR_DORA, 0x1880000, 0x3800000);
    turned = Actor_Get(ACTOR_DORA);
    /*
     * Overwritten at once, but the store must stay: its zero halfword
     * temporary is what the record byte stores below reuse out of a high
     * register.
     */
    *(u16 *)(turned + 6) = 0;
    turn_back = 0xc000;
    *(u16 *)(turned + 6) = turn_back;
    Actor_SetPosition(ACTOR_GERALD, 0x12a0000, 0x2e00000);
    turned = Actor_Get(ACTOR_GERALD);
    turn_side = 0x4000;
    *(u16 *)(turned + 6) = turn_side;
    Actor_SetPosition(ACTOR_JASMINE, 0x12a0000, 0x2f80000);
    turned = Actor_Get(ACTOR_JASMINE);
    *(u16 *)(turned + 6) = turn_side;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
    Engine_ActorEnableActionCallback(0, (u32)gLeaderHammerAction);
    rec = Actor_Get(23);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3480000;
    Engine_ActorSetSpriteFlags(rec, 0);
    rec = Actor_Get(24);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x34c0000;
    Engine_ActorSetSpriteFlags(rec, 0);
    rec = Actor_Get(25);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3500000;
    Engine_ActorSetSpriteFlags(rec, 0);
    ((u8 *)Engine_EventGetViewCenter())[85] = 0;
    Engine_TaskWait(1);
    Camera_MoveTo(0x17f0000, 0xa00000, 0x36d0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    *(s32 *)(*(u8 **)&gEventWork + 0x1c8) = 32;
    Engine_EventOpenScreen();
    Actor_SetSpeed(ACTOR_JASMINE, 0x8000, turn_side);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, turn_side);
    Engine_ActorEnableActionCallback(5, (u32)gJasmineAction);
    Engine_ActorEnableActionCallback(1, (u32)gGeraldAction);
    Engine_EventWait(40);
    Engine_ActorEnableActionCallback(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Call3(Engine_ActorFaceDirection, 0, 0xb000, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 400, 840);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, turn_back, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 40);
    FieldScene_RunStep8C();
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 17);
    Engine_TaskAddCallback((void (*)(void))Effect_PlayStepSound, 3200);
    for (i = 0; i < 40; i++) {
        OverlayObject_UpdateOnFrameBit1(scene);
        Engine_TaskWait(1);
    }
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
    /* Callback symbols are pooled loads that stay after the preceding call. */
    callback_a = (s32)SceneState_SetValue0ThenCall;
    Value2(Engine_TaskAddCallback, callback_a, 3200);
    callback_b = (s32)FieldScene_RunStep17;
    Call2(Engine_TaskAddCallback, callback_b, 3200);
    Actor_SetSpeed(23, 0x3333, 0x1999);
    Actor_MoveToAndWait(23, 390, 832);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_MoveToAndWait(23, 400, 826);
    Engine_EventWait(20);
    {
        u8 *record = Actor_Get(ACTOR_PARTY_LEADER);
        u8 value = *(volatile u8 *)&record[35]; /* Keeps the byte in its own register. */

        record[35] = (u8)(value | 1);
    }
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Scheduler_RemoveCallback((s32)Effect_PlayStepSound);
    Scheduler_RemoveCallback(callback_a);
    Scheduler_RemoveCallback(callback_b);
    Engine_TaskWait(1);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_SetChildValue(23, 0);
    Actor_SetPosition(23, 0, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
    Engine_ActorEnableActionCallback(0, (u32)gLeaderHammerAction);
    Engine_EventWait(120);
    Map_CopyCellsTo(7, 102, 84, 41, 2, 1);
    Engine_ActorEnableActionCallback(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 17);
    Call2(Engine_TaskAddCallback, (s32)Effect_PlayStepSound, 3200);
    for (i = 0; i < 40; i++) {
        OverlayObject_UpdateOnFrameBit1(scene);
        Engine_TaskWait(1);
    }
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
    callback_c = (s32)SceneState_SetValue0ThenCall;
    Value2(Engine_TaskAddCallback, callback_c, 3200);
    callback_d = (s32)SceneState_SetValue24ThenCall;
    Call2(Engine_TaskAddCallback, callback_d, 3200);
    Actor_SetSpeed(24, 0x3333, 0x1999);
    Actor_MoveToAndWait(24, 390, 832);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_MoveToAndWait(24, 377, 828);
    Engine_EventWait(20);
    {
        u8 *record = Actor_Get(ACTOR_PARTY_LEADER);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Scheduler_RemoveCallback((s32)Effect_PlayStepSound);
    Scheduler_RemoveCallback(callback_c);
    Scheduler_RemoveCallback(callback_d);
    Engine_TaskWait(1);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_SetChildValue(24, 0);
    Actor_SetPosition(24, 0, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
    Engine_ActorEnableActionCallback(0, (u32)gLeaderHammerAction);
    Engine_EventWait(120);
    Map_CopyCellsTo(6, 102, 83, 41, 1, 1);
    Engine_ActorEnableActionCallback(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
    Actor_FaceDirection(ACTOR_DORA, 0xb000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 17);
    Call2(Engine_TaskAddCallback, (s32)Effect_PlayStepSound, 3200);
    for (i = 0; i < 40; i++) {
        OverlayObject_UpdateOnFrameBit1(scene);
        Engine_TaskWait(1);
    }
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
    callback_e = (s32)SceneState_SetValue0ThenCall;
    Value2(Engine_TaskAddCallback, callback_e, 3200);
    callback_f = (s32)SceneState_SetValue25ThenCall;
    Engine_TaskAddCallback(callback_f, 3200);
    Actor_SetSpeed(25, 0x3333, 0x1999);
    Actor_MoveToAndWait(25, 390, 832);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_MoveToAndWait(25, 360, 837);
    Engine_EventWait(20);
    {
        u8 *record = Actor_Get(ACTOR_PARTY_LEADER);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Scheduler_RemoveCallback((s32)Effect_PlayStepSound);
    Scheduler_RemoveCallback(callback_e);
    Scheduler_RemoveCallback(callback_f);
    Engine_TaskWait(1);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_SetChildValue(25, 0);
    Actor_SetPosition(25, 0, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
    Engine_ActorEnableActionCallback(0, (u32)gLeaderHammerAction);
    Engine_EventWait(120);
    FieldScene_RunSingleStep();
    Map_CopyCellsTo(5, 103, 82, 42, 1, 1);
    Engine_ActorEnableActionCallback(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Engine_ActorJump(ACTOR_DORA, 2, 20);
    Engine_EventSetMessage((s32)MsgHaidiaGoodJob);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x1000, 10);
    HaidiaMura_RunWalkScene032B0(21, 5, 6, 0);
    Actor_SetSpeed(ACTOR_DORA, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(ACTOR_DORA, 397, 832);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_DORA, 0x4000, 60);
    Actor_FaceDirection(ACTOR_DORA, 0xc000, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_DORA, 0x4000, 40);
    Actor_FaceDirection(ACTOR_DORA, 0x8000, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, 0x5000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
    Event_OpenMessage(ACTOR_DORA, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
        bump_step(1);
    } else {
        Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
    }
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_EventSetMessage((s32)MsgHaidiaWorkingYourselvesBone);
    Actor_WalkToAndWait(ACTOR_DORA, 386, 841);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_DORA, 0xd000, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, 0x5000, 30);
    Event_OpenMessage(ACTOR_DORA, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, 0xd000, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventSetMessage((s32)MsgHaidiaDevastatedWhenKyle);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_WalkToAndWait(ACTOR_DORA, 386, 825);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
    Engine_EventWait(60);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
    Actor_FaceDirection(ACTOR_DORA, 0x5000, 10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
    Actor_FaceDirection(ACTOR_DORA, 0x5000, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x1790000, 0xa00000, 0x35c0000, 1);
    Actor_SetSpeed(ACTOR_JASMINE, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_GERALD, 369, 904);
    Actor_WalkToAndWait(ACTOR_JASMINE, 392, 904);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    HaidiaMura_RunWalkScene032B0(5, 10, 11, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 0);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_DORA, 0x3000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x1000, 20);
    Engine_ActorJump(ACTOR_JASMINE, 4, 0);
    Actor_WalkToAndWait(ACTOR_JASMINE, 392, 843);
    Actor_FaceDirection(ACTOR_JASMINE, 0x9000, 0);
    Actor_FaceDirection(ACTOR_DORA, 0x3000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 20);
    Engine_ActorSetAnimationAndWait(21, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
    Engine_ActorSetAnimation(ACTOR_DORA, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    HaidiaMura_RunWalkScene032B0(1, 10, 11, 0);
    Actor_SetSpeed(ACTOR_JASMINE, 0x4ccc, 0x2666);
    Actor_SetSpeed(ACTOR_GERALD, 0x4ccc, 0x2666);
    Actor_WalkTo(ACTOR_GERALD, 392, 843);
    ((u8 *)Object_GetById(5))[90] &= 0xfe;
    Actor_WalkToAndWait(ACTOR_JASMINE, 408, 843);
    Engine_EventWait(1);
    {
        u8 *record = Actor_Get(ACTOR_JASMINE);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 0);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 30);
    Engine_ActorJump(ACTOR_DORA, 4, 30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_DORA, 0x5000, 20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 30);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 258);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_DORA, 0x3000, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_ShowEmote(ACTOR_DORA, 257, 80);
    Actor_FaceDirection(ACTOR_DORA, 0x5000, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
    Actor_FaceDirection(ACTOR_DORA, 0x3000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(80);
    Engine_ActorFaceEachOther(ACTOR_JASMINE, ACTOR_GERALD, 30);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_DORA, 261, 60);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0x1750000, 0xa00000, 0x3450000, 1);
    Actor_WalkToAndWait(ACTOR_DORA, 364, 816);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    Actor_FaceDirection(ACTOR_DORA, 0x3000, 10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 40);
    Engine_ActorFaceEachOther(ACTOR_JASMINE, ACTOR_GERALD, 30);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 5, 0x8000, 20);
    Actor_FaceDirection(ACTOR_DORA, 0x5000, 20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 261, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
    Event_OpenMessage(ACTOR_DORA, 0);
    none = 0; /* One zero shared by the placement call and the byte store. */
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_EventSetMessage((s32)MsgHaidiaAsStubbornAsYourFather);
    Actor_ShowEmote(ACTOR_DORA, 259, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Engine_ActorJump(ACTOR_DORA, 4, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
    Engine_ActorSetAnimation(ACTOR_DORA, 7);
    Engine_EventWait(5);
    Call11(Engine_EventShowTwoMessagesAndWait, 21, 14, 2, 24, 2, 1, 10, 14, 4, 14, none);
    Audio_PlayCue(161);
    rec = Actor_Get(ACTOR_DORA);
    {
        u8 value = *(volatile u8 *)&rec[90];

        *(u8 *)(*(s32 *)(rec + 80) + 38) = none;
        rec[90] = (u8)(value & 0xfe);
    }
    Actor_SetSpeed(ACTOR_DORA, 0x30000, 0x18000);
    Actor_WalkToAndWait(ACTOR_DORA, 364, 815);
    Engine_EventWait(4);
    for (i = 0; i != 4; i++) {
        *(s32 *)(rec + 16) += 0x18000;
        *(s32 *)(rec + 28) += -0x1999;
        Engine_EventWait(1);
    }
    Actor_SetPosition(ACTOR_DORA, 0, 0);
    Actor_SetSpeed(ACTOR_GERALD, 0x30000, 0x18000);
    Engine_ActorJump(ACTOR_GERALD, 6, 0);
#if defined(TBS_EDITION_EN)
    Actor_WalkToAndWait(ACTOR_GERALD, 374, 827);
#else
    /* The localized repair scene commits Gerald's landing two pixels
       further south before the conversation resumes. */
    Actor_WalkToAndWait(ACTOR_GERALD, 374, 829);
    Actor_SetPosition(ACTOR_GERALD, PIXELS(374), PIXELS(829));
#endif
    Event_ShowMessage(ACTOR_JASMINE, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xb000, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 256, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_ShowEmote(ACTOR_GERALD, 256, 10);
    Engine_ActorSetAnimation(ACTOR_GERALD, 13);
    Engine_ActorJump(ACTOR_GERALD, 2, 5);
    Audio_PlayCue(143);
    Work_SetValuesIfNonNegative(0, 0x40000, 0x10000);
    Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_GERALD), 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 3);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Actor_ShowEmote(ACTOR_GERALD, 258, 80);
    Engine_ActorSetAnimation(ACTOR_DORA, 8);
    *(s32 *)(rec + 28) = 0x8000;
    Actor_SetPosition(ACTOR_DORA, 0x16c0000, 0x32b0000);
    for (i = 0; i != 5; i++) {
        *(s32 *)(rec + 28) += 0x1999;
        Engine_EventWait(1);
    }
    none = 0; /* Refreshed after the loops for the closing scene store. */
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 30);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Camera_SetSpeed(0x4ccc, 0x999);
    Camera_MoveTo(0x1740000, 0xa00000, 0x35b0000, 1);
    Actor_SetSpeed(ACTOR_DORA, 0x30000, 0x18000);
    Engine_ActorJump(ACTOR_DORA, 6, 0);
    Actor_WalkToAndWait(ACTOR_DORA, 359, 835);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_DORA, 0x4000, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    rec[35] &= 0xfe;
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 80);
    Actor_ShowEmote(ACTOR_DORA, 257, 80);
    Actor_FaceDirection(ACTOR_DORA, 0, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_SetAttachedEffect(ACTOR_DORA, 258);
    Engine_EventWait(80);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 40);
    Actor_ShowEmote(ACTOR_GERALD, 258, 80);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_GERALD), 1);
    Engine_ActorJump(ACTOR_GERALD, 6, 0);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Actor_SetSpeed(ACTOR_GERALD, 0x40000, 0x20000);
    rec = Actor_Get(ACTOR_GERALD);
    rec[90] &= 0xfe;
#if defined(TBS_EDITION_EN)
    Actor_SetDestination(ACTOR_GERALD, 403, 827);
#else
    Actor_SetDestination(ACTOR_GERALD, 403, 829);
#endif
    Actor_SetAttachedEffect(ACTOR_JASMINE, 258);
    Actor_FaceDirection(ACTOR_JASMINE, 0xc000, 20);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 1);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 256, 0);
    Engine_ActorSetAnimation(ACTOR_GERALD, 13);
    Engine_ActorJump(ACTOR_GERALD, 2, 5);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_GERALD), 0);
    Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
    Audio_PlayCue(143);
    Work_SetValuesIfNonNegative(0, 0x40000, 0x10000);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Actor_ShowEmote(ACTOR_GERALD, 258, 30);
    Actor_SetSpeed(ACTOR_JASMINE, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(ACTOR_JASMINE, 408, 855);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_ShowEmote(ACTOR_DORA, 261, 60);
    Engine_ActorStartRepeatedMotion(ACTOR_JASMINE, 3);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(80);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 4);
    Engine_EventWait(80);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_DORA, 0, 60);
    Actor_FaceDirection(ACTOR_DORA, 0x4000, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_DORA, 0, 80);
    Actor_ShowEmote(ACTOR_DORA, 261, 80);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 257, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 257, 0);
    Actor_ShowEmote(ACTOR_GERALD, 257, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 60);
    Actor_FaceDirection(ACTOR_DORA, 0x4000, 30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, 0, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    Actor_ShowEmote(ACTOR_DORA, 256, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_GERALD), 0);
    Engine_ActorJump(ACTOR_GERALD, 4, 0);
    Engine_ActorWalkToAndWait(1, 398, 828);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_DORA, 0x4000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
    Engine_EventWait(60);
    rec = Actor_Get(ACTOR_GERALD);
    flag = 1; /* One shared mark bit for the three record flags. */
    rec[90] |= flag;
    rec = Actor_Get(ACTOR_JASMINE);
    rec[90] |= flag;
    rec = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_JASMINE, 0x10000, 0x8000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_WalkTo(ACTOR_JASMINE, *(s16 *)(rec + 10) + 16, *(s16 *)(rec + 18));
    Actor_WalkToAndWait(ACTOR_GERALD, *(s16 *)(rec + 10) + 16, *(s16 *)(rec + 18) - 16);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 30);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_JASMINE, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(40);
    Actor_WalkToAndWait(ACTOR_JASMINE, *(s16 *)(rec + 10), *(s16 *)(rec + 18));
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, *(s16 *)(rec + 10), *(s16 *)(rec + 18));
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Engine_PartyAddMembers(1, 5);
    Camera_MoveTo(0x1790000, 0xa00000, 0x3770000, 1);
    HaidiaMura_RunWalkScene03380(0, 13, 10, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    {
        u8 *record = Actor_Get(ACTOR_DORA);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | flag);
    }
    HaidiaMura_RunWalkScene03380(21, 6, 5, 0);
    Actor_WalkToAndWait(ACTOR_DORA, 373, 887);
    Actor_FaceDirection(ACTOR_DORA, 0x4000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 40);
    Engine_ActorSetAnimation(ACTOR_DORA, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(100);
    Map_CopyCellAttributes(49, 46, 8, 4, 20, 50);
    GameFlag_Set(514);
    GameFlag_Clear(303);
    scene[85] = 3;
    *(s32 *)(scene + 12) = 0xa00000;
    *(s32 *)(scene + 60) = 0x80000000;
    *(s32 *)(scene + 40) = none;
    Engine_EventEnd();
}

void FieldScene_RunStep8C(void)
{
    Engine_PsynergyBegin(0x8c, 0);
}

void FieldScene_RunSingleStep(void)
{
    BattleEffect_CleanupSceneObjects();
}

void SceneState_SetValue1ThenCall(void)
{
    Actor_Get(ACTOR_GERALD);
    SceneEffect_UpdateObjectOnOddFrames();
}

void SceneState_SetValue0ThenCall(void)
{
    Actor_Get(ACTOR_PARTY_LEADER);
    SceneEffect_UpdateObjectOnOddFrames();
}

void FieldScene_RunStep9(void)
{
    Actor_Get(9);
    SceneEffect_UpdateObjectOnOddFramesOnly();
}

void FieldScene_RunStep17(void)
{
    Actor_Get(0x17);
    SceneEffect_UpdateObjectOnOddFramesOnly();
}

void SceneState_SetValue24ThenCall(void)
{
    Actor_Get(0x18);
    SceneEffect_UpdateObjectOnOddFramesOnly();
}

void SceneState_SetValue25ThenCall(void)
{
    Actor_Get(0x19);
    SceneEffect_UpdateObjectOnOddFramesOnly();
}

s32 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 HaidiaMura_TestFacing(struct FieldActor *obj, struct FieldActor *target, s32 range, s32 force)
{
    s32 result;
    u32 angle;
    u32 left;
    u32 right;
    u32 dir;

    result = 0;
    if (obj->unknown_5b == 1) {
        if (obj->rise_counter == 0) {
            Object_SetMode(obj, 1);
            return 1;
        }
    }
    if (Runtime_ComputeFixedPointDistance(&target->x.fixed, &obj->x.fixed) < range || force != 0) {
        angle = (u16)ArcTan2(target->z.fixed - obj->z.fixed,
                                target->x.fixed - obj->x.fixed);
        left = (angle - 0x1000) & 0xf000;
        right = (angle + 0x1000) & 0xf000;
        angle &= 0xf000;
        dir = obj->facing & 0xf000;
        if (angle == dir || right == dir || left == dir || force != 0) {
            /* FAKEMATCH: plain-byte publication avoids synthetic QI masks. */
            *(u8 *)&obj->unknown_5b = 1;
            Object_SetMode(obj, 1);
            result = 1;
            *(u8 *)&obj->rise_counter = result;
        } else {
            *(u8 *)&obj->unknown_5b = 0;
            Object_SetMode(obj, 2);
            *(u8 *)&obj->rise_counter = 0;
        }
    } else {
        *(u8 *)&obj->unknown_5b = 0;
        Object_SetMode(obj, 2);
        *(u8 *)&obj->rise_counter = 0;
    }
    return result;
}

s32 SceneActor_RunStep18WhenTargetSet(s32 *p)
{
    struct FieldActor *t = Actor_Get(ACTOR_PARTY_LEADER);
    if (p[14] == (s32)0x80000000 && p[16] == (s32)0x80000000)
        return 0;
    HaidiaMura_TestFacing((struct FieldActor *)p, t, 18, 0);
    return 0;
}

void Effect_ConfigureSpawnedParticle(struct SourceEntity *source)
{
    s32 spawn_position[3];
    s32 particle_index;
    struct StagedParticle *particle;
    spawn_position[0] = source->f08;
    spawn_position[1] = source->f0c - (((s32 (*)())Engine_RandomNext)(source) << 4) + (s32)0xfff80000;
    spawn_position[2] = source->f10;
    particle_index = Random_Next();
    Vector_AddPolarOffset(((particle_index << 1) + particle_index) << 4, Random_Next(), spawn_position);
    particle = Engine_ObjectCreate(0x11d, spawn_position[0], spawn_position[1], spawn_position[2]);
    if (particle != 0) {
        particle->f55 = 2;
        particle->f48 = 0x1999;
        particle->f5e = 12;
        Engine_ActorSetSpriteFlags(particle, 0);
        Object_SetMode(particle, 0);
        Object_SetScript(particle, (s32)gDustBurstScript);
        {
            struct ParticleRecord *record = particle->f50;
            s32 record_flags = ~12;
            record_flags &= record->f09;
            record_flags |= 4;
            record->f09 = record_flags;
        }
    }
    Audio_PlayCue(0x8a);
}

void Effect_SpawnRisingDustBurst(struct Resource373Emitter *emitter)
{
    s32 frame_countdown;

    Audio_PlayCue(154);

    for (frame_countdown = 30; frame_countdown >= 0; frame_countdown--) {
        emitter->y += 0x10000;              /* 0x80 << 9. */
        emitter->field06 = (u16)(emitter->field06 + 0x2000);  /* 0x80 << 6. */
        emitter->field18 += -2048;          /* The pool word 0xfffff800. */
        emitter->field1c += -2048;
        Engine_TaskWait(1);
    }

    for (frame_countdown = 7; frame_countdown >= 0; frame_countdown--) {
        struct Resource373Particle *particle =
            Engine_ObjectCreate(0x11d, emitter->x, emitter->y, emitter->z);

        if (particle != 0) {
            s32 vertical_speed;

            Engine_ActorSetSpriteFlags(particle, 0);
            Object_SetScript(particle, (const void *)&gDustBurstScript[1]);

            vertical_speed = Random_Next() + 0x10000;
            particle->field34 = 0x10000;
            particle->field30 = vertical_speed;
            particle->field55 = 2;
            particle->field48 = 0x0a3d;

            particle->lifetime = Random_Next() - Random_Next();

            Effect_UpdateParticlePosition(
                particle,
                ((Random_Next() * 3) << 3) + 0x80000,
                Random_Next());
        }
    }

    Audio_PlayCue(131);

    emitter->x = 0;
    emitter->y = 0;
    emitter->z = 0;
    emitter->field38 = (s32)0x80000000;
    emitter->field3c = (s32)0x80000000;
    emitter->field40 = (s32)0x80000000;
    emitter->field24 = 0;
    emitter->field28 = 0;
    emitter->field2c = 0;
}

void Effect_UpdateParticlePosition(s32 *particle, s32 delta_x, s32 delta_z)
{
    s32 position[3];
    if (particle != 0) {
        position[0] = particle[2];
        position[1] = particle[3];
        position[2] = particle[4];
        Vector_AddPolarOffset(delta_x, delta_z, position);
        Object_SetPosition((s32)particle, position[0], position[1], position[2]);
    }
}

void SceneState_ApplyRectAndRunTwo(void)
{
    s32 e = 22;
    s32 f = 36;
    Map_CopyCellAttributes(17, 0, 3, 1, e, f);
    StagedActor_AdvancePair();
    HaidiaMura_OpenVillagerLane();
}

/* Opens the lane beside whichever villager stands in it: the cells copied depend on the villager's column, and the matching flag is set. */
void HaidiaMura_OpenVillagerLane(void)
{
    struct FieldActor *actor;
    s32 column;

    Call6((void (*)())Engine_MapCopyCellAttributes, 17, 0, 3, 1, 22, 36);
    if (Engine_GameFlagIsSet(0x87a))
        actor = Object_GetById(21);
    else
        actor = Object_GetById(20);
    if (actor == NULL)
        return;
    Call1((void (*)())Engine_GameFlagClear, 0x314);
    Call1((void (*)())Engine_GameFlagClear, 0x315);
    Call1((void (*)())Engine_GameFlagClear, 0x316);
    column = actor->x.fixed >> 20;
    if (column == 22) {
        Engine_MapCopyCellAttributes(17, 1, 1, 1, column, 36);
        Engine_GameFlagSet(0x314);
    } else if (column == 23) {
        Engine_MapCopyCellAttributes(17, 1, 1, 1, column, 36);
        Engine_GameFlagSet(0x315);
    } else {
        Call6((void (*)())Engine_MapCopyCellAttributes, 17, 1, 1, 1, 24, 36);
        Engine_GameFlagSet(0x316);
    }
}

void Effect_PlayStepSound(void)
{
    if ((*(u32 *)&gFrameCount & 15) == 0)
        Audio_PlayCue(0x83);
}

void FieldScene_RunScriptedStepEE4(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgHaidiaDoorWontOpen, 1);
    Engine_EventEnd();
}

void FieldScene_RunScene373SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    if (GameFlag_IsSet(0x241) != 0) {
        rec7 = GameFlag_IsSet(0x106);
        if (rec7 != 0) {
            goto L_02005a8a;
        }
        *(u8 *)((u8 *)Object_GetById(22) + 91) = rec7;
        GameFlag_Clear(0x241);
    } else {
        if (GameFlag_IsSet(0x106) != 0) {
            *(u8 *)((u8 *)Object_GetById(22) + 91) = 1;
            GameFlag_Set(0x241);
        }
    }
    L_02005a8a:;
}

void SceneActor_SetFlagByteBySlotZeroPosition(void)
{
    s32 *g = Actor_Get(ACTOR_PARTY_LEADER);
    u8 *q;
    if (GameFlag_IsSet(0x87a) != 0)
        q = Actor_Get(21);
    else
        q = Actor_Get(20);
    if (q != 0) {
        if (g[3] > 0xc80000)
            q[0x23] = 3;
        else
            q[0x23] = 1;
    }
}

s32 SceneEffect_UpdateOrbitPosition(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Engine_MathSin(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Engine_MathCos(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Engine_MathCos(p[12] + 0x8000) / 8;
    a = Random_Next();
    b = Random_Next();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

void InitializeStagedActorSceneOrbitingEffect(s32 id)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = (OrbitingSceneObject *)Object_GetById(id);
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    Engine_ActorSetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (GameFlag_IsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = Runtime_AllocateHeapBlock(17, 0x608);
    Engine_ItemLoadIcon(ITEM_NUT);
    transfer += 0x400;
    Engine_VramLoad(sprite->palette, 128, transfer);
    Engine_HeapRelease(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)SceneEffect_UpdateOrbitPosition;
    actor->state = zero;
}

void OverlayObject_UpdateOnFrameBit1(s32 p)
{

    if ((gFrameCount & 2) != 0)
        Engine_ObjectSetPartPalettes(p, 7);
    else
        Engine_ObjectSetPartPalettes(p, 0);
    if ((gFrameCount & 0xf) == 0)
        WorldMap_CreateLinkedEffects(p);
}

void SceneEffect_UpdateObjectOnOddFrames(s32 p)
{
    extern volatile u32 gFrameCount;

    if ((gFrameCount & 1) != 0)
        Engine_ObjectSetPartPalettes(p, (gFrameCount >> 1) % 6);
    if ((gFrameCount & 0xf) == 0)
        WorldMap_CreateLinkedEffects(p);
}

void SceneEffect_UpdateObjectOnOddFramesOnly(s32 p)
{
    extern volatile u32 gFrameCount;

    if ((gFrameCount & 1) != 0)
        Engine_ObjectSetPartPalettes(p, (gFrameCount >> 1) % 6);
}

void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Engine_ObjectDispatchRelease((s32)effect);
    } else {
        s32 amplitude = Engine_MathSin(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] + offset * 5 + 0x80000;
    }
}

void Effect_AnimateVerticalNegative(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Engine_ObjectDispatchRelease((s32)effect);
    } else {
        s32 amplitude = Engine_MathSin(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = -amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] - offset * 5 + 0x100000;
    }
}
