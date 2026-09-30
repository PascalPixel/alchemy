#include "FUNE.H"
#include "CALL.H"
extern u8 MsgFuneMonsters2[];

void FieldScene_RunActorNinePresentationCycles(void)
{
    u32 i;
    u8 *rec9;
    u8 *record;

    Event_Begin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = (u8 *)Engine_ActorGet(0);
    Actor_SetSpriteFlags(record, 0);
    Task_Wait(1);
    ((void (*)())Event_CallWithLastActiveObjectId)((s32)FuneHobashira_EnsembleObjects);
    Task_Wait(1);
    OverlayObject_InitWithRandomFields(9);
    OverlayObject_InitWithRandomFields(10);
    OverlayObject_InitWithRandomFields(11);
    OverlayObject_InitWithRandomFields(12);
    OverlayObject_InitWithRandomFields(13);
    OverlayObject_InitWithRandomFields(14);
    OverlayObject_InitWithRandomFields(15);
    Engine_ActorEnableActionCallback(8, FuneHobashira_LookoutActions);
    gEventWork->start_transition = 0x203;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(120);
    rec9 = (u8 *)Engine_ActorGet(9);
    Actor_Stop(9);
    /* Reset record 9's waypoint/velocity fields: three fields to the
     * minimum s32, then four fields to zero. */
    RECORD_S32(rec9, 56) = -0x80000000;
    RECORD_S32(rec9, 60) = -0x80000000;
    RECORD_S32(rec9, 64) = -0x80000000;
    RECORD_S32(rec9, 36) = 0;
    RECORD_S32(rec9, 40) = 0;
    RECORD_S32(rec9, 44) = 0;
    RECORD_S32(rec9, 76) = 0;
    Event_Wait(20);
    Actor_SetSpeed(9, 0x80000, 0x40000);
    Call4(Object_SetPosition, rec9, 0xa40000, 0x900000, 0x1410000);
    Object_CommitPosition(rec9);
    Call4(Object_SetPosition, rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition(rec9);
    Call4(Object_SetPosition, rec9, 0xcc0000, 0x7c0000, 0x1410000);
    Object_CommitPosition(rec9);
    Call4(Object_SetPosition, rec9, 0x900000, 0, 0xa90000);
    Actor_Stop(8);
    Task_Wait(1);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_ShowEmote(8, 0x103, 60);
    Actor_SetSpeed(9, 0x20000, 0x10000);
    OverlayObject_InitWithRandomFields(9);
    Event_Wait(20);
    Engine_ActorEnableActionCallback(8, FuneHobashira_LookoutActions);
    Event_Wait(120);
    Actor_Stop(9);
    /* Same reset pattern on record 9 for the second cycle. */
    RECORD_S32(rec9, 56) = -0x80000000;
    RECORD_S32(rec9, 60) = -0x80000000;
    RECORD_S32(rec9, 64) = -0x80000000;
    RECORD_S32(rec9, 36) = 0;
    RECORD_S32(rec9, 40) = 0;
    RECORD_S32(rec9, 44) = 0;
    RECORD_S32(rec9, 76) = 0;
    Event_Wait(20);
    Actor_SetSpeed(9, 0x80000, 0x40000);
    Call4(Object_SetPosition, rec9, 0xa40000, 0x900000, 0x1410000);
    Object_CommitPosition(rec9);
    Actor_SetSpeed(9, 0x50000, 0x28000);
    Call4(Object_SetPosition, rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition(rec9);
    Call4(Object_SetPosition, rec9, 0xa40000, 0x720000, 0x1410000);
    Object_CommitPosition(rec9);
    Call4(Object_SetPosition, rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition(rec9);
    Call4(Object_SetPosition, rec9, 0xcc0000, 0x7c0000, 0x1410000);
    Object_CommitPosition(rec9);
    Object_SetPosition(rec9, 0x900000, 0, 0xa90000);
    Actor_Stop(8);
    Task_Wait(1);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_ShowEmote(8, 0x103, 60);
    Actor_SetSpeed(9, 0x20000, 0x10000);
    OverlayObject_InitWithRandomFields(9);
    Actor_Jump(8, 4, 20);
    Actor_Jump(8, 6, 40);
    Audio_PlayCue(29);
    GameFlag_Set(0x8f0);
    Event_SetMessage((s32)MsgFuneMonsters2);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_ShowEmote(8, 0x100, 0);
    Actor_WalkToAndWait(8, 164, 0x158);
    Event_Wait(40);
    Actor_RunRepeatedMotion(8, 2);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(12);
    Event_End();
}

void OverlayObject_InitWithRandomFields(s32 a)
{
    u8 *obj;
    u32 x;

    obj = (u8 *)Engine_ActorGet(a);
    Actor_SetSpritePriority(a, 1);
    obj[0x55] = 0;
    *(u16 *)(obj + 0x64) = Random16() >> 15;
    *(u16 *)(obj + 0x66) = Random16() >> 15;
    x = Random16();
    x <<= 2;
    x >>= 16;
    x <<= 16;
    x += 0x60000;
    *(s32 *)(obj + 0xc) = x;
    x = Random16();
    *(s32 *)(obj + 0x4c) = ((x * 3 << 13) >> 16) - 0x3000;
    *(s32 *)(obj + 0x18) = 0x14000;
    *(s32 *)(obj + 0x1c) = 0x14000;
    Actor_EnableActionCallback(a, FuneHobashira_DriftActions);
}

/* Drives ids 8 through 18 through position, scale, and flag updates in
 * sequence, stepping the shared scene phase at the start and end. */
void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    s32 rec;
    s32 id0_state;

    Event_Begin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    id0_state = (u8 *)Engine_ActorGet(0);
    Actor_SetSpriteFlags(id0_state, 0);
    Task_Wait(1);
    ((void (*)())Event_CallWithLastActiveObjectId)((s32)FuneHobashira_EnsembleObjects);
    Task_Wait(1);
    ((void (*)())Event_CallWithLastActiveObjectId)((s32)FuneHobashira_LandingObjects);
    Task_Wait(1);
    OverlayObject_InitWithRandomFields(9);
    OverlayObject_InitWithRandomFields(10);
    OverlayObject_InitWithRandomFields(11);
    OverlayObject_InitWithRandomFields(12);
    OverlayObject_InitWithRandomFields(13);
    OverlayObject_InitWithRandomFields(14);
    OverlayObject_InitWithRandomFields(15);
    Actor_EnableActionCallback(8, FuneHobashira_LookoutActions);
    gEventWork->start_transition = 0x203;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(0x12c);
    Audio_PlayCue(147);
    Event_Wait(100);
    Actor_Stop(9);
    Actor_Stop(10);
    Actor_Stop(11);
    Actor_Stop(12);
    Actor_Stop(13);
    Actor_Stop(14);
    Actor_Stop(15);
    Actor_SetSpeed(9, 0x30000, 0x18000);
    Actor_SetSpeed(10, 0x30000, 0x18000);
    Actor_SetSpeed(11, 0x30000, 0x18000);
    Actor_SetSpeed(12, 0x30000, 0x18000);
    Actor_SetSpeed(13, 0x30000, 0x18000);
    Actor_SetSpeed(14, 0x30000, 0x18000);
    Actor_SetSpeed(15, 0x30000, 0x18000);
    Actor_SetDestination(9, 0, 100);
    Actor_SetDestination(10, 60, 100);
    Actor_SetDestination(11, 120, 100);
    Actor_SetDestination(12, 180, 100);
    Actor_SetDestination(13, 240, 100);
    Actor_SetDestination(14, 0x140, 100);
    Actor_SetDestination(15, 0x17c, 100);
    Event_Wait(40);
    Actor_ShowEmote(8, 0x101, 0);
    Event_Wait(20);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(15, 0, 0);
    Event_Wait(100);
    rec = (u8 *)Engine_ActorGet(18);
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    Actor_SetPosition(18, 0xac0000, 0x1540000);
    Actor_Stop(8);
    Task_Wait(1);
    Actor_RunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x3000, 0);
    Audio_PlayCue(29);
    GameFlag_Set(0x8f0);
    for (i = 0; i < 32; i++) {
        *(s32 *)(rec + 24) += 0xccc;
        *(s32 *)(rec + 28) += 0xccc;
        Task_Wait(1);
    }
    Actor_ShowEmote(8, 0x101, 60);
    Actor_RunRepeatedMotion(8, 2);
    Actor_WalkToAndWait(8, 168, 0x154);
    Actor_WalkToAndWait(8, 200, 0x154);
    Actor_FaceDirection(8, 0x8000, 0);
    rec = (u8 *)Engine_ActorGet(17);
    *(s32 *)(rec + 24) = 0x12666;
    *(s32 *)(rec + 28) = 0x12666;
    *(s32 *)(rec + 8) = 0xac0000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x1540000;
    {
        /* Clear the flag word at +6. */
        s32 shown = 0;

        *(u16 *)(rec + 6) = shown;
    }
    *(s32 *)(rec + 68) = 0x6666;
    *(s32 *)(rec + 72) = 0x30000;
    Event_Wait(20);
    Actor_Jump(8, 6, 20);
    Audio_PlayCue(147);
    Event_Wait(20);
    Actor_EnableActionCallback(8, FuneHobashira_WaveActions);
    Event_Wait(80);
    Actor_SetSpritePriority(17, 1);
    Actor_SetSpeed(17, 0x10000, 0x8000);
    *(s32 *)(rec + 68) = 0x1999;
    *(s32 *)(rec + 72) = 0xb333;
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = 0x80000;
    Actor_SetDestination(17, 132, 0x168);
    Actor_SetDestination(18, 132, 0x168);
    Event_Wait(40);
    Actor_SetPosition(17, 0, 0);
    rec = (u8 *)Engine_ActorGet(8);
    *(s32 *)(rec + 24) = 0x10000;
    *(s32 *)(rec + 28) = 0x10000;
    {
        /* Set the flag word at +6. */
        s32 shown = 0x5000;

        *(u16 *)(rec + 6) = shown;
    }
    Event_Wait(40);
    gEventWork->start_transition = 0x202;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(13);
    Event_End();
}
