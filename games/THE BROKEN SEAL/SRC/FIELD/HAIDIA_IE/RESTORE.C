/* Return visits, supplemental sequences and the entry state. */
#include "HAIDIA.H"
extern u8 MsgHaidiaGoAidElders[];
extern u8 MsgHaidiaOnlyTwoSurvived[];
extern u8 MsgHaidiaThePsynergyStoneIsGone[];
extern u8 MsgHaidiaThisIsVale[];
extern u8 MsgHaidiaYouCameBackHome[];

void Villager_WelcomeBack(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x302) != 0) {
        Event_SetMessage((s32)MsgHaidiaThisIsVale);
    } else {
        Event_SetMessage((s32)MsgHaidiaYouCameBackHome);
        GameFlag_Set(0x302);
    }
    Event_ShowMessage(11, 0);
    Event_End();
}

void Scene_PsynergyStoneIsGone(void)
{
    struct Obj *p = Engine_ActorGet(21);
    Event_Begin();
    p->f38 = 0x80000000;
    p->f3c = 0x80000000;
    p->f40 = 0x80000000;
    Actor_SetAnimation(21, 1);
    Actor_Stop(21);
    Actor_ShowEmote(21, 256, 40);
    p->f06 = 0xb000;
    Event_Wait(20);
    Actor_StartRepeatedMotion(21, 2);
    Event_SetMessage((s32)MsgHaidiaThePsynergyStoneIsGone);
    Event_ShowMessageAndWait(21, 0, 40);
    Actor_FaceActor(21, ACTOR_PARTY_LEADER, 20);
    Actor_StartRepeatedMotion(21, 2);
    Event_ShowMessage(21, 0);
    GameFlag_Set(0x306);
    Actor_Stop(21);
    Task_Wait(1);
    Actor_EnableActionCallback(21, Data_0200ae34);
    Event_End();
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
    Event_RequestExit(no);
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value_0200beb4 as its first argument. */
void FieldScene_RunSupplementalSequenceOne(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 44, 7);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 0x117);
    Call1(SceneState_SetWork1c0AndRun, 1);
}

/* Sets step 188, then runs a pair of 6-argument setup calls for indices 0
 * and 2 sharing the same trailing four values, followed by a pair of
 * 3-argument calls sharing the same leading two arguments, and a closing
 * 1-argument call. */
void FieldScene_RunSupplementalSequenceTwo(void)
{
    Audio_PlayCue(188);
    Map_CopyCellsTo(0, 63, 51, 8, 2, 2);
    Task_Wait(10);
    Map_CopyCellsTo(2, 63, 51, 8, 2, 2);
    Task_Wait(10);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 306);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 296);
    Call1(SceneState_SetWork1c0AndRun, 2);
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value_0200beb4 as its first argument. */
void FieldScene_RunSupplementalSequenceThree(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 43, 15); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 230, 0x197);
    Call1(SceneState_SetWork1c0AndRun, 3);
}

/* Runs four scene calls in sequence: a single-argument call, a call that
 * passes the address of Value_0200beb4 with two more values, a call that
 * passes 0, 374, and 0x1a3, and a final single-argument call. */
void FieldScene_RunSupplementalSequenceFour(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 52, 18); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 374, 0x1a3); /* object_id 0, x 374, z 0x1a3 */
    Call1(SceneState_SetWork1c0AndRun, 4);
}

/* Runs a fixed sequence of four scripted calls: one keyed off Value_0200beb4
 * with two small numeric arguments, one with a 0x222 argument, and two plain
 * single-argument calls. */
void FieldScene_RunSupplementalSequenceFive(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 41, 32); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 200, 0x222);
    Call1(SceneState_SetWork1c0AndRun, 5);
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
    Call1(SceneState_SetWork1c0AndRun, 6);
}

/* Runs four scripted scene calls in sequence, passing a byte's address and a
 * handful of small immediate constants to each. */
void FieldScene_RunSupplementalSequenceSeven(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 51, 39); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 358, 0x29e);
    Call1(SceneState_SetWork1c0AndRun, 7);
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

    if (Value1_02000940(Engine_GameFlagIsSet, 0x90b) != 0) {
        Engine_ActorSetPosition(8, 0, 0);
    }
    if (Value1_02000940(Engine_GameFlagIsSet, 0x90c) != 0) {
        Call3(Engine_ActorSetPosition, 9, 0, 0);
    }
    if (Value1_02000940(Engine_GameFlagIsSet, 0x90d) != 0) {
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
    record = Engine_ActorGet(8);
    *(s32 *)(record + 28) = (v5 << 9);
    record = Value1_02000940(Engine_ActorGet, 9);
    *(s32 *)(record + 28) = (v5 << 9);
    record = Value1_02000940(Engine_ActorGet, 10);
    *(s32 *)(record + 28) = (v5 << 9);
    if (Value1_02000940(Engine_GameFlagIsSet, 0x87a) != 0) {
        Call6(Engine_MapCopyCellsTo, 97, 2, 80, 5, 2, 2);
        Call6(Engine_MapCopyCellsTo, 42, 53, 42, 54, 3, 1);
        Engine_MapRedraw();
        Engine_TaskWait(1);
    } else {
        if (Value1_02000940(Engine_GameFlagIsSet, 0x834) != 0) {
            Unnamed_08094ac8();
            BattleFx_StartTwelveFrameBlend();
            Call6(Scene_CopyCellAttributes, 21, 38, 1, 1, 18, 41);
            record = Value1_02000940(Engine_GameFlagIsSet, 0x840);
            if (record == 0) {
                goto L_02000aaa;
            }
            Engine_ActorSetPosition(17, 0, 0);
            Engine_ActorSetPosition(18, 0, 0);
            Call3(Object_SetTargetAndCallback, 19, 0x10000, (s32)Data_0200ac00);
        } else {
            if (Value1_02000940(Engine_GameFlagIsSet, 0x815) != 0) {
                Call3(Engine_ActorSetPosition, 16, 0xb40000, 0x2380000);
                Call6(Engine_MapCopyCellsTo, 92, 2, 80, 5, 2, 2);
                Call6(Engine_MapCopyCellsTo, 42, 53, 42, 54, 3, 1);
                Engine_MapRedraw();
                Engine_TaskWait(1);
            }
        }
        L_02000aaa:;
        if (gGameState.entrance == 12) {
            Scene_RunExtendedActorSequence();
        } else {
            if (Value1_02000940(Engine_GameFlagIsSet, 0x834) != 0) {
                record = Engine_ActorGet(20);
                *(s32 *)(record + 24) = 0x4ccc;
                *(s32 *)(record + 28) = 0x4ccc;
                record = Engine_ActorGet(20);
                Engine_ActorSetSpriteFlags(record, 0);
                record = Engine_ActorGet(21);
                *(s32 *)(record + 24) = 0x9999;
                *(s32 *)(record + 28) = 0x9999;
                Engine_ActorSetAnimation(13, 5);
            } else {
                if (Value1_02000940(Engine_GameFlagIsSet, 0x815) != 0) {
                    Call3(Engine_ActorSetPosition, 21, 0x14b0000, 0xf90000);
                    record = Engine_ActorGet(21);
                    Engine_ActorSetSpriteFlags(record, 0);
                }
            }
            if (Value1_02000940(Engine_GameFlagIsSet, 0x840) != 0) {
                Engine_ActorSetPosition(26, 0, 0);
                Engine_ActorSetPosition(22, 0, 0);
            }
            if (gGameState.entrance == 19) {
                HaidiaIe_RunScene015B4();
            } else {
                if (Value1_02000940(Engine_GameFlagIsSet, 0x834) != 0) {
                    record = Value1_02000940(Engine_GameFlagIsSet, 0x842);
                    if (record == 0) {
                        goto L_02000b6a;
                    }
                    HaidiaIe_RunScriptScene();
                } else {
                    L_02000b6a:;
                    if (Value1_02000940(Engine_GameFlagIsSet, 0x834) != 0) {
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

    if (Value1(Engine_GameFlagIsSet, 0x834) != 0 && Value1(Engine_GameFlagIsSet, 0x840) == 0) {
        Engine_EventBegin();
        Call2(Engine_CameraSetSpeed, 0x19999, 0x3333);
        Camera_MoveTo(0xc50000, -1, 0x3000000, 1);
        BattleFx_CommitObjectPositionAndWait();
        Call1(Engine_EventSetMessage, (s32)MsgHaidiaGoAidElders);
        Engine_ActorRunRepeatedMotion(19, 2);
        Call3(Engine_EventShowMessageAndWait, 0x4013, 0, 10);
        Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
        Call3(Engine_ActorSetSpeed, 25, 0x10000, 0x8000);
        Call3(Engine_ActorWalkToAndWait, 0, 179, 0x315);
        record = Value1(Engine_ActorGet, 0);
        if (record != 0) {
            Engine_ActorSetPosition(25, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Call3(Engine_ActorWalkToAndWait, 25, 179, 0x324);
        Engine_ActorFaceEachOther(0, 25, 40);
        Engine_ActorFaceDirection(0, 0, 0);
        Actor_FaceDirection(25, 0, 0);
        Actor_SetAnimation(17, 3);
        Actor_SetAnimationAndWait(18, 3);
        Engine_ActorFaceEachOther(17, 18, 0);
        Engine_EventWait(20);
        Engine_ActorStartRepeatedMotion(17, 1);
        Call3(Engine_EventShowMessageAndWait, 0x4011, 0, 10);
        Engine_ActorSetAnimation(18, 3);
        Engine_EventShowMessageAndWait(18, 0, 10);
        Engine_ActorFaceDirection(17, 0, 0);
        Call3(Engine_ActorFaceDirection, 18, 0xf000, 10);
        Engine_ActorSetAnimationAndWait(19, 3);
        Call3(Engine_EventShowMessageAndWait, 0x4013, 0, 10);
        Call3(Engine_ActorSetSpeed, 17, 0x19999, 0xcccc);
        Call3(Engine_ActorSetSpeed, 18, 0x19999, 0xcccc);
        Engine_ActorEnableActionCallback(17, (s32)Data_0200aef0);
        Event_Wait(20);
        Engine_ActorEnableActionCallback(18, (s32)Data_0200aef0);
        Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
        Call3(Engine_ActorFaceDirection, 25, 0xc000, 60);
        Value2(Engine_ActorEnableActionCallback, 0, (s32)Data_0200af50);
        Call2(Object_SetActionCallbackAndRefreshById, 25, (s32)Data_0200af78);
        Engine_EventWait(20);
        Engine_ActorFaceDirection(0, 0, 0);
        Engine_ActorFaceDirection(25, 0, 10);
        Engine_EventShowMessage(25, 0);
        Call3(Engine_ActorFaceDirection, 19, 0x8000, 0);
        Value3(SceneActor_SetPairZeroAndValue, 26, 0x6000, 20);
        Actor_RunRepeatedMotion(26, 2);
        Event_SayThenWait(26, 10);
        Engine_ActorSetAnimation(0, 3);
        Actor_SetAnimationAndWait(25, 3);
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(19, 2);
        Value2(Engine_EventOpenMessage, 0x4013, 0);
        if (Value2(Engine_EventChooseYesNo, 0, 0) == 1) {
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
        Call2(Engine_CameraSetSpeed, 0x13333, 0x2666);
        Call4(Engine_CameraMoveTo, 0xd70000, -1, 0x2f60000, 1);
        BattleFx_CommitObjectPositionAndWait();
        Call2(Engine_CameraSetSpeed, 0xcccc, 0x1999);
        Call4(Engine_CameraMoveTo, 0xcd0000, -1, 0x30a0000, 1);
        Actor_EnableActionCallback(22, (s32)Data_0200a874);
        Object_RefreshSelectorById(22);
        Value3(SceneActor_SetPairZeroAndValue, 22, 0x2000, 60);
        Engine_ActorRunRepeatedMotion(19, 2);
        Event_SayThenWait(19, 10);
        Engine_ActorSetAnimationAndWait(22, 3);
        Event_SayThenWait(22, 20);
        Engine_ActorSetAnimationAndWait(19, 3);
        Event_Wait(10);
        SceneActor_SetPairZeroAndValue(19, unk, 30);
        Event_SayThenWait(unk + 19, 10);
        SceneActor_SetPairZeroAndValue(26, 0xe000, 30);
        Engine_ActorSetAnimationAndWait(26, 3);
        SceneActor_SetPairZeroAndValue(19, 0x8000, 30);
        Engine_ActorRunRepeatedMotion(19, 2);
        Event_SayThenWait(unk + 19, 10);
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, 25, 40);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        SceneActor_SetPairZeroAndValue(25, 0, 20);
        SceneActor_SetPairZeroAndValue(26, 0x8000, 30);
        Engine_ActorSetAnimationAndWait(26, 3);
        Event_SayThenWait(26, 30);
        Value3(SceneActor_SetPairZeroAndValue, 26, 0xc000, 30);
        Engine_ActorSetAnimationAndWait(26, 3);
        Actor_SetAnimationAndWait(22, 3);
        Actor_SetAnimation(25, 2);
        record = Value1(Engine_ActorGet, 0);
        if (record != 0) {
            Engine_ActorSetDestination(25, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(25);
        Actor_SetPosition(25, 0, 0);
        Engine_ActorSetAnimation(26, 2);
        record = Value1(Engine_ActorGet, 0);
        if (record != 0) {
            Actor_SetDestination(26, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(26);
        Actor_SetPosition(26, 0, 0);
        Engine_ActorSetAnimation(22, 2);
        record = Value1(Engine_ActorGet, 0);
        if (record != 0) {
            Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(22);
        Engine_ActorSetPosition(22, 0, 0);
        Call3(Object_SetTargetAndCallback, 19, 0x10000, (s32)Data_0200ac00);
        Call1(Engine_GameFlagSet, 0x840);
        Event_End();
    }
}

/* NONMATCHING: 496 of 496 bytes, 11 halfword edits (2026-09-24). Script call run;
 * 0x8017 and 0x2018 are shared constants whose registers (r6/r8) and pool
 * order still differ from the reference. */
void HaidiaIe_RunScriptScene(void)
{
    u8 *rec7;

    rec7 = Value0(Battle_GetWorkObject1e0);
    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Engine_TaskWait(1);
    Call4(Engine_CameraMoveTo, 0x400000, 0x900000, 0x15e0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Event_PrepareObjectAndApplyValue(1, 0);
    Engine_EventOpenScreen();
    Engine_AudioPlayCue(17);
    BattleFx_SetBlock30Values128One();
    Call3(Engine_ActorSetPosition, 23, 0x690000, 0x10b0000);
    Engine_TaskWait(1);
    Call3(Engine_ActorSetSpeed, 0, 0x13333, 0x9999);
    Call3(Engine_ActorWalkToAndWait, 0, 93, 0x157);
    Call1(Engine_EventSetMessage, (s32)MsgHaidiaOnlyTwoSurvived);
    Engine_EventShowMessage(23, 0);
    Engine_AudioPlayCue(61);
    rec7[85] = 0;
    Call2(Engine_CameraSetSpeed, 0x30000, 0x6000);
    Call4(Engine_CameraMoveTo, 0x6d0000, 0xb00000, 0x1190000, 1);
    BattleFx_CommitObjectPositionAndWait();
    Engine_EventWait(40);
    Call3(Engine_ActorSetPosition, 24, 0x870000, 0xb10000);
    Call3(Engine_ActorSetSpeed, 24, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkTo, 24, 126, 0x102);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 23, 0xd000, 0);
    Engine_ActorWaitForMove(24);
    Engine_ActorSetAnimation(24, 1);
    Value3(SceneActor_SetPairZeroAndValue, 24, 0x7000, 10);
    Engine_ActorSetAnimationAndWait(23, 3);
    Engine_ActorSetAnimationAndWait(24, 4);
    Call2(Engine_EventShowMessage, 0x2018, 0);
    Engine_ActorRunRepeatedMotion(23, 2);
    Event_SayThenWait(0x8017, 30);
    SceneActor_SetPairZeroAndValue(24, 0xb000, 20);
    Event_SayThenWait(0x2018, 10);
    Value3(SceneActor_SetPairZeroAndValue, 23, 0xb000, 40);
    Engine_EventShowMessage(0x8017, 0);
    Engine_ActorSetAnimationAndWait(24, 4);
    Engine_EventShowMessage(0x2018, 0);
    Value3(SceneActor_SetPairZeroAndValue, 23, 0xf000, 10);
    Engine_ActorRunRepeatedMotion(23, 2);
    Engine_EventShowMessage(0x8017, 0);
    Value3(SceneActor_SetPairZeroAndValue, 24, 0x6000, 20);
    Engine_ActorSetAnimationAndWait(24, 3);
    Engine_EventWait(20);
    Event_SayThenWait(0x2018, 20);
    FieldScene_RunGroupChoreography();
    Engine_EventEnd();
}
