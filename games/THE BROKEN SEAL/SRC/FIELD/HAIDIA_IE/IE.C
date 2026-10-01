/* Facing, scene tables and the villagers' first lines. */
#include "HAIDIA.H"
#include "MAP_SCROLL.H"
#include "CALL.H"
#include "text/MSG_IDS.H"

TEXT_MESSAGE_ENUM(MsgHaidiaWhysEveryoneHanging);
TEXT_MESSAGE_ENUM(MsgHaidiaFarewell);

extern u8 MsgHaidiaADifficultTimeThreeYears[];
extern u8 MsgHaidiaCheckedThePsynergyStone[];
extern u8 MsgHaidiaDidTheTravelersMeetThe[];
extern u8 MsgHaidiaMeditateOnMtAlephDaily[];
extern u8 MsgHaidiaPartyPpRestored[];

extern u8 MsgHaidiaArentWorriedCrossing[];
extern u8 MsgHaidiaBeholdPowerPsynergy[];
extern u8 MsgHaidiaShownNewAbility[];

extern u8 MsgHaidiaAnythingInterestingOnYourTrip[];
extern u8 MsgHaidiaCanIUsePsynergy[];
extern u8 MsgHaidiaIHaveSomePsynergyLeft[];
extern u8 MsgHaidiaTheStoneFellOnThe[];
extern u8 MsgHaidiaYouSawTheWiseOne[];

extern u8 MsgHaidiaSureHelp[];
extern u8 MsgHaidiaWentOffWay[];

extern u8 MsgHaidiaGoAidElders[];
extern u8 MsgHaidiaOnlyTwoSurvived[];
extern u8 MsgHaidiaThePsynergyStoneIsGone[];
extern u8 MsgHaidiaThisIsVale[];
extern u8 MsgHaidiaYouCameBackHome[];

/* The four actors' closing actions, where the overlay's data lies. */
extern u8 HaidiaIe_Actor23Actions[];
extern u8 HaidiaIe_Actor24Actions[];
extern u8 HaidiaIe_Actor25Actions[];
extern u8 HaidiaIe_LeaderActions[];

s32 Object_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 facing_delta;
    u16 old_facing;
    s32 target_facing;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        target_facing = (u16)ArcTan2(target->position_z - object->position_z, target->position_x - object->position_x);
        old_facing = object->facing;
        facing_delta = (s16)(target_facing - old_facing);
        if (facing_delta != 0) {
            if (facing_delta > 0x1000) {
                facing_delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (facing_delta < -0x1000) {
                facing_delta = -0x1000;
            }
            object->facing = (u16)(old_facing + facing_delta);
        }
    }
    return 1;
}

u8 *SceneData_GetTableAfa0(void)
{
    return Data_0200afa0;
}

s32 Func_02000090(void)
{
    return 0;
}

void *SceneData_SelectTableByFlag834(void)
{
    if (GameFlag_IsSet(0x834) != 0) {
        return Data_0200b144;
    }
    return Data_0200b108;
}

void *SceneData_SelectTableByFlags834And87a(void)
{
    if (GameFlag_IsSet(0x834) != 0) {
        return Data_0200b380;
    }
    if (gGameState.entrance == 12) {
        return Data_0200b560;
    }
    if (GameFlag_IsSet(0x87a) != 0) {
        return Data_0200b7d0;
    }
    return Data_0200b170;
}

void Scene_CheckPsynergyStone(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgHaidiaCheckedThePsynergyStone, 1);
    Audio_PlayCue(126);
    BattleParty_ApplyDrain(0x3e7, 0);
    Engine_EventWait(10);
    Engine_MessageShowCentered((s32)MsgHaidiaPartyPpRestored, 1);
    UiWork_FinalizePendingCore();
    GameFlag_Clear(322);
    Engine_EventEnd();
}

void *SceneData_SelectTableByFlags87a_815_834(void)
{
    if (GameFlag_IsSet(0x87a) != 0) {
        return Data_0200bcec;
    }
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        return Data_0200bb3c;
    }
    if (gGameState.entrance == 12) {
        return Data_0200bb30;
    }
    if (GameFlag_IsSet(0x834) != 0) {
        return Data_0200ba64;
    }
    return Data_0200b938;
}

void Villager_AskAboutMeditation(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaMeditateOnMtAlephDaily);
    Engine_ActorFaceEachOther(23, ACTOR_PARTY_LEADER, 2);
    Event_AskYesNo(23, 0);
    Engine_EventEnd();
}

void Villager_RecallThreeYearsAgo(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaADifficultTimeThreeYears);
    Engine_ActorFaceEachOther(24, ACTOR_PARTY_LEADER, 2);
    Event_AskYesNo(24, 0);
    Engine_EventEnd();
}

void Villager_AskAboutTheTravelers(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaDidTheTravelersMeetThe);
    Engine_ActorFaceEachOther(15, ACTOR_PARTY_LEADER, 2);
    Event_AskYesNo(15, 0);
    Engine_EventEnd();
}

/* The villager who shows off his psynergy by shaking the view. */

/* Once the party has left the vale he only asks about the journey, one line
 * further for each of the two companions; before, he lifts the view up and
 * down for three seconds. */
void Villager_ShowOffPsynergy(void)
{
    u32 i;
    s32 message;
    s32 shakes;
    s32 *origin;

    Engine_EventBegin();
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        message = (s32)MsgHaidiaArentWorriedCrossing;
        Engine_EventSetMessage(message);
        if (GameFlag_IsSet(2) != 0) {
            bump_step(1);
        }
        if (GameFlag_IsSet(3) != 0) {
            bump_step(1);
        }
        Event_OpenMessage(17, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventSetMessage(message + 3);
        } else {
            Engine_EventSetMessage(message + 4);
        }
        Event_ShowMessage(17, 0);
    } else {
        origin = gCam->origin;
        Engine_EventSetMessage((s32)MsgHaidiaShownNewAbility);
        Engine_ActorFaceEachOther(17, ACTOR_PARTY_LEADER, 0);
        Event_AskYesNo(17, 0);
        Engine_EventWait(20);
        Engine_ActorStartRepeatedMotion(17, 2);
        Engine_EventWait(15);
        SceneState_ApplyPair140And0();
        shakes = 0;
        for (i = 0; i < 40; i++) {
            OverlayObject_UpdateOnFrameBit1((s32)Object_GetById(17));
            Engine_TaskWait(1);
        }
        Scheduler_AddOrUpdateCallback((s32)FieldScene_RunStep17, 0xc80);
        Audio_PlayCue(107);
        for (i = 0; i != 180; i++) {
            if (Math_RemainderUnsigned(i, 10) == 0) {
                if ((1 & shakes) != 0) {
                    *origin -= 0x10000;
                } else {
                    *origin += 0x10000;
                }
                shakes = shakes + 1;
            }
            Engine_EventWait(1);
        }
        Audio_PlayCue(0x121);
        Scheduler_RemoveCallback((s32)FieldScene_RunStep17);
        Engine_TaskWait(1);
        FieldScene_Forward4dac();
        Actor_SetChildValue(17, 0);
        Engine_EventWait(40);
        Engine_EventSetMessage((s32)MsgHaidiaBeholdPowerPsynergy);
        Event_ShowMessage(17, 0);
    }
    Engine_EventEnd();
}

/* The psynergy stone falling on the hut. */
void SceneDialogue_RunFlagGatedMessageStep(void)
{
    Engine_EventBegin();
    if (GameFlag_IsSet(0x87a) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaAnythingInterestingOnYourTrip);
        Event_OpenMessage(15, 0);
        if (Engine_EventChooseYesNo(0, 0) == 1) {
            Event_ShowMessage(15, 0);
        } else {
            u8 *p = (u8 *)gEventWork;
            *(u16 *)(p + 472) = *(u16 *)(p + 472) + 1;
            Event_AskYesNo(15, 0);
        }
    } else if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaYouSawTheWiseOne);
        Event_AskYesNo(11, 0);
    } else {
        Engine_EventSetMessage((s32)MsgHaidiaCanIUsePsynergy);
        Event_AskYesNo(11, 0);
    }
    Engine_EventEnd();
}

void Scene_StoneFellOnTheHut(void)
{
    Engine_EventBegin();
    Engine_ActorSetAnimation(26, 1);
    Actor_FaceActor(26, ACTOR_PARTY_LEADER, 20);
    Actor_FaceActor(26, 21, 40);
    Engine_EventSetMessage((s32)MsgHaidiaTheStoneFellOnThe);
    Event_SayThenWait(26, 20);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x1510000, -1, 0x1100000, 1);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(26, 2);
    Engine_EventWait(20);
    Actor_FaceActor(26, ACTOR_PARTY_LEADER, 10);
    Event_SayThenWait(26, 40);
    Engine_ActorEnableActionCallback(26, 2);
    Engine_EventEnd();
}

void FieldScene_RunMiddleAuxiliarySequence(void)
{
    u32 i;
    s32 p8;
    u8 *rec8;
    s32 record;
    s32 v2;

    Engine_EventBegin();
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 82, 0x2f8);
    Engine_ActorFaceEachOther(15, ACTOR_PARTY_LEADER, 30);
    Engine_EventSetMessage((s32)MsgHaidiaIHaveSomePsynergyLeft);
    Event_SayThenWait(15, 20);
    SceneActor_SetPairZeroAndValue(15, 0xa000, 20);
    Actor_SetAttachedEffect(15, 0x102);
    Engine_EventWait(20);
    SceneState_ApplyPair140And0();
    for (i = 0; i < 40; i++) {
        OverlayObject_UpdateOnFrameBit1(Object_GetById(15));
        Engine_TaskWait(1);
    }
    Value2(Scheduler_AddOrUpdateCallback, (s32)FieldScene_RunStep15, 0xc80);
    Scheduler_AddOrUpdateCallback((s32)FieldScene_RunStep20, 0xc80);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 10);
    rec8 = Object_GetById(20);
    v2 = rec8[85];
    rec8[85] = 0;
    p8 = v2;
    for (i = 0; i < 40; i++) {
        *(s32 *)(rec8 + 12) += 0x1800;
        Engine_TaskWait(1);
    }
    rec8[85] = p8;
    Scheduler_RemoveCallback((s32)FieldScene_RunStep15);
    Scheduler_RemoveCallback((s32)FieldScene_RunStep20);
    Engine_TaskWait(1);
    Audio_PlayCue(161);
    Actor_SetChildValue(15, 0);
    Actor_SetChildValue(20, 0);
    Engine_EventWait(40);
    FieldScene_Forward4dac();
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 15, 30);
    Event_ShowMessage(15, 0);
    Engine_EventEnd();
}

/* The villager who points the way the others went. */
void Villager_PointTheWay(void)
{
    Engine_EventBegin();
    Engine_ActorFaceEachOther(16, ACTOR_PARTY_LEADER, 10);
    if (GameFlag_IsSet(0x840) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaSureHelp);
        Event_ShowMessage(16, 0);
    } else {
        Engine_EventSetMessage((s32)MsgHaidiaWentOffWay);
        Event_ShowMessage(16, 0);
    }
    Engine_EventEnd();
}

/* Return visits, supplemental sequences and the entry state. */
void Villager_WelcomeBack(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x302) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaThisIsVale);
    } else {
        Engine_EventSetMessage((s32)MsgHaidiaYouCameBackHome);
        GameFlag_Set(0x302);
    }
    Event_ShowMessage(11, 0);
    Engine_EventEnd();
}

void Scene_PsynergyStoneIsGone(void)
{
    struct Obj *p = Object_GetById(21);
    Engine_EventBegin();
    p->f38 = 0x80000000;
    p->f3c = 0x80000000;
    p->f40 = 0x80000000;
    Engine_ActorSetAnimation(21, 1);
    Engine_ActorStop(21);
    Actor_ShowEmote(21, 256, 40);
    p->f06 = 0xb000;
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(21, 2);
    Engine_EventSetMessage((s32)MsgHaidiaThePsynergyStoneIsGone);
    Event_ShowMessageAndWait(21, 0, 40);
    Actor_FaceActor(21, ACTOR_PARTY_LEADER, 20);
    Engine_ActorStartRepeatedMotion(21, 2);
    Event_ShowMessage(21, 0);
    GameFlag_Set(0x306);
    Engine_ActorStop(21);
    Engine_TaskWait(1);
    Engine_ActorEnableActionCallback(21, Data_0200ae34);
    Engine_EventEnd();
}

void SceneState_SetWork1c0AndRun(s32 no)
{
    u8 *p;
    if (GameFlag_IsSet(0x834) != 0) {
        BattleFx_SetBlock30ValuesMaxZero();
    }
    p = (u8 *)gEventWork;
    *(s32 *)(p + 0x1c0) = 0x100;
    *(s32 *)(p + 0x1c8) = 16;
    Engine_EventRequestExit(no);
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value_0200beb4 as its first argument. */
void FieldScene_RunSupplementalSequenceOne(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 44, 7);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 0x117);
    SceneState_SetWork1c0AndRun(1);
}

/* Sets step 188, then runs a pair of 6-argument setup calls for indices 0
 * and 2 sharing the same trailing four values, followed by a pair of
 * 3-argument calls sharing the same leading two arguments, and a closing
 * 1-argument call. */
void FieldScene_RunSupplementalSequenceTwo(void)
{
    Audio_PlayCue(188);
    Map_CopyCellsTo(0, 63, 51, 8, 2, 2);
    Engine_TaskWait(10);
    Map_CopyCellsTo(2, 63, 51, 8, 2, 2);
    Engine_TaskWait(10);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 306);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 296);
    SceneState_SetWork1c0AndRun(2);
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value_0200beb4 as its first argument. */
void FieldScene_RunSupplementalSequenceThree(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 43, 15); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 230, 0x197);
    SceneState_SetWork1c0AndRun(3);
}

/* Runs four scene calls in sequence: a single-argument call, a call that
 * passes the address of Value_0200beb4 with two more values, a call that
 * passes 0, 374, and 0x1a3, and a final single-argument call. */
void FieldScene_RunSupplementalSequenceFour(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 52, 18); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 374, 0x1a3); /* object_id 0, x 374, z 0x1a3 */
    SceneState_SetWork1c0AndRun(4);
}

/* Runs a fixed sequence of four scripted calls: one keyed off Value_0200beb4
 * with two small numeric arguments, one with a 0x222 argument, and two plain
 * single-argument calls. */
void FieldScene_RunSupplementalSequenceFive(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 41, 32); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 200, 0x222);
    SceneState_SetWork1c0AndRun(5);
}

/* Runs four scripted calls with fixed literal arguments: a single-argument
 * call, a 3-argument call whose first argument is the address of
 * Value_0200beb4, another 3-argument call, and a closing single-argument
 * call. */
void FieldScene_RunSupplementalSequenceSix(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 35, 36); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 0x263); /* object_id 0, x 102, z 611 */
    SceneState_SetWork1c0AndRun(6);
}

/* Runs four scripted scene calls in sequence, passing a byte's address and a
 * handful of small immediate constants to each. */
void FieldScene_RunSupplementalSequenceSeven(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 51, 39); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 358, 0x29e);
    SceneState_SetWork1c0AndRun(7);
}

void FieldScene_RunStep7BThen8(void)
{
    Audio_PlayCue(123);
    SceneState_SetWork1c0AndRun(8);
}

void SceneState_ApplyFlag815Branch(void)
{
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Audio_PlayCue(123);
        SceneState_SetWork1c0AndRun(10);
    }
}

void SceneState_ApplyFlag90b(void)
{
    GameFlag_Set(0x90b);
}

void SceneState_ApplyFlag90c(void)
{
    GameFlag_Set(0x90c);
}

void SceneState_ApplyFlag90d(void)
{
    GameFlag_Set(0x90d);
}

s32 HaidiaIe_RestoreEntryState(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200ac00[];
    void HaidiaIe_RunScene015B4();
    void SceneState_SetValues352_365_2116_2117_40();
    void Scene_RunExtendedActorSequence();

    u32 i;
    s32 record;
    s32 v5;

    if (Engine_GameFlagIsSet(0x90b) != 0) {
        Engine_ActorSetPosition(8, 0, 0);
    }
    if (Engine_GameFlagIsSet(0x90c) != 0) {
        Engine_ActorSetPosition(9, 0, 0);
    }
    if (Engine_GameFlagIsSet(0x90d) != 0) {
        Engine_ActorSetPosition(10, 0, 0);
    }
    switch (gGameState.entrance) {
    case 98:
        Engine_GameFlagSet(32);
        Engine_EventRequestExit(50);
        return 0;
    case 99:
        SceneState_SetValues352_365_2116_2117_40();
        return 0;
    case 97:
        HaidiaIe_RunScene015B4();
        return 0;
    }
    v5 = 192;
    record = Object_GetById(8);
    *(s32 *)(record + 28) = (v5 << 9);
    record = Object_GetById(9);
    *(s32 *)(record + 28) = (v5 << 9);
    record = Object_GetById(10);
    *(s32 *)(record + 28) = (v5 << 9);
    if (Engine_GameFlagIsSet(0x87a) != 0) {
        Engine_MapCopyCellsTo(97, 2, 80, 5, 2, 2);
        Call6(Engine_MapCopyCellsTo, 42, 53, 42, 54, 3, 1);
        Engine_MapRedraw();
        Engine_TaskWait(1);
    } else {
        if (Engine_GameFlagIsSet(0x834) != 0) {
            Unnamed_08094ac8();
            BattleFx_StartTwelveFrameBlend();
            Call6(Scene_CopyCellAttributes, 21, 38, 1, 1, 18, 41);
            record = Engine_GameFlagIsSet(0x840);
            if (record == 0) {
                goto L_02000aaa;
            }
            Engine_ActorSetPosition(17, 0, 0);
            Engine_ActorSetPosition(18, 0, 0);
            Call3(Object_SetTargetAndCallback, 19, 0x10000, (s32)Data_0200ac00);
        } else {
            if (Engine_GameFlagIsSet(0x815) != 0) {
                Call3(Engine_ActorSetPosition, 16, 0xb40000, 0x2380000);
                Engine_MapCopyCellsTo(92, 2, 80, 5, 2, 2);
                Call6(Engine_MapCopyCellsTo, 42, 53, 42, 54, 3, 1);
                Engine_MapRedraw();
                Engine_TaskWait(1);
            }
        }
        L_02000aaa:;
        if (gGameState.entrance == 12) {
            Scene_RunExtendedActorSequence();
        } else {
            if (Engine_GameFlagIsSet(0x834) != 0) {
                record = Object_GetById(20);
                *(s32 *)(record + 24) = 0x4ccc;
                *(s32 *)(record + 28) = 0x4ccc;
                record = Object_GetById(20);
                Engine_ActorSetSpriteFlags(record, 0);
                record = Object_GetById(21);
                *(s32 *)(record + 24) = 0x9999;
                *(s32 *)(record + 28) = 0x9999;
                Engine_ActorSetAnimation(13, 5);
            } else {
                if (Engine_GameFlagIsSet(0x815) != 0) {
                    Call3(Engine_ActorSetPosition, 21, 0x14b0000, 0xf90000);
                    record = Object_GetById(21);
                    Engine_ActorSetSpriteFlags(record, 0);
                }
            }
            if (Engine_GameFlagIsSet(0x840) != 0) {
                Engine_ActorSetPosition(26, 0, 0);
                Engine_ActorSetPosition(22, 0, 0);
            }
            if (gGameState.entrance == 19) {
                HaidiaIe_RunScene015B4();
            } else {
                if (Engine_GameFlagIsSet(0x834) != 0) {
                    record = Engine_GameFlagIsSet(0x842);
                    if (record == 0) {
                        goto L_02000b6a;
                    }
                    HaidiaIe_RunScriptScene();
                } else {
                    L_02000b6a:;
                    if (Engine_GameFlagIsSet(0x834) != 0) {
                        Engine_EventOpenScreen();
                        Engine_EventWaitForScreen();
                        BattleFx_SetBlock30Values128One();
                    }
                }
            }
        }
    }
    L_02000b80:;
    return 0;
}

/* Runs once flag 0x834 is set, until this event sets flag 0x840 at its end.
 * The dialogue starts at MsgHaidiaGoAidElders, urging the party to aid the elders,
 * while the actors move, turn and animate around it. After the prompt an
 * answer of 1 shows the next reply and skips the one after it; any other
 * answer skips straight to that second reply. */
void FieldScene_RunElderAidEvent(void)
{
    extern u8 Data_0200ac00[];

    s32 record;
    s32 skip_reply = 0;
    s32 unk;

    if (Engine_GameFlagIsSet(0x834) != 0 && Engine_GameFlagIsSet(0x840) == 0) {
        Engine_EventBegin();
        Call2(Engine_CameraSetSpeed, 0x19999, 0x3333);
        Camera_MoveTo(0xc50000, -1, 0x3000000, 1);
        BattleFx_CommitObjectPositionAndWait();
        Engine_EventSetMessage((s32)MsgHaidiaGoAidElders);
        Engine_ActorRunRepeatedMotion(19, 2);
        Call3(Engine_EventShowMessageAndWait, 0x4013, 0, 10);
        Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
        Call3(Engine_ActorSetSpeed, 25, 0x10000, 0x8000);
        Call3(Engine_ActorWalkToAndWait, 0, 179, 0x315);
        record = Object_GetById(0);
        if (record != 0) {
            Engine_ActorSetPosition(25, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Call3(Engine_ActorWalkToAndWait, 25, 179, 0x324);
        Engine_ActorFaceEachOther(0, 25, 40);
        Engine_ActorFaceDirection(0, 0, 0);
        Actor_FaceDirection(25, 0, 0);
        Engine_ActorSetAnimation(17, 3);
        Engine_ActorSetAnimationAndWait(18, 3);
        Engine_ActorFaceEachOther(17, 18, 0);
        Engine_EventWait(20);
        Engine_ActorStartRepeatedMotion(17, 1);
        Engine_EventShowMessageAndWait(0x4011, 0, 10);
        Engine_ActorSetAnimation(18, 3);
        Engine_EventShowMessageAndWait(18, 0, 10);
        Engine_ActorFaceDirection(17, 0, 0);
        Call3(Engine_ActorFaceDirection, 18, 0xf000, 10);
        Engine_ActorSetAnimationAndWait(19, 3);
        Call3(Engine_EventShowMessageAndWait, 0x4013, 0, 10);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
        Engine_ActorSetSpritePriority(18, 1);
#endif
        Call3(Engine_ActorSetSpeed, 17, 0x19999, 0xcccc);
        Call3(Engine_ActorSetSpeed, 18, 0x19999, 0xcccc);
        Engine_ActorEnableActionCallback(17, (s32)Data_0200aef0);
        Engine_EventWait(20);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
        ((struct FieldActor *)Object_GetById(18))->priority_flags |= 1;
#endif
        Engine_ActorEnableActionCallback(18, (s32)Data_0200aef0);
        Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
        Call3(Engine_ActorFaceDirection, 25, 0xc000, 60);
        Engine_ActorEnableActionCallback(0, (s32)Data_0200af50);
        Object_SetActionCallbackAndRefreshById(25, (s32)Data_0200af78);
        Engine_EventWait(20);
        Engine_ActorFaceDirection(0, 0, 0);
        Engine_ActorFaceDirection(25, 0, 10);
        Engine_EventShowMessage(25, 0);
        Call3(Engine_ActorFaceDirection, 19, 0x8000, 0);
        SceneActor_SetPairZeroAndValue(26, 0x6000, 20);
        Engine_ActorRunRepeatedMotion(26, 2);
        Event_SayThenWait(26, 10);
        Engine_ActorSetAnimation(0, 3);
        Engine_ActorSetAnimationAndWait(25, 3);
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(19, 2);
        Engine_EventOpenMessage(0x4013, 0);
        if (Engine_EventChooseYesNo(0, 0) == 1) {
            skip_reply = 1;
            Engine_ActorSetAnimation(19, 4);
        } else {
            Engine_ActorSetAnimation(19, 3);
            *(u16 *)(((s32)gEventWork + 0x1d8)) += 1;
        }
        Call2(Engine_EventShowMessage, 0x4013, 0);
        if (skip_reply != 0) {
            *(u16 *)(((s32)gEventWork + 0x1d8)) += 1;
        }
        unk = 0x4000;
        SceneActor_SetPairZeroAndValue(22, 0x4000, 30);
        Engine_EventShowMessage(22, 0);
        Call3(Engine_ActorShowEmote, 19, 0x100, 0);
        Call3(Engine_ActorShowEmote, 26, 0x100, 0);
        Call3(Engine_ActorShowEmote, 0, 0x100, 0);
        Call3(Engine_ActorShowEmote, 25, 0x100, 40);
        Call3(Engine_ActorFaceDirection, 19, 0xa000, 0);
        Call3(Engine_ActorFaceDirection, 26, 0xa000, 0);
        Call3(Engine_ActorFaceDirection, 0, 0xe000, 0);
        SceneActor_SetPairZeroAndValue(25, 0xe000, 10);
        Engine_CameraSetSpeed(0x13333, 0x2666);
        Call4(Engine_CameraMoveTo, 0xd70000, -1, 0x2f60000, 1);
        BattleFx_CommitObjectPositionAndWait();
        Engine_CameraSetSpeed(0xcccc, 0x1999);
        Call4(Engine_CameraMoveTo, 0xcd0000, -1, 0x30a0000, 1);
        Engine_ActorEnableActionCallback(22, (s32)Data_0200a874);
        Object_RefreshSelectorById(22);
        /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
        Value3(SceneActor_SetPairZeroAndValue, 22, 0x2000, 60);
        Engine_ActorRunRepeatedMotion(19, 2);
        Event_SayThenWait(19, 10);
        Engine_ActorSetAnimationAndWait(22, 3);
        Event_SayThenWait(22, 20);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_EventWait(10);
        SceneActor_SetPairZeroAndValue(19, unk, 30);
        Event_SayThenWait(unk + 19, 10);
        SceneActor_SetPairZeroAndValue(26, 0xe000, 30);
        Engine_ActorSetAnimationAndWait(26, 3);
        SceneActor_SetPairZeroAndValue(19, 0x8000, 30);
        Engine_ActorRunRepeatedMotion(19, 2);
        Event_SayThenWait(unk + 19, 10);
        Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 25, 40);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        SceneActor_SetPairZeroAndValue(25, 0, 20);
        SceneActor_SetPairZeroAndValue(26, 0x8000, 30);
        Engine_ActorSetAnimationAndWait(26, 3);
        Event_SayThenWait(26, 30);
        SceneActor_SetPairZeroAndValue(26, 0xc000, 30);
        Engine_ActorSetAnimationAndWait(26, 3);
        Engine_ActorSetAnimationAndWait(22, 3);
        Engine_ActorSetAnimation(25, 2);
        record = Object_GetById(0);
        if (record != 0) {
            Engine_ActorSetDestination(25, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(25);
        Actor_SetPosition(25, 0, 0);
        Engine_ActorSetAnimation(26, 2);
        record = Object_GetById(0);
        if (record != 0) {
            Actor_SetDestination(26, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(26);
        Actor_SetPosition(26, 0, 0);
        Engine_ActorSetAnimation(22, 2);
        record = Object_GetById(0);
        if (record != 0) {
            Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(22);
        Engine_ActorSetPosition(22, 0, 0);
        Call3(Object_SetTargetAndCallback, 19, 0x10000, (s32)Data_0200ac00);
        Engine_GameFlagSet(0x840);
        Engine_EventEnd();
    }
}

/* NONMATCHING: 496 of 496 bytes, 11 halfword edits (2026-09-24). Script call run;
 * 0x8017 and 0x2018 are shared constants whose registers (r6/r8) and pool
 * order still differ from the reference. */
void HaidiaIe_RunScriptScene(void)
{
    u8 *rec7;

    rec7 = Battle_GetWorkObject1e0();
    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_CameraMoveTo(0x400000, 0x900000, 0x15e0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Event_PrepareObjectAndApplyValue(1, 0);
    Engine_EventOpenScreen();
    Engine_AudioPlayCue(17);
    BattleFx_SetBlock30Values128One();
    Engine_ActorSetPosition(23, 0x690000, 0x10b0000);
    Engine_TaskWait(1);
    Call3(Engine_ActorSetSpeed, 0, 0x13333, 0x9999);
    Engine_ActorWalkToAndWait(0, 93, 0x157);
    Engine_EventSetMessage((s32)MsgHaidiaOnlyTwoSurvived);
    Engine_EventShowMessage(23, 0);
    Engine_AudioPlayCue(61);
    rec7[85] = 0;
    Call2(Engine_CameraSetSpeed, 0x30000, 0x6000);
    Engine_CameraMoveTo(0x6d0000, 0xb00000, 0x1190000, 1);
    BattleFx_CommitObjectPositionAndWait();
    Engine_EventWait(40);
    Call3(Engine_ActorSetPosition, 24, 0x870000, 0xb10000);
    Call3(Engine_ActorSetSpeed, 24, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkTo, 24, 126, 0x102);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(23, 0xd000, 0);
    Engine_ActorWaitForMove(24);
    Engine_ActorSetAnimation(24, 1);
    SceneActor_SetPairZeroAndValue(24, 0x7000, 10);
    Engine_ActorSetAnimationAndWait(23, 3);
    Engine_ActorSetAnimationAndWait(24, 4);
    Call2(Engine_EventShowMessage, 0x2018, 0);
    Engine_ActorRunRepeatedMotion(23, 2);
    Event_SayThenWait(0x8017, 30);
    SceneActor_SetPairZeroAndValue(24, 0xb000, 20);
    Event_SayThenWait(0x2018, 10);
    SceneActor_SetPairZeroAndValue(23, 0xb000, 40);
    Engine_EventShowMessage(0x8017, 0);
    Engine_ActorSetAnimationAndWait(24, 4);
    Engine_EventShowMessage(0x2018, 0);
    SceneActor_SetPairZeroAndValue(23, 0xf000, 10);
    Engine_ActorRunRepeatedMotion(23, 2);
    Engine_EventShowMessage(0x8017, 0);
    SceneActor_SetPairZeroAndValue(24, 0x6000, 20);
    Engine_ActorSetAnimationAndWait(24, 3);
    Engine_EventWait(20);
    Event_SayThenWait(0x2018, 20);
    FieldScene_RunGroupChoreography();
    Engine_EventEnd();
}

/* Stages the two moving actors around actor 25, sends actors 23, 24, 25 and
 * the leader off on their own actions, then advances the area's scene state
 * and records this house as the scene the party returns to. */
void FieldScene_RunGroupChoreography(void)
{
    extern u8 Data_0200ac00[];

    s32 record;
    s32 walk_speed;
    s32 turn_speed;
    s32 approach_speed;

    Engine_ActorSetChildValue(25, 15);
    record = Object_GetById(25);
    Engine_ActorSetSpriteFlags(record, 0);
    Call3(Engine_ActorSetPosition, 25, 0, 0x14b0000);
    Engine_TaskWait(1);
    Call2(Engine_EventShowMessage, 0x1019, 0);
    Call3(Engine_ActorShowEmote, 23, 0x100, 0);
    Call3(Engine_ActorShowEmote, 24, 0x100, 40);
    Engine_ActorSetPosition(25, 0, 0);
    Call3(Engine_ActorFaceDirection, 23, 0x5000, 0);
    walk_speed = 0x5000;
    SceneActor_SetPairZeroAndValue(24, walk_speed, 40);
    Engine_CameraSetSpeed(0x18000, 0x3000);
    Engine_CameraMoveTo(0x590000, 0xb00000, 0x1390000, 1);
    BattleFx_CommitObjectPositionAndWait();
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 23, 0xe000, 0);
    SceneActor_SetPairZeroAndValue(24, 0x7000, 40);
    Camera_SetSpeed(0xcccc, 0x1999);
    Engine_CameraMoveTo(0x640000, 0x900000, 0x14d0000, 1);
    Call3(Engine_ActorSetSpeed, 23, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 24, 0x10000, 0x8000);
    Call3(Engine_ActorWalkTo, 23, 105, 0x149);
    Engine_EventWait(10);
    Actor_WalkTo(24, 124, 0x149);
    Engine_ActorWaitForMove(23);
    Engine_ActorSetAnimation(23, 1);
    Engine_ActorFaceDirection(23, walk_speed, 0);
    Engine_ActorWaitForMove(24);
    Engine_ActorSetAnimation(24, 1);
    Engine_ActorFaceDirection(24, walk_speed, 0);
    Engine_ActorSetChildValue(25, 0);
    record = Object_GetById(25);
    Engine_ActorSetSpriteFlags(record, 1);
    Call3(Engine_ActorSetPosition, 25, 0, 0x14b0000);
    Call3(Engine_ActorSetSpeed, 25, 0x13333, 0x9999);
    Actor_WalkToAndWait(25, 37, 0x153);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(23, 3);
    Engine_EventOpenMessage(23, 0);
    Call3(Engine_ActorShowEmote, 25, 0x101, 0);
    approach_speed = 0xd000;
    SceneActor_SetPairZeroAndValue(0, approach_speed, 10);
    Engine_EventChooseYesNo(0, 0);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(23, 0, 0);
    turn_speed = 0x8000;
    SceneActor_SetPairZeroAndValue(24, turn_speed, 20);
    Engine_ActorRunRepeatedMotion(25, 2);
    Event_SayThenWait(0x1019, 10);
    Call3(Object_SetTargetAndCallback, 0, 0x10019, (s32)Data_0200ac00);
    Call3(Engine_ActorWalkToAndWait, 25, 93, 0x169);
    SceneActor_SetPairZeroAndValue(25, approach_speed, 40);
    Event_SayThenWait(25, 20);
    Engine_ActorStop(0);
    Engine_ActorFaceDirection(23, 0, 0);
    SceneActor_SetPairZeroAndValue(24, turn_speed, 15);
    Engine_ActorSetAnimation(23, 3);
    Engine_ActorSetAnimationAndWait(24, 3);
    Engine_ActorFaceDirection(23, walk_speed, 0);
    SceneActor_SetPairZeroAndValue(24, walk_speed, 30);
    Engine_ActorSetAnimationAndWait(24, 4);
    Engine_EventShowMessage(0x2018, 0);
    SceneActor_SetPairZeroAndValue(0, approach_speed, 30);
    SceneActor_SetPairZeroAndValue(0, 0x4000, 40);
    Engine_ActorRunRepeatedMotion(23, 2);
    Engine_ActorSetAnimationAndWait(23, 3);
    Event_SayThenWait(23, 20);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Engine_ActorSetAttachedEffect(25, 0x102);
    Engine_EventWait(80);
    Engine_ActorEnableActionCallback(24, (s32)HaidiaIe_Actor24Actions);
    Engine_EventWait(6);
    Engine_ActorEnableActionCallback(23, (s32)HaidiaIe_Actor23Actions);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(0, (s32)HaidiaIe_LeaderActions);
    Engine_EventWait(6);
    Object_SetActionCallbackAndRefreshById(25, (s32)HaidiaIe_Actor25Actions);
    /* FAKEMATCH: the do/while loads the linked scene after the store. */
    do {
        gGameState.unknown_200[0x22b - 0x200] = 2;
    } while (0);
    {
        const void *message = &SceneId_HaidiaIe;

        Party_SetFields1ceAnd1d0(message, 19);
        Event_SetPair1d4(message, 19);
    }
    BattleFx_SetWeightedResult(12, 4);
    Engine_GameFlagSet(0x11a);
}

/* Scripted scenes and the paired effects they spawn. */
void HaidiaIe_RunScene015B4(void)
{

    u32 i;
    s32 record;
    s32 v5;

    Engine_AudioPlayCue(17);
    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_CameraMoveTo(0x400000, 0x900000, 0x15e0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Call3(Engine_ActorSetPosition, 0, 0x300000, 0x15a0000);
    Call3(Engine_ActorSetPosition, 25, 0x4e0000, 0x1660000);
    Call3(Engine_ActorSetPosition, 23, 0x670000, 0x1560000);
    Call3(Engine_ActorSetPosition, 24, 0x700000, 0x1680000);
    Call3(Engine_ActorFaceDirection, 23, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 24, 0x8000, 0);
    Engine_ActorSetAnimation(0, 16);
    record = Object_GetById(0);
    *(s32 *)(record + 24) = -0x10000;
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetAnimation(25, 7);
    record = Object_GetById(25);
    {
        u8 *motion = *(u8 **)(record + 80);
        s32 shown = 0x1555;

        *(u16 *)(motion + 30) = shown;
    }
    record = Object_GetById(25);
    Engine_ActorSetSpriteFlags(record, 0);
    {
        u8 *work = (u8 *)gEventWork;

        *(s32 *)(work + 0x1c0) = 0x100;
    }
    Engine_EventOpenScreen();
    BattleFx_SetBlock30Values128One();
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Call3(Engine_ActorFaceDirection, 23, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 24, 0xc000, 40);
    Engine_ActorSetAnimationAndWait(23, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(24, 3);
    Call3(Engine_ActorFaceDirection, 23, 0x8000, 10);
    Call3(Engine_ActorFaceDirection, 24, 0x8000, 10);
    Engine_ActorSetSpritePriority(0, 3);
    Engine_ActorSetSpritePriority(25, 3);
    Call3(Engine_ActorSetSpeed, 23, 0x26666, 0x13333);
    v5 = 128;
    record = Object_GetById(23);
    *(s32 *)(record + 68) = 0x28f;
    *(s32 *)(record + 72) = (v5 << 8);
    Engine_ActorEnableActionCallback(23, (s32)Data_0200aa48);
    Engine_EventWait(24);
    Engine_ActorSetSpeed(24, 0x26666, 0x13333);
    record = Object_GetById(24);
    *(s32 *)(record + 68) = 0x28f;
    *(s32 *)(record + 72) = (v5 << 8);
    Object_SetActionCallbackAndRefreshById(24, (s32)Data_0200ab2c);
    Engine_EventWait(40);
    BattleFx_SetBlock30ValuesMaxZero();
    BattleFx_SetBlock30Values12Zero();
    Engine_TaskWait(20);
    BattleFx_SetBlock30Values12Zero();
    Engine_TaskWait(60);
    BattleFx_SetBlock30Values12Zero();
    Engine_TaskWait(20);
    BattleFx_SetBlock30ValuesMaxZero();
    Engine_EventWait(40);
    *(s32 *)(((s32)gEventWork + 0x1c8)) = 120;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_GameFlagClear(0x834);
    Engine_EventRequestExit(9);
    Engine_EventEnd();
}

void Scene_RunExtendedActorSequence(void)
{
    extern const u8 Data_0200ac00[];

    u8 *record;
    struct EventWork *work;
    const u8 *base5_200ac00;
    s32 v5;
    const u8 *base5_200ac90;
    s32 v6;
    const u8 *base5_200adf0;

    Owner_RefreshActiveRatios(1);
    Engine_EventBegin();
    Call6(Engine_MapCopyCellsTo, 42, 53, 42, 54, 3, 1);
    Call4(Engine_CameraMoveTo, 0xb40000, 0x100000, 0x26a0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    record = Object_GetById(22);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Object_GetById(23);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Object_GetById(24);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Object_GetById(25);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Object_GetById(26);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Object_GetById(29);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    Engine_ActorSetSpritePriority(0, 1);
    Engine_ActorSetSpritePriority(1, 1);
    Engine_ActorSetSpritePriority(17, 1);
    Engine_ActorSetSpritePriority(16, 1);
    Engine_ActorSetSpritePriority(15, 1);
    Call3(Engine_ActorSetPosition, 0, 0xd00000, 0x32e0000);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Call3(Engine_ActorShowEmote, 12, 0x101, 40);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 20);
    Call1(Engine_EventSetMessage, MsgHaidiaWhysEveryoneHanging);
    Event_SayThenWait(12, 10);
    Call3(Engine_ActorShowEmote, 11, 0x102, 20);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 10);
    Event_SayThenWait(11, 10);
    Engine_ActorSetAnimationAndWait(12, 3);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(11, 2);
    Event_SayThenWait(11, 10);
    Call3(Engine_ActorShowEmote, 12, 0x100, 40);
    Call3(Engine_ActorSetSpeed, 12, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 12, 184, 0x26a);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 60);
    Event_SayThenWait(12, 20);
    Call3(Engine_ActorSetSpeed, 11, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 11, 168, 0x26a);
    Call3(SceneActor_SetPairZeroAndValue, 11, 0xf000, 10);
    Engine_ActorSetAnimation(11, 4);
    Event_SayThenWait(11, 20);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 10);
    Engine_ActorRunRepeatedMotion(12, 1);
    Engine_EventShowMessage(12, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorSetSpeed, 30, 0x26666, 0x13333);
    Call3(Engine_ActorSetPosition, 30, 0x6e0000, 0x2e80000);
    Engine_TaskWait(2);
    Engine_ActorSetAnimation(30, 3);
    Engine_ActorEnableActionCallback(30, (s32)Data_0200ac14);
    Engine_EventWait(40);
    base5_200ac00 = Data_0200ac00;
    Call3(Object_SetTargetAndCallback, 11, 0x1001e, (s32)(base5_200ac00));
    Call3(Object_SetTargetAndCallback, 12, 0x1001e, (s32)(base5_200ac00));
    Object_RefreshSelectorById(30);
    Engine_ActorStop(11);
    Engine_ActorStop(12);
    Engine_EventWait(60);
    Call3(Engine_ActorShowEmote, 11, 0x105, 0);
    Call3(Engine_ActorShowEmote, 12, 0x105, 120);
    Engine_ActorFaceDirection(11, 0x1000, 0);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 80);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 40);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 20);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(12, 0x5000, 60);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 40);
    SceneActor_SetPairZeroAndValue(12, 0x5000, 60);
    Call3(Engine_ActorShowEmote, 12, 0x101, 80);
    Engine_ActorFaceDirection(11, 0x3000, 0);
    Call3(Engine_ActorWalkToAndWait, 12, 184, 0x276);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 20);
    SceneActor_SetPairZeroAndValue(12, 0x5000, 20);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 20);
    Call3(Engine_ActorShowEmote, 12, 0x101, 40);
    Call3(Engine_ActorShowEmote, 11, 0x101, 40);
    Call3(Engine_ActorWalkToAndWait, 11, 168, 0x276);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(11, 0x3000, 40);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 40);
    SceneActor_SetPairZeroAndValue(11, 0x3000, 40);
    Call3(Engine_ActorShowEmote, 11, 0x101, 40);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 10);
    Event_SayThenWait(11, 20);
    Engine_ActorSetAnimationAndWait(12, 3);
    Event_SayThenWait(12, 10);
    Call3(Engine_ActorShowEmote, 11, 0x100, 20);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 20);
    SceneActor_SetPairZeroAndValue(11, 0x3000, 20);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 20);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 60);
    Engine_ActorSetAnimationAndWait(11, 3);
    Event_SayThenWait(11, 10);
    {
        struct FieldActor *actor = Object_GetById(30);

        if (actor != NULL) {
            Engine_ActorSetPosition(31, actor->x.fixed, actor->z.fixed);
        }
    }
    v5 = 254;
    Engine_TaskWait(2);
    Object_GetById(30)->priority_flags &= v5;
    Object_GetById(31)->priority_flags &= v5;
    Engine_ActorSetSpritePriority(30, 2);
    Engine_ActorSetSpritePriority(31, 2);
    Call3(Engine_ActorSetSpeed, 31, 0x39999, 0x1cccc);
    Engine_ActorSetAnimation(31, 2);
    base5_200ac90 = Data_0200ac90;
    Engine_ActorEnableActionCallback(31, base5_200ac90);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(30, 3);
    Call3(Engine_ActorSetSpeed, 30, 0x4cccc, 0x26666);
    Object_SetActionCallbackAndRefreshById(30, base5_200ac90);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(12, 2);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 10);
    Event_SayThenWait(12, 10);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Event_SayThenWait(11, 20);
    Engine_ActorSetAnimation(12, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(20);
    Call3(Engine_ActorSetSpeed, 11, 0x26666, 0x13333);
    Call3(Engine_ActorSetSpeed, 12, 0x26666, 0x13333);
    Engine_ActorEnableActionCallback(11, (s32)Data_0200acf8);
    Engine_EventWait(10);
    Call2(Engine_CameraSetSpeed, 0x26666, 0x4ccc);
    v6 = 0;
    Battle_GetWorkObject1e0()->motion_flags = v6;
    Call4(Engine_CameraMoveTo, 0xd70000, 0x100000, 0x3210000, 1);
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(12, (s32)Data_0200ad74);
    Object_RefreshSelectorById(12);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 120);
    Engine_ActorRunRepeatedMotion(13, 2);
    Engine_EventWait(20);
    Event_SayThenWait(13, 20);
    Engine_ActorFaceDirection(0, 0, 0);
    SceneActor_SetPairZeroAndValue(1, 0x9000, 20);
    SceneActor_SetPairZeroAndValue(0, 0xc000, 10);
    SceneActor_SetPairZeroAndValue(1, 0xb000, 10);
    Engine_ActorSetAnimation(0, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(1, 3);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(16, 2);
    Engine_EventWait(20);
    Event_SayThenWait(16, 10);
    Call3(Engine_ActorSetSpeed, 16, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 16, 216, 0x320);
    Call3(Engine_ActorFaceDirection, 16, 0x4000, 0);
    Engine_PartyGiveItem(180, 0);
    Call3(Engine_ActorWalkToAndWait, 16, 0x108, 0x320);
    Call3(Engine_ActorFaceDirection, 16, 0x6000, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 40);
    Call3(SceneActor_SetPairZeroAndValue, 1, 0xf000, 10);
    Event_SayThenWait(1, 10);
    Engine_ActorSetAnimationAndWait(17, 4);
    Event_SayThenWait(17, 10);
    Engine_ActorFaceDirection(1, 0x1000, 0);
    Call3(Engine_ActorShowEmote, 1, 0x103, 20);
    ObjectMotion_Launch(1, 4, 60);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(14, 0xd000, 10);
    Event_SayThenWait(14, 60);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Call3(Engine_ActorShowEmote, 16, 0x102, 0);
    Call3(Engine_ActorShowEmote, 17, 0x102, 0);
    Call3(Engine_ActorShowEmote, 18, 0x102, 0);
    Call3(Engine_ActorShowEmote, 19, 0x102, 80);
    Call3(Engine_ActorShowEmote, 17, 0x100, 0);
    Event_SayThenWait(17, 60);
    Object_LinkObjectAndSetCallback(0, 17);
    Object_LinkObjectAndSetCallback(1, 17);
    Call3(Engine_ActorWalkToAndWait, 17, 216, 0x320);
    Call3(Engine_ActorFaceDirection, 17, 0x4000, 0);
    Event_SayThenWait(17, 60);
    Engine_PartyGiveItem(207, 0);
    Engine_ActorStop(0);
    Engine_ActorStop(1);
    Call3(Engine_ActorWalkToAndWait, 17, 0x110, 0x330);
    Call3(Engine_ActorFaceDirection, 17, 0x8000, 0);
    Engine_ActorRunRepeatedMotion(1, 2);
    SceneActor_SetPairZeroAndValue(1, 0x9000, 10);
    Event_SayThenWait(1, 10);
    Engine_ActorFaceDirection(14, 0x3000, 0);
    SceneActor_SetPairZeroAndValue(0, 0, 10);
    Call3(Engine_ActorShowEmote, 0, 0x101, 60);
    Engine_ActorRunRepeatedMotion(16, 1);
    Event_SayThenWait(16, 10);
    Call3(SceneActor_SetPairZeroAndValue, 1, 0xf000, 10);
    Call3(Engine_ActorShowEmote, 1, 0x101, 20);
    Engine_ActorSetAnimationAndWait(16, 4);
    Event_SayThenWait(16, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Engine_EventShowMessage(18, 0);
    Engine_ActorFaceDirection(1, 0xd000, 0);
    Engine_ActorSetAnimation(18, 4);
    Engine_EventShowMessage(18, 0);
    Engine_ActorStartRepeatedMotion(18, 3);
    Engine_EventShowMessage(18, 0);
    Engine_ActorSetAnimationAndWait(16, 3);
    Engine_ActorSetAnimation(19, 3);
    Engine_ActorSetAnimation(17, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(24, 3);
    Engine_ActorSetAnimation(18, 3);
    Engine_ActorSetAnimation(27, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(28, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(25, 3);
    Engine_ActorSetAnimation(20, 3);
    Engine_ActorSetAnimationAndWait(21, 3);
    ObjectMotion_Launch(15, 2, 10);
    ObjectMotion_Launch(15, 4, 40);
    Event_SayThenWait(15, 10);
    Engine_ActorFaceDirection(1, 0xb000, 0);
    SceneActor_SetPairZeroAndValue(0, 0xc000, 20);
    SceneActor_SetPairZeroAndValue(15, 0xd000, 10);
    Event_SayThenWait(15, 10);
    SceneActor_SetPairZeroAndValue(15, 0x9000, 20);
    SceneActor_SetPairZeroAndValue(15, 0x5000, 10);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(14, 3);
    Engine_ActorSetAnimation(17, 3);
    Engine_ActorSetAnimation(20, 3);
    Engine_ActorSetAnimation(23, 3);
    Engine_ActorSetAnimation(26, 3);
    Engine_ActorSetAnimation(29, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(15, 3);
    Engine_ActorSetAnimation(18, 3);
    Engine_ActorSetAnimation(21, 3);
    Engine_ActorSetAnimation(24, 3);
    Engine_ActorSetAnimation(27, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(13, 3);
    Engine_ActorSetAnimation(16, 3);
    Engine_ActorSetAnimation(19, 3);
    Engine_ActorSetAnimation(22, 3);
    Engine_ActorSetAnimation(25, 3);
    Engine_ActorSetAnimationAndWait(28, 3);
    Engine_EventWait(80);
    ObjectMotion_Launch(11, 4, 0);
    ObjectMotion_Launch(14, 4, 0);
    ObjectMotion_Launch(17, 4, 0);
    ObjectMotion_Launch(20, 4, 0);
    ObjectMotion_Launch(23, 4, 0);
    ObjectMotion_Launch(26, 4, 0);
    ObjectMotion_Launch(29, 4, 0);
    ObjectMotion_Launch(12, 4, 0);
    ObjectMotion_Launch(15, 4, 0);
    ObjectMotion_Launch(18, 4, 0);
    ObjectMotion_Launch(21, 4, 0);
    ObjectMotion_Launch(24, 4, 0);
    ObjectMotion_Launch(27, 4, 0);
    ObjectMotion_Launch(13, 4, 0);
    ObjectMotion_Launch(16, 4, 0);
    ObjectMotion_Launch(19, 4, 0);
    ObjectMotion_Launch(22, 4, 0);
    ObjectMotion_Launch(25, 4, 0);
    ObjectMotion_Launch(28, 4, 0);
    Engine_MessageShowCentered(MsgHaidiaFarewell, 1);
    v5 = 1;
    Engine_EventWait(80);
    Object_GetById(0)->priority_flags |= v5;
    {
        struct FieldActor *actor = Object_GetById(1);
        u8 value = (u8)(v5 | actor->priority_flags);

        actor->priority_flags = value;
    }
    Call3(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 80);
    SceneActor_SetPairZeroAndValue(0, 0x4000, 10);
    SceneActor_SetPairZeroAndValue(1, 0x5000, 20);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    base5_200adf0 = Data_0200adf0;
    Engine_ActorEnableActionCallback(0, base5_200adf0);
    Engine_EventWait(20);
    Call2(Engine_CameraSetSpeed, 0x6666, 0xccc);
    Engine_CameraMoveTo(0xd80000, 0x100000, 0x3890000, 1);
    Engine_EventWait(20);
    Call3(Engine_ActorSetSpeed, 1, 0xcccc, 0x6666);
    Engine_ActorEnableActionCallback(1, base5_200adf0);
    Engine_EventWait(60);
    work = *(struct EventWork **)Data_03001ebc;
    work->start_transition = 0x100;
    work->transition_frames = 60;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_ActorStop(0);
    Engine_ActorStop(1);
    Engine_EventRequestExit(10);
    Engine_EventEnd();
}

/* Shows the next line of dialogue, then holds the scene for a moment. */
void Event_SayThenWait(s32 speaker, s32 frames)
{
    Event_ShowMessage(speaker, 0);
    Engine_EventWait(frames);
}

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Actor_FaceDirection(a, b, 0);
    Engine_EventWait(c);
}

void OverlayObject_UpdateOnFrameBit1(s32 obj)
{
    if ((*(volatile s32 *)&gFrameCount & 2) != 0) {
        Engine_ObjectSetPartPalettes(obj, 7);
    } else {
        Engine_ObjectSetPartPalettes(obj, 0);
    }
    if ((*(volatile s32 *)&gFrameCount & 15) == 0) {
        HaidiaIe_SpawnEffectPair(obj);
    }
}

void SceneEffect_UpdateByFrameBits(s32 no)
{
    volatile s32 *p = (volatile s32 *)&gFrameCount;
    if ((*p & 1) != 0) {
        s32 t = Math_RemainderUnsigned((u32)*p >> 1, 6);
        Engine_ObjectSetPartPalettes(no, t);
    }
    if ((*p & 15) == 0) {
        HaidiaIe_SpawnEffectPair(no);
    }
}

void SceneEffect_UpdateByFrameBit(s32 no)
{
    volatile s32 *p = (volatile s32 *)&gFrameCount;
    if ((*p & 1) != 0) {
        s32 t = Math_RemainderUnsigned((u32)*p >> 1, 6);
        Engine_ObjectSetPartPalettes(no, t);
    }
}

void SceneEffect_AnimateVerticalPositive(struct SceneVerticalEffect *effect)
{
    struct VerticalEffectAnchor *anchor;
    s32 frame;
    s32 amplitude;
    s32 offset;

    anchor = effect->anchor;
    effect->frame = effect->frame + 1;
    frame = (s16)effect->frame;
    if (frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }
    amplitude = Engine_MathSin(frame << 10);
    effect->amplitude_x = amplitude;
    effect->amplitude_y = amplitude;
    effect->x = anchor->x;
    offset = 0x10000;
    effect->y = effect->y + offset;
    offset = offset - amplitude;
    effect->z = anchor->z + (offset * 4 + offset) + 0x80000;
}

void SceneEffect_AnimateVerticalNegative(struct SceneVerticalEffect *effect)
{
    struct VerticalEffectAnchor *anchor;
    s32 frame;
    s32 amplitude;
    s32 offset;

    anchor = effect->anchor;
    effect->frame = effect->frame + 1;
    frame = (s16)effect->frame;
    if (frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }
    amplitude = Engine_MathSin(frame << 10);
    effect->amplitude_x = amplitude;
    effect->amplitude_y = -amplitude;
    effect->x = anchor->x;
    offset = 0x10000;
    effect->y = effect->y + offset;
    offset = offset - amplitude;
    effect->z = anchor->z - (offset * 4 + offset) + 0x100000;
}

/* Haidia house: spawns the linked pair of effect objects above the parent actor, with a cue, and gives the two their update routines and priorities. */
void HaidiaIe_SpawnEffectPair(union PairObject *parent)
{
    union PairObject *pair[2];
    union PairObject *child;
    struct PairSprite *part;
    struct FieldSprite *sprite;
    struct PairWork *work = Data_03001f30;
    s32 i;

    Engine_AudioPlayCue(131);
    for (i = 0; i < 2; ++i) {
        child = (union PairObject *)Engine_ObjectCreate(26,
            parent->object.actor.x.fixed, parent->object.actor.y.fixed,
            parent->object.actor.z.fixed);
        pair[i] = child;
        if (child != NULL) {
            child->words[5] = parent->words[5];
            part = (struct PairSprite *)child->object.actor.sprite;
            child->object.actor.motion_flags = 0;
            child->object.effect.spin = 0;
            child->link.parent = parent;
            if (part != NULL) {
                sprite = &part->sprite;
                AnimationObjects_SelectAnimation(sprite, 0);
                sprite->flags = 0;
                Resource_ResetEntry(sprite->vram_block);
                sprite->vram_block = work->vram_block;
                /* FAKEMATCH: a plain byte access; the struct field store
                 * leaves a dead QImode zero that takes r3 from the +85
                 * address. */
                *(u8 *)&sprite->unknown_1d |= 1;
                sprite->tile = (ResourceTableEntries[sprite->vram_block].offset >> 5) & 0x3ff;
                sprite->full_color = 0;
                sprite->shape = 1;
                ((struct WorldMapOam *)sprite)->size = 2;
                part->detail->field_16 = 0;
            }
        }
    }
    {
        union PairObject *p = pair[0];
        struct FieldSprite *sp = p->object.actor.sprite;

        p->object.actor.update = (void (*)(union FieldObject *))SceneEffect_AnimateVerticalNegative;
        sp->priority = 2;
    }
    {
        struct FieldActor *p = &pair[1]->object.actor;
        struct FieldSprite *sp = p->sprite;

        sp->priority = 2;
        p->update = (void (*)(union FieldObject *))SceneEffect_AnimateVerticalPositive;
        p->priority_flags = 2;
    }
}

void SceneState_ApplyPair140And0(void)
{
    Engine_PsynergyBegin(140, 0);
}

void FieldScene_Forward4dac(void)
{
    BattleEffect_CleanupSceneObjects();
}

void FieldScene_RunStep15(void)
{
    SceneEffect_UpdateByFrameBits((s32)Object_GetById(15));
}

void FieldScene_RunStep17(void)
{
    SceneEffect_UpdateByFrameBits((s32)Object_GetById(17));
}

void FieldScene_RunStep20(void)
{
    SceneEffect_UpdateByFrameBit((s32)Object_GetById(20));
}

void SceneState_SetValues352_365_2116_2117_40(void)
{
    GameFlag_Set(352);
    GameFlag_Set(0x16d);
#if defined(TBS_EDITION_FR) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
    GameFlag_Set(0x830);
    GameFlag_Set(0x831);
    GameFlag_Set(0x832);
    GameFlag_Set(0x833);
    GameFlag_Set(0x835);
    GameFlag_Set(0x836);
    GameFlag_Set(0x837);
    GameFlag_Set(0x838);
    GameFlag_Set(0x839);
    GameFlag_Set(0x83a);
    GameFlag_Set(0x840);
    GameFlag_Set(0x841);
    GameFlag_Set(0x842);
    GameFlag_Set(0x806);
    GameFlag_Set(0x807);
    GameFlag_Set(0x808);
    GameFlag_Set(0x800);
    GameFlag_Set(0x801);
    GameFlag_Set(0x823);
    GameFlag_Set(0x802);
    GameFlag_Set(0xf01);
    GameFlag_Set(0x81a);
    GameFlag_Set(0x804);
    GameFlag_Set(0xf02);
    GameFlag_Set(0x821);
    GameFlag_Set(0x825);
    GameFlag_Set(0x809);
    GameFlag_Set(0x80a);
    GameFlag_Set(0x818);
    GameFlag_Set(0x80b);
    GameFlag_Set(0x80c);
    GameFlag_Set(0x80d);
    GameFlag_Set(0x80e);
    GameFlag_Set(0x80f);
    GameFlag_Set(0x813);
    GameFlag_Set(0x810);
    GameFlag_Set(0x811);
    GameFlag_Set(0x819);
    GameFlag_Set(0x83b);
    GameFlag_Set(0x83c);
    GameFlag_Set(0x83d);
    GameFlag_Set(0x83e);
    GameFlag_Set(0x83f);
    GameFlag_Set(0x814);
    GameFlag_Set(0x879);
    GameFlag_Set(0x815);
    GameFlag_Set(0x81b);
    GameFlag_Set(0x81d);
    GameFlag_Set(0x87a);
#else
    GameFlag_Set(0x844);
    GameFlag_Set(0x845);
#endif
    Engine_EventRequestExit(40);
}
