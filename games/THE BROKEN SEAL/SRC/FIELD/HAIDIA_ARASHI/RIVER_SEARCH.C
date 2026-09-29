#include "GROUP_DEPARTURE.H"

extern u8 MsgHaidiaDontSupposeTwo[];
extern u8 MsgHaidiaGoLookNorth[];
extern u8 HaidiaArashi_ActorNineScriptA[];
extern u8 HaidiaArashi_ActorNineScriptB[];
extern u8 HaidiaArashi_ActorNineScriptC[];
extern u8 HaidiaArashi_ActorNineScriptD[];
extern u8 HaidiaArashi_ActorTwentySixScriptA[];
extern u8 HaidiaArashi_ActorTwentySixScriptB[];
extern u8 HaidiaArashi_ActorTwentySixScriptC[];
extern u8 HaidiaArashi_CellSteps4[];
extern u8 HaidiaArashi_CellSteps5[];

/* The storm night by the river: while the cells shake, actors 9 and 26
 * run their scripts, actors 10, 24 and 25 each get a random count of 60
 * to 149 and actor 8's script, actor 9 says she will look north and
 * sends the others to the plaza, and asks the party twice for help before
 * actor 22 follows the leader out. */
void HaidiaArashi_RunRiverSearch(void)
{
    s32 entry;
    s32 record;
    s32 script;
    s32 north;
    s32 suppose;

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
    Actor_EnableActionCallback(26, HaidiaArashi_ActorTwentySixScriptA);
    Object_SetActionCallbackAndRefreshById(9, (s32)HaidiaArashi_ActorNineScriptA);
    Audio_PlayCue(158);
    Map_AnimateCells((const u16 *)HaidiaArashi_CellSteps2, 38, 72);
    Event_Wait(10);
    Actor_WalkToAndWait(9, 149, 0x497);
    Actor_SetPosition(9, 0, 0);
    Actor_WalkToAndWait(25, 250, 0x4be);
    BattleFx_PlayQueuedSound();
    Actor_FaceDirection(10, 0x3000, 0);
    Actor_FaceDirection(24, 0x3000, 0);
    Actor_FaceDirection(25, 0x3000, 0);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(24, 6);
    Actor_SetAnimation(25, 6);
    /* For records 10, 24 and 25: fetch the record's entry pointer, fetch a
     * value from that record's own state, and store a derived value into
     * the entry's field at offset 100. */
    entry = (s32)Actor_Get(10);
    record = Engine_RandomNext();
    *(u16 *)(entry + 100) = (IwramUnsignedRemainder(record, 90) + 60);
    entry = (s32)Actor_Get(24);
    record = Engine_RandomNext();
    *(u16 *)(entry + 100) = (IwramUnsignedRemainder(record, 90) + 60);
    entry = (s32)Actor_Get(25);
    record = Engine_RandomNext();
    *(u16 *)(entry + 100) = (IwramUnsignedRemainder(record, 90) + 60);
    script = (s32)HaidiaArashi_ActorEightScript;
    Actor_EnableActionCallback(10, (const u8 *)script);
    Actor_EnableActionCallback(24, (const u8 *)script);
    Actor_EnableActionCallback(25, (const u8 *)script);
    Object_RefreshSelectorById(26);
    Event_Wait(10);
    Audio_PlayCue(159);
    Map_AnimateCells((const u16 *)HaidiaArashi_CellSteps5, 38, 72);
    Event_Wait(30);
    BattleFx_PlayQueuedSound();
    Camera_MoveTo(0x700000, -1, 0x4c90000, 1);
    Audio_PlayCue(158);
    Map_AnimateCells((const u16 *)HaidiaArashi_CellSteps1, 35, 73);
    Event_Wait(20);
    BattleFx_PlayQueuedSound();
    Actor_EnableActionCallback(9, HaidiaArashi_ActorNineScriptB);
    Event_Wait(20);
    Actor_EnableActionCallback(26, HaidiaArashi_ActorTwentySixScriptB);
    Event_Wait(40);
    Audio_PlayCue(159);
    Map_AnimateCells((const u16 *)HaidiaArashi_CellSteps4, 35, 73);
    Object_RefreshSelectorById(26);
    BattleFx_PlayQueuedSound();
    Event_Wait(40);
    north = (s32)MsgHaidiaGoLookNorth;
    Event_SetMessage(north);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAnimationAndWait(26, 3);
    Event_ShowMessageAndWait(0x201a, 0, 40);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(26, 3);
    Event_Wait(30);
    Actor_EnableActionCallback(9, HaidiaArashi_ActorNineScriptC);
    Actor_EnableActionCallback(26, HaidiaArashi_ActorTwentySixScriptC);
    Event_Wait(40);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x690000, -1, 0x43e0000, 1);
    Object_RefreshSelectorById(9);
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
        Event_SetMessage((north + 4));
    } else {
        Actor_RunRepeatedMotion(9, 2);
        Event_SetMessage((north + 5));
    }
    Event_ShowMessage(0x8009, 0);
    Actor_FaceDirection(22, 0x8000, 40);
    Actor_ShowEmote(9, 0x100, 30);
    suppose = (s32)MsgHaidiaDontSupposeTwo;
    Event_SetMessage(suppose);
    Event_OpenMessage(0x8009, 0);
    /* Branch on a condition; each side reads a different byte of the
     * 0xea1 table and runs its own follow-up sequence. */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(9, 3);
        Event_SetMessage((suppose + 1));
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
        Event_SetMessage((suppose + 2));
        Event_ShowMessage(0x8009, 0);
    }
    Actor_EnableActionCallback(9, HaidiaArashi_ActorNineScriptD);
    Event_Wait(90);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(20);
    Actor_SetAnimation(22, 2);
    /* If a record pointer is returned, pass its s16 fields at offsets 10
     * and 18 through to the follow-up call. */
    record = (s32)Actor_Get(0);
    if (record != 0) {
        Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(22);
    Actor_SetPosition(22, 0, 0);
}
