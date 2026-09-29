#include "STORY.H"

void RunEventScript01(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_OpenScreen();
    Event_WaitForScreen();
    Battle_SetObjectFlag5bWhenMode3();
    GameFlag_Set(0x94f);
    Actor_SetPosition(11, 0x16e00000, 0x49c0000);
    Actor_SetDestinationOffset(11, 24, 8);
    Actor_WaitForMove(11);
    Event_Wait(60);
    Actor_SetPosition(12, 0x16e00000, 0x49c0000);
    Actor_SetDestinationOffset(12, 12, 24);
    Event_Wait(30);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_FaceDirection(12, 0xd000, 0);
    Event_Wait(60);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Event_Wait(120);
    Actor_SetPosition(8, 0x16f80000, 0x4b80000);
    Event_Wait(60);
    Actor_SetAnimation(12, 2);
    record = Value1(Engine_ActorGet, 8);
    if (record != 0) {
        Actor_SetDestination(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(12);
    Actor_SetPosition(12, 0, 0);
    ((void (*)())Engine_EventWait)(60);
    Actor_SetAnimation(11, 2);
    record = Value1(Engine_ActorGet, 8);
    if (record != 0) {
        Actor_SetDestination(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(11);
    Actor_SetPosition(11, 0, 0);
    ((void (*)())Engine_EventWait)(60);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    record = Value1(Engine_ActorGet, 8);
    if (record != 0) {
        Actor_SetDestination(ACTOR_PARTY_LEADER, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    ((void (*)())Engine_EventWait)(60);
    Actor_SetSpeed(8, 0x8000, 0x4000);
    Actor_SetDestinationOffset(8, 56, 8);
    Actor_WaitForMove(8);
    Actor_SetDestinationOffset(8, 40, 40);
    Actor_WaitForMove(8);
    Actor_SetDestinationOffset(8, 8, 88);
    Actor_WaitForMove(8);
    Event_CloseScreen();
    Event_RequestExit(108);
    Event_End();
}

void FieldScene_RunActorTransferSequence(void)
{
    u8 *actor;
    u8 *record;
    s32 scale;
    s32 action;

    actor = Pointer1(Engine_ActorGet, 15);
    Event_Begin();
    Call2(BattleFx_ScheduleRatioTransition, 0x14000, 1);
    Task_Wait(4);
    Event_OpenScreen();
    Event_WaitForScreen();
    Battle_SetObjectFlag5bWhenMode3();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x16fc, 0x628);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_SetPosition(8, 0x16d80000, 0x6280000);
    Task_Wait(1);
    Actor_SetChildValue(8, 15);
    record = Actor_Get(8);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetSpeed(10, 0x19999, 0x6666);
    Actor_SetSpeed(11, 0x19999, 0x6666);
    Actor_SetSpeed(12, 0x19999, 0x6666);
    Actor_SetSpeed(13, 0x19999, 0x6666);
    Audio_PlayCue(141);
    Actor_EnableActionCallback(10, gTransferArrive10);
    Event_Wait(20);
    Actor_EnableActionCallback(11, gTransferArrive11);
    Event_Wait(20);
    Actor_EnableActionCallback(12, gTransferArrive12);
    Event_Wait(20);
    Call2(Object_SetActionCallbackAndRefreshById, 13, (s32)gTransferArrive13);
    Audio_PlayCue(0x121);
    record = Pointer1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_WalkToAndWait(ACTOR_GERALD, 0x1704, 0x640);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Actor_SetPosition(8, 0x16d80000, 0x6380000);
    Task_Wait(1);
    Event_SetMessage(MSG_WE_CANT_STAY_ANOTHER_MINUTE);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 10);
    Audio_PlayCue(141);
    Actor_EnableActionCallback(10, gTransferDepart10);
    Event_Wait(20);
    Actor_EnableActionCallback(11, gTransferDepart11);
    Event_Wait(20);
    Actor_EnableActionCallback(12, gTransferDepart12);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 10);
    Call2(Object_SetActionCallbackAndRefreshById, 13, (s32)gTransferDepart13);
    Audio_PlayCue(0x121);
    Event_Wait(20);
    Battle_ClearObjectFlag5bWhenMode3();
    Actor_SetAnimation(10, 1);
    Actor_SetAnimation(11, 1);
    Actor_SetAnimation(12, 1);
    Actor_SetAnimation(13, 1);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Battle_SetObjectFlag5bWhenMode3();
    Actor_SetPosition(9, 0x16080000, 0x6d80000);
    Task_Wait(1);
    Actor_SetSpeed(9, 0x13333, 0x9999);
    Actor_WalkToAndWait(9, 0x1608, 0x6c8);
    Actor_WalkToAndWait(9, 0x15f8, 0x6c8);
    Actor_WalkToAndWait(9, 0x15f8, 0x6f8);
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Actor_SetAttachedEffect(9, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(9, 0, 20);
    Actor_StartRepeatedMotion(9, 3);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetPosition(8, 0x16180000, 0x6f80000);
    Task_Wait(1);
    Actor_SetChildValue(8, 0);
    record = Actor_Get(8);
    Actor_SetSpriteFlags(record, 1);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_WalkToAndWait(8, 0x1608, 0x6f8);
    Event_Wait(20);
    Actor_StartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_FaceDirection(8, 0x3000, 60);
    Actor_FaceDirection(8, 0x8000, 10);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(9, 0x3000, 0);
    Actor_FaceDirection(8, 0x3000, 40);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Actor_StartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(0x2008, 0, 40);
    Actor_RunRepeatedMotion(9, 1);
    Actor_FaceDirection(9, 0, 10);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_ShowEmote(8, 0x105, 60);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_ShowEmote(9, 0x101, 60);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Battle_ClearObjectFlag5bWhenMode3();
    Audio_PlayCue(107);
    Camera_SetSpeed(0x40000, 0x40000);
    FieldScene_RunScene371_0200155c();
    Audio_PlayCue(0x121);
    Actor_ShowEmote(8, 0x100, 0);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_FaceDirection(9, 0, 40);
    Actor_FaceDirection(8, 0xb000, 0);
    Actor_FaceDirection(9, 0xb000, 0);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x15e80000, -1, 0x6c80000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(14, 0x15a80000, 0x6a80000);
    Task_Wait(1);
    Actor_SetSpeed(14, 0x4ccc, 0x2666);
    Actor_EnableActionCallback(14, gTransferGuide14);
    Event_Wait(160);
    *(s32 *)(actor + 72) = 0x1999;
    *(s32 *)(actor + 68) = 0x1999;
    *(s32 *)(actor + 24) = 0x18000;
    *(s32 *)(actor + 28) = 0x18000;
    {
        s32 shown = 0;

        *(u16 *)(actor + 100) = shown;
    }
    *(s32 *)(actor + 12) = 0x400000;
    {
        u8 *target = *(u8 **)(actor + 80);
        s32 shown = 0xf000;

        *(u16 *)(target + 30) = shown;
    }
    Actor_SetSpriteFlags(actor, 0);
    Object_SetAnimation(actor, 2);
    Task_Wait(1);
    record = Actor_Get(15);
    Actor_SetSpriteFlags(record, 0);
    Call2(Engine_TaskAddCallback, (s32)FieldScene_RunScene371_020017a4, 0xc80);
    do {
        Task_Wait(1);
    } while (*(s16 *)(actor + 100) == 0);
    record = Actor_Get(15);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(14);
    Actor_SetSpriteFlags(record, 0);
    Event_Wait(10);
    scale = 192;
    record = Actor_Get(9);
    *(s32 *)(record + 40) = (scale << 11);
    record = Pointer1(Engine_ActorGet, 8);
    *(s32 *)(record + 40) = (scale << 11);
    Audio_PlayCue(145);
    Camera_SetSpeed(0x40000, 0x40000);
    FieldScene_RunScene371_02001680();
    FieldScene_RunScene371_02001680();
    Event_Wait(60);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Camera_WaitForMove();
    Battle_SetObjectFlag5bWhenMode3();
    Actor_SetAttachedEffect(9, 0x102);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Call1(Engine_TaskRemoveCallback, (s32)FieldScene_RunScene371_020017a4);
    Task_Wait(1);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(15, 0, 0);
    Actor_FaceDirection(8, 0x8000, 10);
    Actor_Jump(8, 4, 40);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_FaceDirection(9, 0, 10);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(8, 0xc000, 40);
    Event_ShowMessageAndWait(0x2008, 0, 20);
    Actor_Jump(9, 4, 20);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x8000, 10);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_SetAttachedEffect(9, 0x102);
    Event_Wait(80);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 1);
    Actor_SetAnimationAndWait(9, 3);
    Actor_WalkToAndWait(8, 0x1618, 0x6f8);
    Actor_SetPosition(8, 0, 0);
    Actor_WalkToAndWait(9, 0x15f8, 0x6c8);
    Actor_WalkToAndWait(9, 0x1608, 0x6c8);
    Actor_WalkToAndWait(9, 0x1608, 0x6d8);
    Actor_SetPosition(9, 0, 0);
    Audio_PlayCue(141);
    Actor_EnableActionCallback(10, (s32)gTransferReturn10);
    Actor_EnableActionCallback(11, gTransferReturn11);
    Event_Wait(40);
    Actor_EnableActionCallback(12, gTransferReturn12);
    Event_Wait(40);
    Call2(Object_SetActionCallbackAndRefreshById, 13, (s32)gTransferReturn13);
    Battle_ClearObjectFlag5bWhenMode3();
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x170c0000, 0x6280000);
    Actor_SetPosition(ACTOR_GERALD, 0x17140000, 0x6400000);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0x16d80000, -1, 0x6480000, 1);
    Camera_WaitForMove();
    action = (s32)gTransferGather;
    Actor_EnableActionCallback(10, action);
    Event_Wait(20);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x16d80000, -1, 0x6080000, 1);
    Actor_EnableActionCallback(11, action);
    Event_Wait(20);
    Actor_EnableActionCallback(12, action);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Actor_EnableActionCallback(13, action);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Object_RefreshSelectorById(13);
    Audio_PlayCue(0x121);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0x16f80000, -1, 0x6480000, 1);
    Camera_WaitForMove();
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 80);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Pointer1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x16d80000, -1, 0x6480000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x16d8, 0x628);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x85a);
    Event_RequestExit(3);
    Event_End();
}

void FieldScene_RunScene371_0200155c(void)
{

    u32 i;
    s32 record;

    Camera_MoveTo(0x160c0000, -1, 0x6f80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16040000, -1, 0x6fc0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x160c0000, -1, 0x6f40000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x160c0000, -1, 0x6fc0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16040000, -1, 0x6f40000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x160a0000, -1, 0x6f80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16060000, -1, 0x6fa0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x160a0000, -1, 0x6f60000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x160a0000, -1, 0x6fa0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16060000, -1, 0x6f60000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Task_Wait(4);
}

void FieldScene_RunScene371_02001680(void)
{

    u32 i;
    s32 record;

    Camera_MoveTo(0x15ec0000, -1, 0x6c80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e40000, -1, 0x6cc0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15ec0000, -1, 0x6c40000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15ec0000, -1, 0x6cc0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e40000, -1, 0x6c40000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e80000, -1, 0x6c80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15ea0000, -1, 0x6c80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e60000, -1, 0x6ca0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15ea0000, -1, 0x6c60000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15ea0000, -1, 0x6ca0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e60000, -1, 0x6c60000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e80000, -1, 0x6c80000, 1);
    Task_Wait(4);
}

void FieldScene_RunScene371_020017a4(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 15);
    record = Value1(Engine_ActorGet, 14);
    *(s32 *)(rec7 + 8) = *(s32 *)(record + 8);
    *(s32 *)(rec7 + 16) = *(s32 *)(record + 16);
    if (*(s32 *)(rec7 + 12) < 0xa0000) {
        *(s32 *)(rec7 + 12) = 0xa0000;
        if (GameFlag_IsSet(0x200) == 0) {
            Audio_PlayCue(145);
            Object_SetAnimation(rec7, 3);
            GameFlag_Set(0x200);
            {
                u16 *target = (u16 *)(rec7 + 100);
                s32 shown = 1;

                *target = shown;
            }
        }
    }
}

void FieldScene_RunScene371_020017fc(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x6666, 0x3333);
    Actor_WalkToAndWait(8, 0x14a8, 0x918);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x927);
    Event_RequestExit(102);
    Event_End();
}

void FieldScene_RunScene371_02001888(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Task_Wait(1);
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
    do {
        Task_Wait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x927);
    Event_RequestExit(103);
    Event_End();
}

void FieldScene_RunScene371_02001938(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Task_Wait(1);
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
    do {
        Task_Wait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x927);
    Event_RequestExit(104);
    Event_End();
}

void FieldScene_RunScene371_020019e8(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Task_Wait(1);
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
    do {
        Task_Wait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x927);
    Event_RequestExit(105);
    Event_End();
}

void FieldScene_RunScene371_02001a98(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Task_Wait(1);
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    if (Value0(StoryScene_ComputeOpposingSlotDelta) == 11) {
        Engine_ActorEnableActionCallback(8, gTransferLeaderTurn);
    } else {
        Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
    }
    do {
        Task_Wait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x927);
    Event_RequestExit(106);
    Event_End();
}

void FieldScene_RunScene371_02001b5c(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x13e80000, 0x9180000);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Task_Wait(1);
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x6666, 0x3333);
    Actor_WalkToAndWait(8, 0x13c8, 0x918);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x93e);
    GameFlag_Clear(0x927);
    Event_RequestExit(107);
    Event_End();
}

void FieldScene_RunScene371_02001c08(void)
{

    u32 i;
    s32 record;

    Battle_ClearObjectFlag5bWhenMode3();
    Call2(BattleFx_ScheduleRatioTransition, 0x10000, 6);
    Event_WaitForDisplayField358Clear();
    Battle_SetObjectFlag5bWhenMode3();
    Actor_RunRepeatedMotion(8, 2);
    Event_SetMessage(MSG_AFTER_BRINGING_DJINNI_INTO_YOUR);
    Event_ShowMessage(8, 0);
    Event_Wait(30);
    Audio_PlayCue(111);
    Menu_AnimateSelectionToEntry(0, 2);
    GameFlag_Clear(0x16f);
    GameFlag_Clear(0x171);
    ItemMenu_Open();
    Actor_Jump(8, 4, 30);
    Event_SetMessage(MSG_NEXT_ILL_SHOW_HOW_CAN);
    Event_ShowMessage(8, 0);
    GameFlag_Clear(0x16f);
    GameFlag_Set(0x171);
    ItemMenu_Open();
    Event_Wait(30);
    BattleFx_SetWeightedResult(12, 6);
}
