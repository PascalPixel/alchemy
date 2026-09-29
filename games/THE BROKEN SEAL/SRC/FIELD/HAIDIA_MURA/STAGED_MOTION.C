#include "STAGED_MOTION.H"

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
        Event_SetMessage(MSG_THE_GROUND_STILL_SHAKES);
        Event_ShowMessage(10, 0);
    } else {
        Event_SetMessage(MSG_YOUR_GRANDPA_IS_THE_MAYOR);
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
        Event_SetMessage(MSG_SUKURETA_HASNT_COME_BACK);
        Event_ShowMessage(14, 0);
    } else if (GameFlag_IsSet(flag) == 0) {
        GameFlag_Set(flag);
        Event_SetMessage(MSG_DO_YOU_NEED_TO_GO_PAST);
        Actor_FaceEachOther(14, ACTOR_PARTY_LEADER, 4);
        Event_AskYesNo(14, 0);
    } else {
        Event_SetMessage(MSG_DONT_GO_BEYOND_SUKURETAS_COTTAGE);
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
        Event_SetMessage(MSG_YOU_MAKE_ME_SO_MAD);
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
        Event_SetMessage(MSG_ILL_GET_YOU_FOR_MY_FLOWERS);
        Event_ShowMessageAndWait(18, 0, 20);
    }
    Event_End();
}

void SceneDialogue_RunActor21FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x202) != 0) {
        Event_SetMessage(MSG_I_TOLD_GERALD_IT_WAS_ALL_RIGHT);
    } else {
        Event_SetMessage(MSG_IT_WONT_RAIN_FOR_SOME_TIME);
    }
    Event_ShowMessage(21, 0);
    Event_End();
}

void SceneDialogue_RunActor10LineAndFlag81f(void)
{
    Event_Begin();
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 10, 20);
    Event_SetMessage(MSG_IS_JASMINE_BACK_YET);
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
    Event_SetMessage(MSG_I_FELT_ANOTHER_ONE);
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
    Event_SetMessage(MSG_NO_TRAVELERS_SINCE_THE_ERUPTION);
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
        Event_SetMessage(MSG_YOU_CANT_BE_ROBIN);
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

    rec8 = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    rec7 = Value1(Engine_ActorGet, ACTOR_JASMINE);
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
    Event_SetMessage(MSG_SUKURETA_IS_WAITING_FOR_US);
    if (*(s32 *)(rec8 + 8) < *(s32 *)(rec7 + 8)) {
        Event_ShowMessageAndWait(0xa005, 0, 2);
    } else {
        Event_ShowMessageAndWait(0x8005, 0, 2);
    }
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(2);
    Actor_SetAnimation(ACTOR_JASMINE, 2);
    record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_JASMINE, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_JASMINE);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 110, 0x12f);
    Event_End();
}
