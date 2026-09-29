/* NONMATCHING: resource_372 at 0x0200a8a4, from FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_E.C, stays listing.
 *
 * Remaining difference: its messages have catalogue names now; 316 halfwords
 * still differ from the ROM, and it names symbols no link defines
 * (ObjectMotion_EnableActionAndSetCallback_1,
 * ObjectMotion_MarkActiveAndSetActionCallback_1,
 * BattleEffect_PlayQueuedSound_1, Scene_GetRecord_1_020028a4, ...); it also
 * lacks declarations it needs to compile.
 */

#include "GROUP_DEPARTURE.H"
extern u8 MsgHaidiaDontSupposeTwo[];
extern u8 MsgHaidiaGoLookNorth[];

/* Sets up records 26, 24, 25, 9 and 10 (position/speed, facing, movement),
 * runs several timed particle/object sequences against constant tables, then
 * takes two branches whose outcome picks entries out of the 0xe9b
 * and0xea1 byte tables to drive further record 9/22 setup calls. */
void RunEventScript01(void)
{
    u32 i;
    s32 entry;
    s32 record;
    s32 base5_200cec8;
    s32 base5_e9b;
    s32 base5_ea1;

    Actor_FaceDirection(26, 0x3000, 0);
    Actor_FaceDirection(24, 0xd000, 0);
    Actor_FaceDirection(25, 0xb000, 0);
    Actor_FaceDirection(9, 0x3000, 0);
    Actor_FaceDirection(10, 0xd000, 20);
    Actor_SetAnimation(26, 3);
    Actor_SetAnimation(24, 3);
    Actor_SetAnimation(25, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(25, 3);
    Event_Wait(20);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x860000, -1, 0x4ab0000, 1);
    Actor_SetSpeed(26, 0x19999, 0xcccc);
    Actor_SetSpeed(9, 0x19999, 0xcccc);
    ObjectMotion_EnableActionAndSetCallback_1(26, 0x200cab4);
    ObjectMotion_MarkActiveAndSetActionCallback_1(9, 0x200ca78);
    Audio_PlayCue(158);
    Map_AnimateCells(0x200d7a0, 38, 72);
    Event_Wait(10);
    Actor_WalkToAndWait(9, 149, 0x497);
    Actor_SetPosition(9, 0, 0);
    Actor_WalkToAndWait(25, 250, 0x4be);
    BattleEffect_PlayQueuedSound_1();
    Actor_FaceDirection(10, 0x3000, 0);
    Actor_FaceDirection(24, 0x3000, 0);
    Actor_FaceDirection(25, 0x3000, 0);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(24, 6);
    Actor_SetAnimation(25, 6);
    /* For records 10, 24 and 25: fetch the record's entry pointer, fetch a
     * value from that record's own state, and store a derived value into
     * the entry's field at offset 100. */
    entry = Scene_GetRecord_1_020028a4(10);
    record = Random_Next();
    *(u16 *)(entry + 100) = (Func_02007008(record, 90) + 60);
    entry = Scene_GetRecord_2(24);
    record = Random_Next();
    *(u16 *)(entry + 100) = (Func_02007020(record, 90) + 60);
    entry = Scene_GetRecord_3(25);
    record = Random_Next();
    *(u16 *)(entry + 100) = (Func_02007038(record, 90) + 60);
    base5_200cec8 = (s32)Data_0200cec8;
    Actor_EnableActionCallback(10, base5_200cec8);
    ObjectMotion_EnableActionAndSetCallback_3(24, base5_200cec8);
    Actor_EnableActionCallback(25, base5_200cec8);
    Object_LookupAndStep_1(26);
    BattleRuntime_WaitIfModeZero_3(10);
    Audio_PlayCue(159);
    Map_AnimateCells(0x200d7e2, 38, 72);
    Event_Wait(30);
    BattleEffect_PlayQueuedSound_2();
    Camera_MoveTo(0x700000, -1, 0x4c90000, 1);
    Audio_PlayCue(158);
    Map_AnimateCells(0x200d78a, 35, 73);
    Event_Wait(20);
    BattleEffect_PlayQueuedSound_3();
    Actor_EnableActionCallback(9, 0x200cb28);
    Event_Wait(20);
    Actor_EnableActionCallback(26, 0x200cb9c);
    Event_Wait(40);
    Audio_PlayCue(159);
    Map_AnimateCells(0x200d7cc, 35, 73);
    Object_LookupAndStep_2(26);
    BattleEffect_PlayQueuedSound_4();
    Event_Wait(40);
    base5_e9b = (s32)MsgHaidiaGoLookNorth;
    Event_SetMessage(base5_e9b);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAnimationAndWait(26, 3);
    Event_ShowMessageAndWait(0x201a, 0, 40);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(26, 3);
    Event_Wait(30);
    ObjectMotion_EnableActionAndSetCallback_7(9, 0x200cc0c);
    Actor_EnableActionCallback(26, 0x200cc5c);
    Event_Wait(40);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x690000, -1, 0x43e0000, 1);
    Object_LookupAndStep_3(9);
    Actor_FaceDirection(9, 0, 0);
    Actor_ShowEmote(9, 0x100, 40);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(22, 0x8000, 10);
    Actor_WalkToAndWait(9, 105, 0x43e);
    Actor_RunRepeatedMotion(9, 2);
    Event_OpenMessage(0x8009, 0);
    Actor_FaceDirection(22, 0, 0);
    /* Branch on a condition; pass byte 4 or byte 5 of the 0xe9b
     * table to the corresponding follow-up call. */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(9, 3);
        Event_SetMessage((base5_e9b + 4));
    } else {
        Actor_RunRepeatedMotion(9, 2);
        Event_SetMessage((base5_e9b + 5));
    }
    Event_ShowMessage(0x8009, 0);
    Actor_FaceDirection(22, 0x8000, 40);
    Actor_ShowEmote(9, 0x100, 30);
    base5_ea1 = (s32)MsgHaidiaDontSupposeTwo;
    Event_SetMessage(base5_ea1);
    Event_OpenMessage(0x8009, 0);
    /* Branch on a condition; each side reads a different byte of the
     * 0xea1 table and runs its own follow-up sequence. */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(9, 3);
        Event_SetMessage((base5_ea1 + 1));
        Event_ShowMessageAndWait(0x8009, 0, 30);
        Actor_FaceDirection(22, 0x8000, 20);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimation(22, 3);
        Actor_SetAnimationAndWait(9, 3);
        Event_Wait(40);
    } else {
        Actor_ShowEmote(9, 0x105, 90);
        Actor_ShowEmote(9, 0x103, 40);
        Actor_SetAnimation(9, 4);
        Event_SetMessage((base5_ea1 + 2));
        Event_ShowMessage(0x8009, 0);
    }
    Actor_EnableActionCallback(9, 0x200cca8);
    Event_Wait(90);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(20);
    Actor_SetAnimation(22, 2);
    /* If a record pointer is returned, pass its s16 fields at offsets 10
     * and 18 through to the follow-up call. */
    record = Scene_GetRecord_4(0);
    if (record != 0) {
        Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(22);
    Actor_SetPosition(22, 0, 0);
}
